README.txt
Supratik Singha
SBUID: 108997392
Assignment 2


Lamport's fast mutual exclusion algorithm and
=============================================
Lamport's bakery algorithm
==========================

FILES:
The code is spread across multiple files as follows:
main.py		contains the startup code. starts all the threads for each algorithm
driver.py	contains the initialization code, core algorithm code for each algorithm
config.py	contains the global variables shared between the threads and modules
README.txt	this file

RUN:
python.exe main.py <number of threads> <number of requests>
check sample_output.txt for a sample run and outputs

OUTPUTS:
The outputs in the program may come interleaved. hence automated grepping may not succeed in
generating the correct results. Please handle appropriately while evaluating. This is true
especially for the prints outside the critical section because they are not protected and may
run simultaneously by threads

The Layout of the code:
we are trying to simulate
Lamport's fast mutual exclusion algorithm and
Lamport's bakery algorithm
using python scripts.

We take the number of threads and the number of requests from the user as command line arguments.
Each thread is supposed to pick a request and serve it by executing through the Critical Section.
We have randomly assigned the requests among each of the threads such that each thread serves a
subset of the total number of requests by going through a CS again and again till it's quota gets
finished. For example, if there are 2 threads and 4 requests, there is a possibility that thread 0
will serve 3 requests and thread 1 will serve 1 request.

The thread numbers and request number start from 0 (and not 1).
All threads sleep for a seconds in the beginning right after starting up so that all threads come
to sync and are ready to enter the contending procedure.
We first serve the requests through the CS by running Lamport's fast mutual exclusion algorithm.
we wait till all the threads exits and we print a message when the simulation ends.
Then we again serve the same quota of requests through the CS by running Lamport's bakery algorithm
and wait till all the threads exits and print a message when the simulation ends.

Random sleeps can be enabled at strategic places to improve the distribution of threads and their
running times. This may drastically increase the running time of the simulation when issuing large 
number of threads and requests.


TESTS:
The code have been tested with a maximum of upto 500 threads and 500 requests in various combinations
All development and tests have been performed on Linux
A sample output has been included in the tarball

NOTES:
in Lamport's fast mutual exclusion algorithm, 0 is a valid thread number so variable y and x is being
initialized to -1 in the beginning and being reset to -1 while leaving the CS.



