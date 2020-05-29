#!/bin/bash

echo "Pre-requisite:"
echo "brew install chrome-cli"

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root" 
    exit 1
fi

# Timeout in minutes
tallow=30
tblock=30


# Use this loop for debugging
while false
do

    dt="["`date`"]"
    echo $dt
    cp -v /etc/hosts_unblocked /etc/hosts
    sleep 1
done


while [ : ]
do
    echo "["`date`"]" sleep $tallow
    sleep $(($tallow*60))

    # ./dnsBlockYouTube
    cp -v /etc/hosts_blocked /etc/hosts
    osascript -e 'display notification "BLOCKED"'


    echo "["`date`"]" Kill Chrome YouTube Tabs
    YTTabs=`chrome-cli list tabs | grep YouTube | grep -v "YouTube Music"`
    echo "["`date`"]" $YTTabs | awk 'NR > 1 {print $1}' RS=':' FS=']' | xargs -n1 chrome-cli close -t
    echo "["`date`"]" Killed:
    echo "["`date`"]" $YTTabs

    echo "["`date`"]" sleep $tblock
    sleep $(($tblock*60))

    # ./dnsAllowYouTube
    cp -v /etc/hosts_unblocked /etc/hosts
    # ascript -e 'display notification "ALLOWED"'

done
