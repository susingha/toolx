#!/usr/bin/python

import random
arr = []
for i in range(10):
	num = random.randint(1,101)
	arr.append(num)
print arr


arrlen = len(arr)
for i in range(arrlen):
	
	min = 999999
	mini = -1
	for j in range(i, arrlen):
		if min >= arr[j]:
			min = arr[j]
			mini = j

	arr[i], arr[mini] = arr[mini], arr[i]
	
print arr


