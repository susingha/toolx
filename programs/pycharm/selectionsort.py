#!/usr/bin/python

import random

def swapindex(array, index1, index2):
	temp = array[index1]
	array[index1] = array[index2]
	array[index2] = temp

def printarray(array, total):
	for i in range(0, total - 1):
		print "[%d], " % (arr[i]),
	print ""

total = 10
arr = []

for i in range(0, total - 1):
	num = random.randint(1,101)
	arr.append(num)

printarray(arr, total)

for ins in range(0, total - 1):
	min = arr[ins]
	minindex = ins
	for j in range(ins + 1, total - 1):
		if arr[j] < min:
			min = arr[j]
			minindex = j
	swapindex(arr, ins, minindex)


printarray(arr, total)

	

