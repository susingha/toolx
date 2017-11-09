in the LUG site,

there is a file called createdatabases.php that
installs the database. This file assumes that the
database has no passwd..

all other php pages includes a page called connect.php
which must be found in the document_root. connect.php
is responsible for connecting to the database during
user requests and hence provides a single place for
changing the database passwd if needed.

i hope it will work, dun remmber any changes if i made
them recently :)
