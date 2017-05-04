# -*- coding: utf-8 -*-
from os import *
from os.path import isfile, join
import os
import sys
import tempfile
import http.client
import shutil
import zipfile
from winreg import *

# Constants
# Register uninstallation path.
uninstRegPath=r'Software\Microsoft\Windows\CurrentVersion\Uninstall'

# Parameters
# -a, --all     Configure All
# Set configure for all the Qt installations.
compile_all = False
# -c, --compile Configure for specific version
# Set configure for specific compiler version
compile_version = ""
supported_compiler = ["mingw", "msvc"]

# Public variables.
workDir = None

# Function name: isQtExist
# Find the Qt data on the hard drive with the information we find in the 
# registry.
def isQtExist(qtRegInfo):
    # Get the root path of binary installation.
    qtRootPath=qtRegInfo["location"]+"\\"+qtRegInfo["version"][0:3]
    # Get the compiler version of the Qt installation.
    qtRootSubDirs=listdir(qtRootPath)
    if len(qtRootSubDirs) != 1:
        # Error happened.
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

# Function name: parseParameter
# Check all the parameter passed from command line.
def parseParameter():
    global compile_all
    global compile_version
    global supported_compiler
    configureList=list(sys.argv[1:]);
    # Pop until the list is empty.
    while len(configureList) > 0:
        # Get the first item in the list.
        parameter = configureList[0]
        configureList = configureList[1:]
        # Check the parameter.
        if parameter == "-h" or parameter == "--help":
            # Display the error information.
            print("Usage: python", sys.argv[0], "[options]")
            print("Options:")
            print("-a, --all        Set dependencies for all Qt installation")
            print("-c, --compiler   Set dependencies for specific Qt " + 
                  "installation")
            print("-h, --help       Display this information")
            print("-v, --version    Display the version of this tool")
            # Use false to force it close.
            return False
        elif parameter == "-v" or parameter == "--version":
            # Use false to force it close.
            return False
        elif parameter == "-a" or parameter == "--all":
            # Check the compiler version.
            if len(compile_version) > 0:
                print("Error: -a cannot enabled at the same time with -c.")
                return False
            # Set it to true
            compile_all = True
        elif parameter == "-c" or parameter == "--compiler":
            # It cannot be enabled with compile all at the same time.
            if compile_all:
                print("Error: -c cannot enabled at the same time with -a.")
                return False
            # Save the next parameter as the version.
            compile_version = configureList[0]
            configureList = configureList[1:]
            # Check the parameter in the list.
            if not compile_version in supported_compiler:
                print("Error:", compile_version, "is not a valid parameter" + \
                      " for", parameter)
                print("-c only supports the following type compiler (case " + 
                      "sensitive)")
                for item in supported_compiler:
                    print(" ", item)
                return False
        else:
            # Unknown parameter.
            print("Error: unknown parameter ", parameter)
            return False
    return True

# Function name: checkFileExist
# Check whether files are exist under qt installation folder.
def checkFileExist(qtInst, fileList):
    # Loop the item in file list.
    for subDir, fileName in fileList:
        # List the item in sub dir.
        subDirList = listdir(qtInst["root"] + "\\" + subDir)
        # Find the file name in the list.
        if fileName not in subDirList:
            # Failed to find data.
            return False
    # Complete, all exist.
    return True

# Function name: downloadPackage
# Download and return the package data.
def downloadPackage(qtInst, packageInfo):
    print("Downloading " + packageInfo['name'] + " from " + packageInfo['url'] + " ...", end='')
    # Prepare the conn.
    conn = 0;
    # Start to install sub package.
    if packageInfo['https']:
        conn = http.client.HTTPSConnection(packageInfo['url'])
    else:
        conn = http.client.HTTPConnection(packageInfo['url'])
    packageUrl, _ =packageInfo['path'][str(qtInst['width'])]
    # Get the url data.
    conn.request("GET", packageUrl)
    response = conn.getresponse()
    if response.status != 200:
        print("Failed")
        print("Connection timeout.")
        return None
    print("Done")
    # Or else, read the data.
    return response.read()

# Function: copytree
# Enhanced copy tree edition for support overwriting file.
def copytree(src, dst, symlinks=False):
    names = os.listdir(src)
    if not os.path.isdir(dst):
        os.makedirs(dst)
          
    errors = []
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        try:
            if symlinks and os.path.islink(srcname):
                linkto = os.readlink(srcname)
                os.symlink(linkto, dstname)
            elif os.path.isdir(srcname):
                copytree(srcname, dstname, symlinks)
            else:
                if os.path.isdir(dstname):
                    os.rmdir(dstname)
                elif os.path.isfile(dstname):
                    os.remove(dstname)
                shutil.copy2(srcname, dstname)
            # XXX What about devices, sockets etc.?
        except (IOError, os.error) as why:
            errors.append((srcname, dstname, str(why)))
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except OSError as err:
            errors.extend(err.args[0])
    try:
        shutil.copystat(src, dst)
    except WindowsError:
        # can't copy file access times on Windows
        pass
    except OSError as why:
        errors.extend((src, dst, str(why)))
    if errors:
        raise Error(errors) 

# Function name: copyDir
# Copy a directory to another path.
def copyDir(src, dst):
    # Check destination dir is exist or not.
    if not os.path.exists(dst):
        os.makedirs(dst)
    # Get all the items under source dir.
    subItems = os.listdir(src)
    # Check all the items
    for item in subItems:
        srcName = os.path.join(src, item)
        dstName = os.path.join(dst, item)
        if os.path.isdir(srcName):
            copytree(srcName, dstName)
        else:
            shutil.copy2(srcName, dstName)

# Function name: installPackage
# Download the package, uncompress the package and install the package.
def installPackage(qtInst, packageInfo):
    print("Start to install", packageInfo['name'])
    # Download the data.
    packageZipFileData = downloadPackage(qtInst, packageInfo)
    if packageZipFileData is None:
        return False
    print(len(packageZipFileData), "bytes downloaded.\nInstalling package...", end='')
    # Decompress the data.
    # Write zip file.
    packageZipFile = tempfile.TemporaryFile()
    packageZipFile.write(packageZipFileData)
    # Get the file list and dir list.
    extractFileList=packageInfo['extract']['files']
    extractDirList=packageInfo['extract']['dirs']
    # Get the prefix of the platform.
    _, extractPrefix=packageInfo['path'][str(qtInst['width'])]
    # Add prefix to all the items in the list.
    appendPrefix=(lambda path:path.replace("($pre)", extractPrefix))
    prefixFileList=[appendPrefix(path) for path, _ in extractFileList]
    prefixDirList=[appendPrefix(path) for path, _ in extractDirList]
    # Prepare the temp directory.
    global workDir
    workDir=tempfile.TemporaryDirectory()
    # Extract files.
    with zipfile.ZipFile(packageZipFile, 'r') as package:
        # Get all the item in the list.
        packageList=package.infolist()
        # Loop for all the item in the list.
        for packageItem in packageList:
            # Check the item file name is in the list or start with the dir.
            if packageItem.filename in prefixFileList:
                package.extract(packageItem, workDir.name)
            else:
                for extractPath in prefixDirList:
                    if packageItem.filename.startswith(extractPath):
                        package.extract(packageItem, workDir.name)
                        break
    # Now all the data should be extracted.
    # Move the directories.
    for extractIndex in range(0, len(extractDirList)):
        # Copy all the content to destination.
        copyDir(os.path.join(workDir.name, prefixDirList[extractIndex].replace('/', '\\')),
                qtInst["root"] + extractDirList[extractIndex][1])
    # Move the files.
    for extractIndex in range(0, len(extractFileList)):
        # Copy all the file to destination.
        shutil.copy2(os.path.join(workDir.name, prefixFileList[extractIndex].replace('/', '\\')),
                     qtInst["root"] + extractFileList[extractIndex][1])
    print("Done")

# Function name: configureQt
# Configure the Qt installation for compiling.
def configureQt(qtInst):
    # Display the data.
    print("\nChecking dependencies for Qt", qtInst["version"], "with", qtInst["compiler"])
    print("Installed at", qtInst["location"])
    # Check ffmpeg installation.
    print("Checking FFMpeg installation...", end='')
    if not checkFileExist(qtInst, [('lib', 'avcodec.lib')]):
        print("\nFailed to find FFMpeg library, start to install.")
        # Install the Ffmpeg for the installation.
        ffmpegDevInfo = {'name' : 'FFMpeg developing files',
                         'https' : True,
                         'url' : "ffmpeg.zeranoe.com",
                         'path': {
                           '32': ('/builds/win32/dev/ffmpeg-latest-win32-dev.zip', 'ffmpeg-latest-win32-dev'),
                           '64': ('/builds/win64/dev/ffmpeg-latest-win64-dev.zip', 'ffmpeg-latest-win64-dev')},
                         'extract' : {
                            'dirs' : [
                            ('($pre)/include/', '\\include\\'),
                            ('($pre)/lib/', '\\lib\\')
                            ],
                            'files' : []}
                        }
        installPackage(qtInst, ffmpegDevInfo)
        ffmpegSharedInfo = {'name' : 'FFMpeg library',
                            'https' : True,
                            'url' : "ffmpeg.zeranoe.com",
                            'path': {
                              '32': ('/builds/win32/shared/ffmpeg-latest-win32-shared.zip', 'ffmpeg-latest-win32-shared'),
                              '64': ('/builds/win64/shared/ffmpeg-latest-win64-shared.zip', 'ffmpeg-latest-win64-shared')},
                            'extract' : {
                                'dirs' : [
                                    ('($pre)/bin/', '\\bin\\'),
                                    ('($pre)/presets/', '\\presets\\')],
                                'files' : []}
                            }
        installPackage(qtInst, ffmpegSharedInfo)
        print("FFMpeg has been installed.")
    else:
        print("Done")
    # Check bass installation
    print("Checking bass installation...", end='')
    if not checkFileExist(qtInst, [('bin', 'bass.dll'), ('include', 'bass.h'), ('lib', 'bass.lib')]):
        print("\nFailed to find Bass library, start to install.")
        # Install the Bass library.
        bassInfo = {'name' : 'Bass developing files and library',
                    'https' : False,
                    'url' : 'www.un4seen.com',
                    'path' : {
                     '32': ('/files/bass24.zip', ''),
                     '64': ('/files/bass24.zip', 'x64/')},
                    'extract' : {
                        'dirs':[],
                        'files':[
                            ('c/bass.h', '\\include\\'),
                            ('($pre)bass.dll', '\\bin\\'),
                            ('c/($pre)bass.lib', '\\lib\\')]}
                    }
        installPackage(qtInst, bassInfo)
        print("Bass has been installed.")
    else:
        print("Done")
    # Complete.
    print("Qt", qtInst["version"], "with", qtInst["compiler"], "is ready.\n")

# Function name: displayAllCompiler
# Display all the installation information on the screen
def displayAllCompiler(qtVersions):
    # List the data.
    itemIndex=0
    print(" # Version  Type    Location")
    for itemIndex in range(0, len(qtVersions)):
        # Print the data.
        print(repr(itemIndex+1).rjust(2), \
                qtVersions[itemIndex]["version"].ljust(8), \
                str(qtVersions[itemIndex]["width"])+"-bits", \
                qtVersions[itemIndex]["location"])

def main():
    # Print header.
    print("Kreogist Mu Configuration Tool v1.0")
    print("(C) Kreogist Dev Team 2017\n")
    # Parse parameters.
    if not parseParameter():
        return
    # Find Qt installation.
    print("Searching for Qt installation...", end='')
    qtVersions = findQtInstallation()
    if len(qtVersions) == 0:
        print("Failed")
        print("Please download and install at least one Qt official binary " + \
              "release.")
        return
    print("Done")
    # Check the settings.
    if not compile_all:
        # Check the other settings.
        if len(compile_version) == 0:
            # List all the installation, ask for the configure.
            print("Please select the Qt installation:")
            selectedVersions=[]
            # Show all the item
            displayAllCompiler(qtVersions)
            # Please input the index of the installation.
            compiler_index=input("Please input the index of Qt: ")
            # Check whether the index is the number.
            while not compiler_index.isdigit():
                print("Please input the correct index.")
                compiler_index=input("Please input the index of Qt: ")
            # Check the index is in the range or not.
            if not int(compiler_index) in range(1, len(qtVersions)+1):
                print("Invalid index.")
                return
            # Pick out the data for the list.
            selectedVersions.append(qtVersions[int(compiler_index)-1])
            qtVersions = selectedVersions
        else:
            # Automatically pick out the compiler with the specific name.
            selectedVersions=[]
            for item in qtVersions:
                if compile_version in item["compiler"]:
                    selectedVersions.append(item)
            # Replace the current version.
            qtVersions=selectedVersions
    # Set dependency for all the items in the list.
    for item in qtVersions:
        configureQt(item)
    # Complete.
    print("Configure complete.")

if __name__=="__main__":
    main()
