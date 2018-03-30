#!/bin/bash

pids=`pidof -x dat2cli gta_ints moni.sh`

if [ -z "$pids" ]; then
	echo "system is not start..,exit";
	exit 0;
fi

for i in $pids
do
	kill -SIGTERM $i
	sleep 1
	echo "send SIGTERM to $i and it is stopped.."
done

sleep 1;

echo "system shutdown  OK..."

exit 0
