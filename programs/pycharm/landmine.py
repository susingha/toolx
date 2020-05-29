#!/usr/bin/python
import os
import sys
import shutil

LOGGERLINE = 'PRINTFLOWCONTEXT(); // sup:gen\n'


if len(sys.argv) < 2:
    print 'Usage', sys.argv[0], '<file to mine>'
    exit (2)

file = sys.argv[1]

def lookahead_curly(f, pos):
    f.seek(pos+1)
    c = ' '
    p = None
    while c.isspace():
        p = f.tell()
        c = f.read(1)

    if c == '{':
        return p
    else:
        return None

def lookback_parenthesis_match(f, pos):
    stack = 0
    p = pos
    while p >= 0:
        f.seek(p)
        c = f.read(1)

        if c == '(':
            stack += 1
        if c == ')':
            stack -= 1
        if stack == 0:
            return p
        p -= 1

    return None

def lookback_hashdefine(f, pos):
    p = pos
    c = None
    while p > 0 and c != os.linesep:
        p -= 1
        f.seek(p)
        c = f.read(1)

    if '#define' in f.readline():
        return True

    return False

def isalnum_(c):
    if c.isalnum() or c == '_':
        return True
    return False

def lookback_word(f, pos):
    p = pos
    c = ' '
    while p > 0 and c.isspace():
        p -= 1
        f.seek(p)
        c = f.read(1)
    if not isalnum_(c):
        return None
    end = p

    while p > 0 and isalnum_(c):
        p -= 1
        f.seek(p)
        c = f.read(1)
    beg = p + 1

    l = end - beg + 1
    f.seek(beg)
    return f.read(l)

def lookback_if(f, pos):
    if lookback_word(f, pos) == 'if':
        return True
    return False

def lookback_for(f, pos):
    if lookback_word(f, pos) == 'for':
        return True
    return False

def lookback_while(f, pos):
    if lookback_word(f, pos) == 'while':
        return True
    return False

def landminefile(filenamesrc, filenamedst):
    filesrc = open(filenamesrc, 'r')
    filedst = open(filenamedst, 'w')
    copybeg = -1
    copyend = -1

    next = 0
    while True:
        filesrc.seek(next)

        post = filesrc.tell()
        char = filesrc.read(1)
        next = filesrc.tell()

        if char == '': # EOF
            break

        # we have char at post

        if char != ')':
            continue

        # we found a ) at post

        postahead = lookahead_curly(filesrc, post)
        if not postahead:
            continue

        # there is a { ahead at postahead

        postback = lookback_parenthesis_match(filesrc, post)

        # there is matching ( at postback

        if lookback_hashdefine(filesrc, postback):
            continue

        # its not a hashdefine

        if lookback_if(filesrc, postback):
            continue
        if lookback_for(filesrc, postback):
            continue
        if lookback_while(filesrc, postback):
            continue

        # its not a if / for / while loop

        # it is a function definition. Copy to postahead
        copybeg = copyend + 1
        copyend = postahead
        copybyt = copyend - copybeg + 1

        filesrc.seek(copybeg)
        for o in xrange(copybyt):
            filedst.write(filesrc.read(1))

        # paste newline, paste log
        filedst.write('\n')
        filedst.write(LOGGERLINE)

    copybeg = copyend + 1
    copyend = filesrc.tell()
    copybyt = copyend - copybeg + 1

    filesrc.seek(copybeg)
    for o in xrange(copybyt):
        filedst.write(filesrc.read(1))

    filedst.close()
    filesrc.close()

    # overwrite file src with the dst

filebak = file+'.bak.c'
filesrc = filebak
filedst = file
shutil.copy(file, filesrc)

landminefile(filesrc, filedst)