#!/bin/bash
#
# Purpose: Shell script to connect a USB connected device via adb over WiFi
#
# Author: Amanshu Raikwar
#
# Assumptions:
#	1. USB debugging is enabled in the Android device
# 	2. The Android device is connected to the computer via USB
# 	3. The Android device is connected to the same wifi as the computer
# 	4. The Android device is accessible through port 5555 over the wifi network
#
# How To Use (without any options):
# 	1. Paste the file where adb resides, for mac it is ~/Library/Android/sdk/platform-tools/
# 	2. Run ./adbwificonnect
# 	3. Choose the device by typing X in [X] --> ... when prompted
# 	4. Wait around 6-7 seconds
# 	5. Connected!
#
# Options:
#	-a
#		To connect to all available devices
#
#	-d device_id
#		Sepcifies the device id to be connected to

# if no command line params provided
if [ -z "$1" ]; then

	clear

	# list available devices
	echo "Available devices:"
	devices="$(./adb devices)"

	export IFS=$'\n'

	declare -a devicemap

	i=0

	# save device info in an array
	for word in $devices; do
		devicemap+=("$(echo $word | awk '{print $1}')")
		echo "[$i] --> $word"
		i=$((i+1))
	done

	# wait for user input to choose device
	read -p "Choose device: "  choice

	# chosen device id
	device=${devicemap[choice]}

	devicename="$(./adb devices -l | grep $device | awk '{print $4}' | cut -d: -f2)"
	devicemodel="$(./adb devices -l | grep $device | awk '{print $5}' | cut -d: -f2)"

	echo "Connecting to device $device $devicename $devicemodel..."

	# restart adb in tcpip mode for the device
	./adb -s $device tcpip 5555

	# wait for adb to restart
	# note: this may change depending on the computer
	sleep 5s

	# find out the wlan0 ip of the device by running ifconfig in device's shell
	ip="$(./adb -s $device shell ip addr show wlan0 | grep 'inet\b' | awk '{print $2}' | cut -d/ -f1)"
	echo "Connecting to $ip..."

	# connect to the device
	./adb connect $ip:5555

# connect to all available devices
elif [ $1 == "-a" ]; then

	# list available devices
	echo "Connecting to all available devices..."
	devices="$(./adb devices)"

	export IFS=$'\n'

	declare -a devicemap

	i=0

	# iterate for each device and connect
	for word in $devices; do

		# device id
		device=("$(echo $word | awk '{print $1}')")

		# skip the first line
		if [ "$i" != "0" ]; then

			echo "---"

			devicename="$(./adb devices -l | grep $device | awk '{print $4}' | cut -d: -f2)"
			devicemodel="$(./adb devices -l | grep $device | awk '{print $5}' | cut -d: -f2)"

			echo "Connecting to device $device $devicename $devicemodel..."

			# restart adb in tcpip mode for the device
			./adb -s $device tcpip 5555

			# wait for adb to restart
			# note: this may change depending on the machine
			sleep 5s

			# find out the wlan ip of the device by running ifconfig in device's shell
			ip="$(./adb -s $device shell ip addr show wlan0 | grep 'inet\b' | awk '{print $2}' | cut -d/ -f1)"
			echo "Connecting to $ip..."

			# connect to the device
			./adb connect $ip:5555

		fi

		i=$((i+1))
	done

# connect to a specific device
elif [ $1 == "-d" ]; then

	if [ -z "$2" ]; then
		echo "-d expects device id as the second parameter"
		exit 1
	fi

	device="$2"

	devicename="$(./adb devices -l | grep $device | awk '{print $4}' | cut -d: -f2)"
	devicemodel="$(./adb devices -l | grep $device | awk '{print $5}' | cut -d: -f2)"

	echo "Connecting to device $device $devicename $devicemodel..."

	# restart adb in tcpip mode for the device
	./adb -s $device tcpip 5555

	# wait for adb to restart
	# note: this may change depending on the machine
	sleep 5s

	# find out the wlan ip of the device by running ifconfig in device's shell
	ip="$(./adb -s $device shell ip addr show wlan0 | grep 'inet\b' | awk '{print $2}' | cut -d/ -f1)"
	echo "Connecting to $ip..."

	# connect to the device
	./adb connect $ip:5555

else
	echo "Flag '$1' is not supported!"
fi
