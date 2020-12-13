#!/usr/bin/python
import os
import time
import datetime
import osascript

print "Pre-requisite:"
print "brew install chrome-cli"
print "pip install osascript"

print "script does not work. under construction. ref: unYouTube-cli.sh"
exit(0)

if os.geteuid() != 0:
    print "This script must be run as root"
    exit(1)


# Timeout in minutes
tallow=30
tblock=30


# Use this loop for debugging
while True:
    print datetime.datetime.now()
    os.system('cp -v /etc/hosts_unblocked /etc/hosts')
    osascript.run('display notification "BLOCKED"')
    time.sleep(10)

while False:
    stream = os.popen('chrome-cli list tabs')
    output = stream.read()
    print output
    print os.geteuid()
    time.sleep(1)


while True:
    print datetime.datetime.now(), "sleep", tallow
    time.sleep(tallow*60)

    # ./dnsBlockYouube
    os.system('cp -v /etc/hosts_blocked /etc/hosts')
    osascript.run('display notification "BLOCKED"')

    print datetime.datetime.now(), "Kill Chrome YouTube Tabs"
    stream=os.popen('chrome-cli list tabs | grep YouTube | grep -v "YouTube Music"')
    yttabs = stream.read()

