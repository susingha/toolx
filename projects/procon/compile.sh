#!/bin/bash

gcc sharedmem.c shbuf.c producer.c -o a.out
gcc sharedmem.c shbuf.c consumer.c -o b.out
