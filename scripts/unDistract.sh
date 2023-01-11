#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root" 
    exit 1
fi


while [ : ]
do
    cp -v /etc/hosts_play /etc/hosts
    osascript -e 'display notification "Play"'
    read -p "Current: PLAY. Press enter to toggle"
    # echo "Current: PLAY. 1 Hour"
    # sleep 3600


    cp -v /etc/hosts_work /etc/hosts
    osascript -e 'display notification "Work"'
    read -p "Current: WORK. Press enter to toggle"

done


