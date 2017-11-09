#!/usr/bin/python3

import time
import config
from random import randint

allotment = []
def init_config(threads, requests):
	global allotment
	config.num_threads = threads
	config.num_requests = requests
	print ("Number of threads = %d" % (config.num_threads))
	print ("Number of requests = %d\n" % (config.num_requests))

	for counter in range(0, config.num_threads):
		allotment.append(0)
	for counter in range(0, config.num_requests):
		allotment[randint(0, config.num_threads-1)] += 1

	print ("init_config done")


########### Lamport's fast mutual exclusion algorithm #####################
y = -1
x = -1
b = []
requestno = 0

def run_config_lamport_fast():
	global y
	global x
	global b
	global requestno
	y = -1
	x = -1
	for counter in range(0, config.num_threads):
		b.append(0)
	requestno = 0
	config.thread_exits = 0

def run_driver_lamport_fast_thread(num):
	global allotment
	global y
	global x
	global b
	global requestno
	pending = allotment[num]
    	# print ("\trunning thread: %d" % (num))
	time.sleep(1)

	while pending:

		########## Entry Section ############
		print ("\t\t%d requesting CS" % (num))
		b[num] = 1
		x = num
		if y != -1:
			b[num] = 0
			while (y != -1):
				pass
			continue
		y = num

		if x != num:
			b[num] = 0
			for j in range(0, config.num_threads):
				while ( b[j] != 0 ):
					pass
			if y != num:
				while (y != -1):
					pass
				continue

		########## Critical Section #########
		requestno = requestno + 1
		print ("\t\t%d entering CS" % (num))
		pending = pending - 1
		# time.sleep(randint(1,3))

		########## Exit Section #############
		print ("\t\t%d exiting CS" % (num))
		y = -1
		b[num] = 0

		#####################################


	config.thread_exits += 1
	# print ("\texiting thread: %d. thread_exits = %d" % (num, config.thread_exits))


########## Lamport's bakery algorithm #####################################
choosing = []
tokennum = []

def run_config_lamport_bakery():
	global requestno
	global choosing
	global tokennum
	for counter in range(0, config.num_threads):
		choosing.append(0)
		tokennum.append(0)
	requestno = 0
	config.thread_exits = 0

def run_driver_lamport_bakery_thread(num):
	global allotment
	global requestno
	global choosing
	global tokennum
	pending = allotment[num]
	# print ("running thread: %d" % (num))
	time.sleep(1)

	while pending:

		########## Entry Section ############
		print ("\t\t%d requesting CS" % (num))
		choosing[num] = 1
		tokennum[num] = 1 + max(tokennum)
		print ("\t\t%d received Token %d" % (num,tokennum[num]))
		choosing[num] = 0
		for j in range(0, config.num_threads):
			while(choosing[j] != 0):
				pass
			while((tokennum[j] !=0) & ((tokennum[j] < tokennum[num]) | ((tokennum[j] == tokennum[num]) & (j < num)))):
				pass

		########## Critical Section #########
		requestno = requestno + 1
		print ("\t\t%d entering CS" % (num))
		pending = pending - 1
		# time.sleep(randint(1,3))

		########## Exit Section #############
		print ("\t\t%d exiting CS" % (num))
		tokennum[num] = 0

		#####################################


	config.thread_exits += 1
	# print ("\texiting thread: %d. thread_exits = %d" % (num, config.thread_exits))


