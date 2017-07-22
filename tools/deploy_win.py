# -*- coding: utf-8 -*-
from util_win import *
import subprocess
import sys
import os

compilerName=""
buildPath=""
deployPath=""

targetQt={}

# Function name: parseParameter
# Check all the parameter passed from command line.
def parseParameter():
    # Get the global data.
    global compilerName
    global buildPath
    global deployPath
    # Get the configure list.
    configureList=list(sys.argv[1:]);
    # Pop until the list is empty.
    while len(configureList) > 0:
        # Get the first item in the list.
        parameter = configureList[0]
        configureList = configureList[1:]
        # Check the parameter.
        if parameter == "-c" or parameter == "--compiler":
            # Get the deploy compiler.
            compilerName = configureList[0]
        elif parameter == "-b" or parameter == "--build":
            # Get the compiler path.
            buildPath = configureList[0]
        elif parameter == "-d" or parameter == "--deploy":
            # Save the deploy path.
            deployPath = configureList[0]
        else:
            # Unknown parameter.
            print("Error: Unknown parameter", parameter)
            return False
        # Remove the configure list.
        configureList = configureList[1:]
    # Mission complete.
    return True

def checkSetting(qtVersions):
    # Get the global compiler
    global compilerName
    global targetQt
    # Check the configure data.
    if compilerName == "":
        print("All the Qt installation(s):")
        displayAllCompiler(qtVersions)
        compilerIndex=""
        while not compilerIndex.isdigit():
            compilerIndex=input("Please set the compiler (1-"+ str(len(qtVersions)) +"): ")
        # Get the compiler index.
        compilerIndex=int(compilerIndex)
        if not compilerIndex in range(1, len(qtVersions)+1):
            # Failed.
            print("Error: Invalid index.")
            return False
        # Get the compiler index.
        compilerName=qtVersions[compilerIndex-1]["compiler"]
    # Get the qt installation.
    for qtInst in qtVersions:
        # Check the compiler.
        if compilerName == qtInst["compiler"]:
            # Save the installation data.
            targetQt=qtInst
            break
    # Complete.
    return True

def buildProject():
    global targetQt
    # Prepare the specification.
    compilerSpec={'msvc2015_64': 'win32_msvc2015'}
    # Prepare qmake parameter.
    qtBinPath=targetQt["root"]+"\\bin\\";
    qmakePath=qtBinPath+"qmake.exe"
    projectDirPath=os.path.abspath('..')+"\\"
    projectPath=projectDirPath+"mu.pro"
    # Execute qmake.
    # Set the QTDIR data.
    os.environ['QTDIR']=targetQt["root"]
    # os.environ['QMAKESPEC']=targetQt["root"]+"\\mkspecs\\"+compilerSpec[targetQt["compiler"]]
    # Execute the qmake for the specific target.
    os.chdir(qtBinPath)
    os.system("qmake.exe "+projectPath+" -spec "+compilerSpec[targetQt["compiler"]])
    print("Finish")

def main():
    # Print header.
    print("Kreogist Mu Deployment Tool v1.0")
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
    # Check parameter.
    if not checkSetting(qtVersions):
        return
    # Build the project.
    buildProject()

if __name__=="__main__":
    main()
