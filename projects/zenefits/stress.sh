#!/bin/bash
while :
do
    echo -n "face" | nc -c 127.0.0.1 5001
done
