#!/bin/bash

pidof_bin="/usr/sbin/pidof"

[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

export TERM=${TERM:-xterm}

ethn=$1

while true
do
	##09点15分之前，15点15分之后，11点45分到12点45分布监控
	my_date=`date "+%k%M%S"`
	if [ $my_date -gt "151500" ] ||
		[ "$my_date" -lt "090000" ] ||
		[ "$my_date" -gt "114500" -a "$my_date" -lt "124500" ]; then
		echo -e "`date '+%Y/%m/%d %k:%M:%S'` this time no monitor"
		sleep 30
		continue;
	fi

	RX_pre=$(cat /proc/net/dev | grep $ethn | sed 's/:/ /g' | awk '{print $2}')
	TX_pre=$(cat /proc/net/dev | grep $ethn | sed 's/:/ /g' | awk '{print $10}')
	sleep 1
	RX_next=$(cat /proc/net/dev | grep $ethn | sed 's/:/ /g' | awk '{print $2}')
	TX_next=$(cat /proc/net/dev | grep $ethn | sed 's/:/ /g' | awk '{print $10}')

	RX=$((${RX_next}-${RX_pre}))
	TX=$((${TX_next}-${TX_pre}))

	if [[ $RX -lt 1024 ]];then
		RX="${RX}B/s"
	elif [[ $RX -gt 1048576 ]];then
		RX=$(echo $RX | awk '{print $1/1048576 "MB/s"}')
	else
		RX=$(echo $RX | awk '{print $1/1024 "KB/s"}')
	fi

	if [[ $TX -lt 1024 ]];then
		TX="${TX}B/s"
	elif [[ $TX -gt 1048576 ]];then
		TX=$(echo $TX | awk '{print $1/1048576 "MB/s"}')
	else
		TX=$(echo $TX | awk '{print $1/1024 "KB/s"}')
	fi

	echo -e " `date '+%Y/%m/%d %k:%M:%S'`	RX=$RX \t TX=$TX"

	netstat -na | egrep "8888|8866|8896|Address"
	
	##增加进程占用的cpu监控
	top -b -n1 `$pidof_bin gta_ints dat2cli | sed 's/[0-9]*/-p&/g'` | egrep "gta_ints|dat2cli|sshd|COMMAND"
	##增加消息队列监控
	ipcs -q
done
