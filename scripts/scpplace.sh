#!/bin/bash

image=$1

echo placing image: $image
cp -v $image temp
cp -v        temp /tftpboot/image.img
md5sum /tftpboot/image.img

