from pytrie import StringTrie 
  
def insert(trie, string):
    trie[string] = string

def lookup(trie, prefix):
    return trie.values(prefix)


  
trie=StringTrie() 

insert(trie, "google")
insert(trie, "googledoc")
insert(trie, "googlemaps")
insert(trie, "googlicious")

print "matching goo"
print lookup(trie, "goo")

print "matching google"
print lookup(trie, "goo")



