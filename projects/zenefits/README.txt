
This is the zenefits take home problem solution.

Functionality:
==============
It implements a google like search bar where in on typing some domain names,
it searches a dictionary backend and lists out all matching domain names as the user types.

The list is populated only when 3 or more characters are input.
If the user can press enter to visit the first result that is populated in the list.
Alternatively, the user can press the down arrow key to move to a different result in the list and press enter to visit the selected domain.
The user can also use the mouse to hover any of the domains in the list and click on it to visit the domain.


Implementation:
===============
In the back end a server is running which reads a list of about 10000 top domian names from a text file (zen_domains.txt)
and organises the domain name strings in a trie (dictionary data structure). (zen_trie.c)
The server creates the trie once and reads the same trie object in the memory to serve multiple requests from the web client (browser).
The server (zen_server.c) listens to new connections on port 5001 using a socket interface
and forks off new child processes to server a dedicated request from the web client.

The HTTP interface is supported using apache web server.
The apache web server uses a cgi-script (domainserver.sh) to ingest the HTTP GET request from the web client and forward it to the socket server.
The web client HTML interface (zenhome.html) is implemented using Javascript, Ajax and HTML and CSS

The server back end can be compiled using ./compile.sh


Future Improvements:
====================
1. Use a UDP socket backend instead of TCP
2. Instead of forking off child at every request, create pthreads to server individual requests


INSTALL:
=======
compile the server backend:
$ ./compile.sh

run the server backend:
$ ./trieserver

enable apache webserver:
------------------------
copy the HTML file to the web server root:
# cp zenhome.html /var/www/html/

enable apache webserver cgi-bin module:
---------------------------------------
copy the cgi script into the cgi folder:
# cp domainserver.sh /usr/lib/cgi-bin

create a softlink:
# ln -s /usr/lib/cgi-bin/domainserver.sh /usr/lib/cgi-bin/domainserver

open the web page:
http://localhost/zenhome.html





