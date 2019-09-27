"""
Snakes and Ladders is a game played on a 10 x 10 board,
the goal of which is get from square 1 to square 100.
On each turn players will roll a six-sided die
and move forward a number of spaces equal to the result.
If they land on a square that represents a snake or ladder,
they will be transported ahead or behind,
respectively, to a new square.

Find the smallest number of turns it takes
to play snakes and ladders.

For convenience, here are the squares
representing snakes and ladders, and their outcomes:

snakes = {16: 6, 48: 26, 49: 11, 56: 53, 62: 19, 64: 60, 87: 24, 93: 73, 95: 75, 98: 78}
ladders = {1: 38, 4: 14, 9: 31, 21: 42, 28: 84, 36: 44, 51: 67, 71: 91, 80: 100}
"""


tunnels = {16: 6, 48: 26, 49: 11, 56: 53, 62: 19, 64: 60, 87: 24, 93: 73, 95: 75, 98: 78, 1: 38, 4: 14, 9: 31, 21: 42, 28: 84, 36: 44, 51: 67, 71: 91, 80: 100}

tunnels_rev = {}
for s in tunnels:
    d = tunnels[s]
    tunnels_rev[d] = s

cells = 100
trycache = [None]*(cells + 1) # will have cells 1 to 100

trycache[1] = 1
for n in xrange(1, cells + 1):

    # mark the tunnel destinations, may be loop
    trycount = trycache[n]
    s = n
    while s in tunnels:
        d = tunnels[s]
        # if the d has been reached before with a lower count, exit
        if trycache[d] != None and trycache[d] <= trycount:
            break
        # if the d has not been reached before or reached with a higher count,
        # update and continue to next destination
        if trycache[d] == None or trycache[d] > trycount:
            trycache[d] = trycount
            s = d

    # mark the next hops
    for i in range(6):
        if n+i <= 100 and (trycache[n+i] == None or trycache[n+i] > trycount + 1):
            trycache[n+i] = trycount + 1

print trycache[100]