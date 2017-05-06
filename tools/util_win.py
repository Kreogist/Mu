# -*- coding: utf-8 -*-
from os import *
from os.path import isfile, join
import os
import sys
from winreg import *

# Constants
# Register uninstallation path.
uninstRegPath=r'Software\Microsoft\Windows\CurrentVersion\Uninstall'

# Function name: isQtExist
# Find the Qt data on the hard drive with the information we find in the 
# registry.
def isQtExist(qtRegInfo):
    # Get the root path of binary installation.
    qtRootPath=qtRegInfo["location"]+"\\"+qtRegInfo["version"][0:3]
    # Get the compiler version of the Qt installation.
    qtRootSubDirs=listdir(qtRootPath)
    if len(qtRootSubDirs) != 1:
        # Error happend.
        return False
    # Save the compiler information.
    qtRegInfo["compiler"]=qtRootSubDirs[0]
    # Check the compiler information.
    if qtRegInfo["compiler"].startswith("msvc"):
        # Check whether the type is 64-bit or not.
        if "_64" in qtRegInfo["compiler"]:
            qtRegInfo["width"]=64
        else:
            qtRegInfo["width"]=32
    elif qtRegInfo["compiler"].startswith("mingw"):
        if "_32" in qtRegInfo["compiler"]:
            qtRegInfo["width"]=32
        else:
            qtRegInfo["width"]=64
    # Combine the compiler with the path to generate root.
    qtRegInfo["root"]=qtRootPath+"\\"+qtRegInfo["compiler"]
    # Probably we need to check whether it has bin and other dirs, ignore.
    return True

# Function name: isQtInstallItem
# Check whether the item is a Qt installation, if the item is a Qt installation,
# return true and the parsed information, or else return false.
def isQtInstallItem(appRegPath):
    # Get all the keys from the folder.
    appRegFolder=OpenKey(HKEY_CURRENT_USER, appRegPath);
    # Prepare the dictionary.
    appInfo={}
    # Loop and get the value in the folder.
    _, appRegValueCount, _ = QueryInfoKey(appRegFolder)
    for valueItemIndex in range(0, appRegValueCount):
        valueName, valueData, _ = EnumValue(appRegFolder, valueItemIndex)
        appInfo[valueName]=valueData
    # Check the basic information.
    if "DisplayName" in appInfo and "InstallLocation" in appInfo and \
            "DisplayVersion" in appInfo:
        # Okay now check the value of the item
        if "Qt" in appInfo["DisplayName"] and \
            len(appInfo["DisplayVersion"]) > 2 and \
            appInfo["DisplayVersion"].startswith("5."):
            # This is a Qt installation in the registry information.
            qtInst={"version"   : appInfo["DisplayVersion"],
                    "location"  : appInfo["InstallLocation"]}
            # Finished.
            return True, qtInst
    # Or else, failed.
    return False, {}

# Function name: findQtInstallation
# Find the Qt installed position and version information from registry.
def findQtInstallation():
    # Define the global variables.
    global uninstRegPath
    # Prepare the list.
    qtVersions = []
    # Get the uninstall folder registry item.
    uninstFolder=OpenKey(HKEY_CURRENT_USER, uninstRegPath)
    # Get the query info of the uninst folder.
    uninstItemCount, _, _ = QueryInfoKey(uninstFolder)
    # Enumerate all the item in the uninst folder.
    for uninstItemIndex in range(0, uninstItemCount):
        # Get the current uninst item
        isQt, qtInfo = \
                isQtInstallItem(uninstRegPath+"\\"+EnumKey(uninstFolder, \
                                                           uninstItemIndex))
        # Now, the qt info is only find in the registry.
        # Check whether it is exist in the folder.
        if isQt and isQtExist(qtInfo):
            qtVersions.append(qtInfo)
    # Give back the qt version.
    return qtVersions

# Function name: displayAllCompiler
# Display all the installation information on the screen
def displayAllCompiler(qtVersions):
    # List the data.
    itemIndex=0
    print(" # Version  Compiler     Type    Location")
    for itemIndex in range(0, len(qtVersions)):
        # Print the data.
        print(repr(itemIndex+1).rjust(2), \
                qtVersions[itemIndex]["version"].ljust(8), \
                qtVersions[itemIndex]["compiler"].ljust(12), \
                str(qtVersions[itemIndex]["width"])+"-bits", \
                qtVersions[itemIndex]["location"])