#!/bin/bash

echo "Pre-requisite:"
echo "brew install chrome-cli"

tallow=30
tblock=10

while [ : ]
do
    echo "sleep $tallow"
    sleep $(($tallow*60))

    # ./dnsBlockYouTube

    echo "Kill Chrome YouTube Tabs"
    YTTabs=`chrome-cli list tabs | grep YouTube`
    echo $YTTabs | awk 'NR > 1 {print $1}' RS=':' FS=']' | xargs -n1 chrome-cli close -t
    echo "Killed:"
    echo "$YTTabs"

    echo "sleep $tblock"
    sleep $(($tblock*60))

    # ./dnsAllowYouTube

done
