
"""
Given a histogram. Find the volume of water it can hold
   
                                     __ 
                                    |24|
             14 10 18 05 03 12 14 08|  |
          __   _  _  _  _  _  _  _  |  |
         |21|                       |  |
         |  |                       |  |
         |  |            __         |  |
         |  |           |18|        |  |
         |  |         __|  |        |  |__ 03 07 12 10
         |  |        |16|  |        |  |16|
         |  |        |  |  |        |  |  |  _  _  _   __
         |  |        |  |  |      __|  |  |           |14|
         |  |        |  |  |     |13|  |  |           |  |
       __|  |   __   |  |  |     |  |  |  |__         |  |
      |11|  |  |11|  |  |  |     |  |  |  |11|        |  |
      |  |  |  |  |  |  |  |__   |  |  |  |  |        |  |__ 
      |  |  |  |  |  |  |  |09|  |  |  |  |  |        |  |09|
    __|  |  |__|  |  |  |  |  |__|  |  |  |  |__      |  |  |
   |07|  |  |07|  |  |  |  |  |07|  |  |  |  |07|     |  |  |
   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |     |  |  |
   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |   __|  |  |__ 
   |  |  |  |  |  |__|  |  |  |  |  |  |  |  |  |  |04|  |  |04|
   |  |  |  |  |  |03|  |  |  |  |  |  |  |  |  |__|  |  |  |  |
   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |02|  |  |  |  |
   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    07 11 21 07 11 03 16 18 09 07 13 24 16 11 07 02 04 14 09 04

   Total: 116

this is O(n) time and O(1) space solution
histogramFill.c has a O(n) time and O(n) space inferior solution
"""


bars = [7, 11, 21,  7, 11, 3, 16, 18, 9, 7, 13, 24, 16, 11,  7, 2,  4, 14, 9, 4];
lenbars = len(bars)

totalfill = 0

prevmaxht = 0
prevht = 0
tmpfill = 0
for ht in bars:
    if ht < prevmaxht:
        tmpfill += (prevmaxht - ht)
    else:
        totalfill += tmpfill
        tmpfill = 0
        prevmaxht = ht


prevmaxht = 0
prevht = 0
tmpfill = 0
for ht in bars[::-1]:
    if ht < prevmaxht:
        tmpfill += (prevmaxht - ht)
    else:
        totalfill += tmpfill
        tmpfill = 0
        prevmaxht = ht


print totalfill


