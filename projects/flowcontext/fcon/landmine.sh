#!/bin/bash


ls       *.c *.h **/*.c **/*.h > _fcon_filelist
p4 edit  *.c *.h **/*.c **/*.h
touch    *.c *.h **/*.c **/*.h
cat _fcon_filelist | xargs -n1 landmine.py


# revert
# cat fcon/_fcon_filelist | xargs p4 revert
# find  . -name "_fcon_bak_*" | xargs rm
