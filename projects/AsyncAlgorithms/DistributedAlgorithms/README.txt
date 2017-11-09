README.txt
Supratik Singha
SBUID: 108997392
Assignment 3


Ricart Agrawala Timestamp based algorithm and
=============================================
Ricart Agrawala Token based algorithm
=====================================

FILES:
RATokenTimestamp_eff.da

RUN:
This project is based on DistAlgo which is a python module specially designed for distributed algorithms
run directions as given in DistAlgo package
recommended: python3

The layout of the code:
We are trying to simulate
Ricart Agrawala Timestamp based algorithm, Clear and Efficient version and
Ricart Agrawala Token based algorithm, Clear and Efficient version
using DistAlgo (python)

We take a number of processes which we are simulating as different processors executing their
own instance of the algorithm by themselves. and we use message passing between processes to
simulate communication between processors. Hence we are able to achieve the simulation in which
each processor have their own version of the algorithm running and they communicate among themselves
to make sure that they do not get into a critical section more than one processor at a time.

The difference between the clear and efficient versions are that in clear version we are using
set queries and comprehensions for many computations which may turn out to be heavy in some cases
In the efficient versions we are implementing our own logic for complicated computations on sets
and lists so as to optimize performances to some extent. However we see small differences in the
execution of the clear and efficient versions

TESTS:
The code has been tested upto a maximum of 50 processes trying to communicate among themselves
and trying to maintain a critical section



