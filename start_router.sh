#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "must provide client and worker ports"
	exit
fi

while [ 1 ]; do
    echo "(re)starting curl-queue router" | logger
    build/tcprouter $1 $2
    killall worker
done
