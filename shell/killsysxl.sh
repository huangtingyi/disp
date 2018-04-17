#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system killprocess  BEGIN..."

pidof_bin="/usr/sbin/pidof"
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

pids=`$pidof_bin -x sysxlj`

if [ -z "$pids" ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` system is not start..,exit";
	exit 0;
fi

for i in $pids
do
	kill -9 $i
	echo "`date '+%Y/%m/%d %k:%M:%S'` send SIGTERM to $i and it is stopped.."
done

sleep 1;

echo "`date '+%Y/%m/%d %k:%M:%S'` system kill OK..."

exit 0
