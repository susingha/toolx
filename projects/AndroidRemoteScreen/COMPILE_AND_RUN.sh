#!/bin/bash

# Set the path to your sdk tools folder below
# Also make sure adb is installed and daemon started

PATH_TOOLS=/home/susingha/checkout/adt-bundle-linux-x86_64-20131030/sdk/tools/
LIBPATH=$PATH_TOOLS/lib/

# Compile:
# ========

javac -cp $LIBPATH/ddmlib.jar:$LIBPATH/x86_64/swt.jar AndroidRemoteScreen.java

# jar:
# ====

jar cvfe AndroidRemoteScreen.jar AndroidRemoteScreen *.class

# Run:
# ====

java -Djava.ext.dirs=$LIBPATH/:$LIBPATH/x86_64/ -jar AndroidRemoteScreen.jar

# Cleanup:
# ========

rm *.class
