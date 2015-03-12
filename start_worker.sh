#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "must provide port"
	exit
fi

while [ 1 ]; do
    echo "(re)starting curl-queue worker" | logger
    build/worker $1
done
