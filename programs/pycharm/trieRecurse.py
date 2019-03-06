
trie = {'quick', 'brown', 'the', 'fox'}
arr = "thequickbrownfox"
trie = {'the', 'theremin'}
arr = "theremin"
trie = {'bed', 'bath', 'bedbath', 'and', 'beyond'}
arr = "bedbathandbeyond"

lenarr = len(arr)
wlist = []
wlistlen = 0


def wordlist(word, leveli):
    global wlistlen
    if leveli < wlistlen:
        wlist[leveli] = word
    else:
        wlist.append(word)
        wlistlen += 1


def lookup(si, leveli):

    if si == lenarr:
        print wlist[:leveli]
        return
    if si > lenarr:
        return

    for i in xrange(si, lenarr):
        word = arr[si:i+1]

        if word in trie:
            wordlist(word, leveli)
            lookup(i+1, leveli+1)


# lookup(0, 0)


def find_sentence(s, dictionary):
    starts = {0: ''}
    for i in range(len(s) + 1):
        new_starts = starts.copy()
        for start_index, _ in starts.items():
            word = s[start_index:i]
            if word in dictionary:
                new_starts[i] = word
        starts = new_starts.copy()
        print starts

    result = []
    current_length = len(s)
    if current_length not in starts:
        return None
    while current_length > 0:
        word = starts[current_length]
        current_length -= len(word)
        result.append(word)

    return list(reversed(result))

print find_sentence(arr, trie)







