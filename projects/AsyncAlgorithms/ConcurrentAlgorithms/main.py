#!/usr/bin/python3


import sys
import threading
import time
import config
from driver import run_driver_lamport_fast_thread
from driver import run_config_lamport_fast
from driver import run_driver_lamport_bakery_thread
from driver import run_config_lamport_bakery
from driver import init_config


if len(sys.argv) != 3:
	print ("Usage: python ./main.py <num of threads> <number of requests>")
	exit(len(sys.argv))
init_config(int(sys.argv[1]), int(sys.argv[2]))

print ("Running Lamport's fast mutual exclusion algorithm: Starting %d threads" % (config.num_threads))
print ("=======================================================================")
run_config_lamport_fast()
for thread_num in range(0, config.num_threads):
	t = threading.Thread(target=run_driver_lamport_fast_thread, args=(thread_num,))
	t.daemon = True
	t.start()
	

while config.thread_exits < config.num_threads:
	pass
time.sleep(1)
print ("All threads exited. thread_exits = %d\n\n" % (config.thread_exits))
time.sleep(5)

print ("Running Lamport's bakery algorithm: Starting %d threads" % (config.num_threads))
print ("========================================================")
run_config_lamport_bakery()
for thread_num in range(0, config.num_threads):
	t = threading.Thread(target=run_driver_lamport_bakery_thread, args=(thread_num,))
	t.daemon = True
	t.start()


while config.thread_exits < config.num_threads:
	pass
time.sleep(1)
print ("All threads exited. thread_exits = %d\n\n" % (config.thread_exits))
time.sleep(1)

