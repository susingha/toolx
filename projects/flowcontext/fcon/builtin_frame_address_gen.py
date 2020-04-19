#!/usr/bin/python

for i in xrange(100):
    print "case ",i,": return __builtin_frame_address(",i,");"
