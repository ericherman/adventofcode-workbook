#!/bin/bash

EXPECT=$1
shift
PROG=$1
shift

ACTUAL=$( ./$PROG $@ | tail -n1 )
if [ "_${EXPECT}_" != "_${ACTUAL}_" ]; then
	echo "Expected: '$EXPECT'"
	echo " But was: '$ACTUAL'"
	exit 1
fi
