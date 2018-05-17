#!/bin/bash

if [ $# -ne 0 -a $# -ne 1 ]; then
	echo "usage $0 or $0 days(defs=config.logdays)"
	exit 1
fi

##判断$1是否为数值，如果非数值则退出
if [ $# -eq 1 ]; then
	if [ -z "`echo $1 | sed -n '/^[0-9][0-9]*$/p'`" ]; then
		echo "param1=$1 must in integer e.g (1,2,3 ... )"
		echo "usage $0 or $0 days(defs=config.logdays)"
	else
		echo "hello error"
	fi
fi

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;
. $conf_file


logdays=${1:-${logdays:-7}}
logpath=$HOME/bin/log

##检查日志路径是否存在
if [ ! -d "$logpath" ]; then
	echo "$logpath is not exist"
	exit 1;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` $logpath path $logdays day agos log clean BEGIN"

find $logpath -mtime +$logdays  2>/dev/null | while read tmp
do
	rm -rf $tmp
	if [ $? -eq 0 ]; then
		echo "`date '+%Y/%m/%d %k:%M:%S'` $tmp file cleaned..."
	else
		echo "`date '+%Y/%m/%d %k:%M:%S'` $tmp file clean fail..."
		exit 1
	fi
	sleep 1
done

echo "`date '+%Y/%m/%d %k:%M:%S'` $logpath path $logdays day agos log clean OK"

exit 0
