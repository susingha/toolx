#!/bin/bash

users="susingha"
hosts="134.141.50.33"
files="/home/susingha/Perforce/susingha_0114_Nantucket/build_AP250/images/AP250-10-0-9.img"
files="/home/susingha/Perforce/susingha_0418_Nantucket/build_AP250/images/AP250-10-0-9.img"
files="/tftpboot/image.img"

userd="cisco"
hostd="192.168.0.16"
filed="/tftpboot/image.img"


while [ : ]
do
    prevsum=$currsum
    currsum=`ssh $users@$hosts "md5sum $files"`
    if [[ $currsum == $prevsum ]]
    then
    	echo same image: $currsum
    else
    	echo diff image: $currsum. previous: $prevsum
	echo "copying SRC:" "$hosts:$files"
	echo "copying DST:" "$hostd:$filed"

	echo "save image tftp://192.168.0.16:image.img now"

	ssh $users@$hosts "md5sum $files"
	scp -3 $users@$hosts:$files  $userd@$hostd:$filed
	ssh $userd@$hostd "md5sum $filed"
    fi

    sleep 5
done

