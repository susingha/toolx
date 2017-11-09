wallongo
========

Hackathon Wall On The Go.

This project is a proof of concept trying to provide online access to fast moving information like:
has the train arrived at the station yet or not.
has the class got cancelled at the last minute or not.
has the concert begun or not.

We may notice that even if we can have access to these information online or on social networks
like twitter or facebook, many people refrain from updating such minute by minute statuses and or
would not reply (or reply later) to these type of queries

We introduce an online poster pinning abstraction on a map (google map in this case) wherein any
person who is at the venue right now and there is a fast moving information for a small amount of time
in which other people who are remotely located may be interested in, may affix a poster or a note
at the very location in that map, which may or may not be notified to other interested people
who are currently say following this venue or geographical area based on topic or friends, etc

- This is a web based application to realize this concept and the application allows us to 
post, delete or update posters on a map / venue and transmit it to friends or followers.
- In the current implementation an person following a location would be able to look for and search
for posters that are available in a radius of 5 miles (say) around a perticular geographical point.
- We also let a person's geographical location be a metric in notifying him / her about posters in
their location

TECHNOLOGY:
HTML, PHP, MYSQL, JAVASCRIPT, AJAX
