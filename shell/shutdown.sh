#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system shutdown  BEGIN..."

pidof_bin="/usr/sbin/pidof"
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

pids=`$pidof_bin -x dat2cli ints_gta replay_gta ints_gta moni.sh`

if [ -z "$pids" ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` system is not start..,exit";
	exit 0;
fi

for i in $pids
do
	kill -SIGTERM $i
	sleep 1
	echo "`date '+%Y/%m/%d %k:%M:%S'` send SIGTERM to $i and it is stopped.."
done

sleep 1;

echo "`date '+%Y/%m/%d %k:%M:%S'` system shutdown  OK..."

exit 0
