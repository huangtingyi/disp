#!/bin/bash

if [ $# -ne 4 ];then
	echo "Usage $0 cmdfile cmdres startcmd stopcmd"
	exit 1
fi

cmdfile=$1
cmdres=$2
startcmd=$3
stopcmd=$4

cmd=""

while [ 1 ];
do
	cmd=`head -1 $cmdfile | awk '{print $1}'`

	cmd=${cmd:-"nocmd"}

	echo "`date '+%Y/%m/%d %k:%M:%S'` cmd=$cmd "

	if [ $cmd = "start" ];then
		datapath=`head -1 $cmdfile | awk '{print $2}'`
		datadate=`head -1 $cmdfile | awk '{print $3}'`
		starttime=`head -1 $cmdfile | awk '{print $4}'`
		starttime=${starttime:-091500}

		$startcmd $datapath $datadate $starttime > $cmdres
		echo "started" > $cmdfile

		echo "$startcmd $datapath $datadate $starttime"
		cat $cmdres
	elif [ $cmd = "stop" ];then
		$stopcmd > $cmdres
		echo "stopped" > $cmdfile

		echo $stopcmd
		cat $cmdres
	fi

	sleep 5
done
