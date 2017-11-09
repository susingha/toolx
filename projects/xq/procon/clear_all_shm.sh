#!/bin/bash

l=`ipcs -m | grep "$USER"| cut -c12-19`
for s in $l
do
    echo $s
    ipcrm shm $s 
done
ipcs -m
