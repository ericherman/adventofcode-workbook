#!/bin/bash

EXPECT=$1
shift
PROG=$1
shift

START=$( date '+%s.%N' )

ACTUAL=$( ./$PROG $@ | tail -n1 )

END=$( date '+%s.%N' )


if [ "_${EXPECT}_" != "_${ACTUAL}_" ]; then
	echo "$PROG"
	echo "Expected: '$EXPECT'"
	echo " But was: '$ACTUAL'"
	EXIT_CODE=1
else
	EXIT_CODE=0;
fi

DURATION=$( echo "$END - $START" | bc )
THRESHOLD=1.0
if (( $(echo "$DURATION > $THRESHOLD" | bc -l) )); then
	echo "$PROG execution time: $DURATION"
fi

exit $EXIT_CODE
