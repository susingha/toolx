

str = "6 + ((5 - 2) + 7  + (2/1))"
strlen = len(str)



def atoi(c):
    if ord('0') <= ord(c) and ord(c) <= ord('9'):
        return True, ord(c) - ord('0')
    else:
        return False, 0


def getnum(idx):
    val = 0
    for i in xrange(idx, strlen):
        numerical, num = atoi(str[i])
        if numerical:
            val = (val*10) + num
        else:
            return val, i-1

    return val, i


def calc (idx):
    sign = None
    result = None

    i = idx-1
    while i < strlen-1:
        i += 1

        if str[i] == '+' or str[i] == '-':
            sign = str[i]
            if result == None:
                result = 0
            continue
        elif str[i] == '*' or str[i] == '/':
            sign = str[i]
            continue
        elif str[i] == ' ':
            continue
        elif str[i] == ')':
            return result, i
        elif str[i] == '(':
            num, i = calc(i+1)
        else:
            num, i = getnum(i)

        if sign == None:
            result = num
        elif sign == '+':
            result += num
        elif sign == '-':
            result -= num
        elif sign == '*':
            result *= num
        elif sign == '/':
            result /= num

    return result, i-1


result, idx = calc(0)
print result
