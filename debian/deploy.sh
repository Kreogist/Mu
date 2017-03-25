#!/bin/bash

# Function: check_package
# Call: check_package "Package Name"
# Check whether the package is in the apt packaging system.
function check_package()
{
	echo -n "[F] Checking $1 package..."
	check_package=$(dpkg -l | grep "ii  $1")
	if [ "$check_package" == "" ]
	then
		echo ""
		echo "[S] Installing $1..."
		sudo apt-get install "$1"
		echo "[I] Done: $1 has been installed."
	else
		echo "Done"
	fi
}

# Check devscripts
check_package "devscripts"
# Launch debuild
cd ..
debuild -us -uc
