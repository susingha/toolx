#!/bin/bash

gcc sharedmem.c sh_sync.c producer.c -o a.out
gcc sharedmem.c sh_sync.c consumer.c -o b.out
