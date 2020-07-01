#!/bin/bash

echo "installing git"
 apt-get install -y git
echo "installing vim"
 apt-get install -y vim
echo "installing screen"
 apt-get install -y screen
echo "installing sshpass"
 apt-get install -y sshpass

echo "skipping apache2"
#apt-get install -y apache2
echo "skipping openssh-server"
#apt-get install -y openssh-server
echo "skipping chrome-cli"
#apt-get install -y chrome-cli # <- mac




