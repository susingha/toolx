#!/bin/bash

##### Mac #####
l=`ipcs -m | grep "$USER"| cut -f2 -d' '`
for s in $l
do
    echo $s
    ipcrm -m $s 
done

##### Linux #####
l=`ipcs -m | grep "$USER"| cut -f3 -d' '`
for s in $l
do
    echo $s
    ipcrm shm $s 
done

ipcs -m
