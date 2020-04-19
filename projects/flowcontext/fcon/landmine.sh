#!/bin/bash


find . -name "*.c" > fcon/_fcon_filelist
cat fcon/_fcon_filelist | xargs p4 edit
cat fcon/_fcon_filelist | xargs touch 
cat fcon/_fcon_filelist | xargs -n1 fcon/landmine.py


# revert
# cat fcon/_fcon_filelist | xargs p4 revert
# find  . -name "_fcon_bak_*" | xargs rm
