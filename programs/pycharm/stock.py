
"""
Stock buy sell multiple times with transaction charge

fn(i, have)

if i is last:
    if bought:
        have to sell
    else:
        pass
else:
    if bought:
        choices: sell or pass
    else:
        choices: pass or buy

"""

arr = [1, 3, 2, 8, 4, 10]
arr = [1, 3, 2, 8, 4, 10, 2, 4, 6, 2, 4, 6, 1, 9, 5, 3]
lenarr = len(arr)
fee = 1


def fnrec(i, have):

    if i == lenarr-1:
        if have:
            # sell
            return arr[i] - fee
        else:
            # pass
            return 0

    if have:
        # sell
        p1 = fnrec(i+1, False) + arr[i] - fee
        # pass
        p2 = fnrec(i+1, True)
    else:
        # buy
        p1 = fnrec(i+1, True) - arr[i] - fee
        # pass
        p2 = fnrec(i+1, False)

    return max(p1, p2)

def fnitr():
    haveT = 0
    haveF = 0
    for i in range(lenarr)[::-1]:
        if i == lenarr - 1:
            haveT = arr[i] - fee
            haveF = 0

        else:
            haveT_tmp = max(haveF + arr[i] - fee, haveT)
            haveF_tmp = max(haveT - arr[i] - fee, haveF)
            haveT = haveT_tmp
            haveF = haveF_tmp

    return haveF

print fnrec(0, False)
print fnitr()