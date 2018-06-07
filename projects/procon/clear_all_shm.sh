#!/bin/bash

##### Remove Shared Memory #####
l=`ipcs -m | grep "$USER"| cut -f2 -d' '`
for s in $l
do
    echo $s
    ipcrm -m $s 
done

##### Remove Semaphores #####
l=`ipcs -s | grep "$USER"| cut -f2 -d' '`
for s in $l
do
    echo $s
    ipcrm -s $s 
done

ipcs -a
