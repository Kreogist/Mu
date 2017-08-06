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

# Check the dependencies
check_package "qtbase5-dev"
check_package "qttools5-dev-tools"
check_package "ffmpeg"
check_package "libavformat-dev"
check_package "libavcodec-dev"
check_package "libavutil-dev"
check_package "libmpv-dev"
