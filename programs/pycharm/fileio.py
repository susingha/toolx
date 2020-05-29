
f = open('fileio.txt', 'r')

for i in range(15):
    print f.tell(),
    c = f.read(1)
    if c == '':
        print 'not'
    else:
        print c

for i in range(15):
    print f.seek(i), f.tell(),
    c = f.read(1)
    if c == '':
        print 'not'
    else:
        print c

f.close()

t = ''
print t.isspace()
t = '\t\r\n'
print t.isspace()
t = 'a\t\r\n'
print t.isspace()

print '_'.isalnum()
print 'a'.isalnum()
print 'A'.isalnum()
print '0'.isalnum()
print '\n'.isspace()

def fun():
    ret = 'if'
    return ret
abc = 'if'
if abc is fun():
    print 'same'
else:
    print 'diff'