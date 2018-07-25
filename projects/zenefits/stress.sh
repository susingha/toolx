#!/bin/bash
while :
do
    echo -n "face" | nc -c 192.168.1.200 5001
done
