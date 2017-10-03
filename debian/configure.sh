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
		sudo apt-get -y install "$1"
		echo "[I] Done: $1 has been installed."
	else
		echo "Done"
	fi
}

function package_ffmpeg_mpv()
{
	check_package "ffmpeg"
	check_package "libmpv-dev"
	check_package "libswresample-dev"
}

# Check the dependencies
check_package "build-essential"
check_package "qtbase5-dev"
check_package "qttools5-dev-tools"
check_package "libavformat-dev"
check_package "libavcodec-dev"
check_package "libavutil-dev"
check_package "libswscale-dev"
# Check Ubuntu version
ubuntu_version=$(cat /etc/issue | grep "Ubuntu")
if [ "$ubuntu_version" != "" ]
then
	echo "[S] Ubuntu distribution detected."
	ubuntu_version=${ubuntu_version#*Ubuntu }
	ubuntu_version=${ubuntu_version%%.*}
	if [ "$ubuntu_version" -gt 13 ] && [ "$ubuntu_version" -lt 16 ]
	then
		# For Ubuntu 14.04 and its later but eariler than 16.04,
		# Use LibAV instead of FFMpeg
		check_package "libavresample-dev"
		# Also, it doesn't have libmpv-dev, use gstreamer.
		check_package "libgstreamer1.0-dev"
	else
		# FFMpeg's libswresample.
		package_ffmpeg_mpv
	fi
else
	# For other debian distributes, use FFMpeg as default
	package_ffmpeg_mpv
fi
