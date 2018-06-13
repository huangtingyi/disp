#!/bin/bash

if [ $# -ne 4 ];then
	echo "Usage $0 cmdfile cmdres startcmd stopcmd"
	exit 1
fi

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

cmdfile=$1
cmdres=$2
startcmd=$3
stopcmd=$4

replaydir=${replaydir:-"/data/work"}

cmd=""

while [ 1 ];
do
	cmd=`head -1 $cmdfile | awk '{print $1}'`

	cmd=${cmd:-"nocmd"}

	echo "`date '+%Y/%m/%d %k:%M:%S'` cmd=$cmd "

	if [ $cmd = "start" ];then
#		datapath=`head -1 $cmdfile | awk '{print $2}'`
		datadate=`head -1 $cmdfile | awk '{print $3}'`
		starttime=`head -1 $cmdfile | awk '{print $4}'`
		starttime=${starttime:-091500}

		$startcmd $replaydir $datadate $starttime > $cmdres
		echo "started" > $cmdfile

		echo "$startcmd $replaydir $datadate $starttime"
		cat $cmdres
	elif [ $cmd = "stop" ];then
		$stopcmd > $cmdres
		echo "stopped" > $cmdfile

		echo $stopcmd
		cat $cmdres
	fi

	sleep 5
done
