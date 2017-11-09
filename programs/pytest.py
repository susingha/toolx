#!/usr/bin/python

import sys
import thread
import time
from random import randint

while 1:
	print("here. random = %d" % (randint(2,9)))


var = 0


pvals = set()
pvals.add((1, 21))
pvals.add((2, 32))
pvals.add((3, 43))
pvals.add((4, 84))
pvals.add((5, 65))
pvals.add((6, 26))
pvals.add((7, 57))
a,b = pvals.pop()
print a
print b
slot = [s1 for (s1, b1) in pvals if b1 == max(b for (s, b) in pvals)]
print slot[0]


n = 20
b = range(n)
for i in range(0,n):
    	print "b[%d] contains %d" % (i, b[i])




# Define a function for the thread
def print_time( threadName, delay):
   global var

###################### entry ####################
   var = var + 1
###################### exit #####################

   count = 0
   while count < 5:
#      time.sleep(delay)
      count += 1
      print "%s: %s" % ( threadName, time.ctime(time.time()) )

# Create two threads as follows
try:
   thread.start_new_thread( print_time, ("Thread-1", 2, ) )
   thread.start_new_thread( print_time, ("Thread-2", 4, ) )
except:
   print "Error: unable to start thread"
time.sleep(3)
while 1:
   pass



# if False:
print "this is in python"
i = 98
print (i)

arr = "not my way"
print (arr)

print 'This {food} is {adjective}.'.format(food='spam', adjective='absolutely horrible')
print 'new way to {a} print {b}'.format(a=arr, b=i)

for item in range(1,5):
	print item

for x in xrange(0, 3):
	print "We're on time %d %d and %s\n" % (x, i, arr)
    	i -= 1
	if x < 0:
	     x = 0
	     print 'Negative changed to zero'
	elif x == 0:
	     print 'Zero'
	elif x == 1:
	     print 'Single'
	else:
	     print 'More'


words = ['cat', 'window', 'defenestrate']
for w in words:
	print w, len(w)




