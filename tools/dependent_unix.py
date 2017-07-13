import os
import sys
import platform

# Main function.
def main():
    # Print header.
    print("Kreogist Mu Configuration Tool v1.0")
    print("(C) Kreogist Dev Team 2017\n")
    # Check operating system version.
    if sys.platform.lower()=="darwin":
        print("Operating System: Mac OS X")
        # Check the python version.
        python_version=platform.python_version()[0:1]
        if python_version=="3":
            # Already use python 3 for default python version.
            os.system("python ./Tools/dependent_mac.py")
        else:
            # Or else, we need to force use python 3.
            os.system("python3 ./Tools/dependent_mac.py")

if __name__ == "__main__":
    # execute only if run as a script
    main()
