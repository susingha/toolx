#!/bin/bash

echo "Pre-requisite:"
echo "brew install chrome-cli"

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root" 
    exit 1
fi

# Timeout in minutes
tallow=60
tblock=15


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
    # ./dnsAllowYouTube
    cp -v /etc/hosts_unblocked /etc/hosts
    # ascript -e 'display notification "ALLOWED"'
    echo "["`date`"]" sleep $tallow
    sleep $(($tallow*60))

    # ./dnsBlockYouTube
    cp -v /etc/hosts_blocked /etc/hosts
    osascript -e 'display notification "BLOCKED"'


    echo "["`date`"]" Kill Chrome YouTube Tabs
    YTTabs=`chrome-cli list tabs | grep YouTube | grep -v "YouTube Music"`
    echo $YTTabs | awk 'NR > 1 {print $1}' RS=':' FS=']' # | xargs -n1 chrome-cli close -t
    echo "["`date`"]" Killed:
    echo "["`date`"]" $YTTabs

    echo "["`date`"]" sleep $tblock
    tremain=$((tblock*60))
    ttravel=5
    echo
    while [ $tremain -gt 0 ]
    do
        echo -ne "\rYouTube back in $((tremain/60)) minutes $((tremain%60)) seconds  "
        sleep $ttravel
        tremain=$((tremain-ttravel))
    done
    echo

done
