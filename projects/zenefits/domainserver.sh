#!/bin/bash
printf "Content-type: text/html\n\n"

query=$1

#printf "<HTML>\n"
#printf "<BODY>\n"
#printf "<pre>\n"

for p in ${QUERY_STRING//&/ }; do
    kvp=( ${p/=/ })
    k=${kvp[0]}
    v=${kvp[1]}
    eval $k=$v
done

if [ -z "$query" ]; then
    query=$1
fi

if [ -z "$query" ]; then
    query="abc"
fi

echo -n "$query" | nc -C 127.0.0.1 5001

#printf "</pre>\n\n"
#printf "</BODY>\n"
#printf "</HTML>\n"
