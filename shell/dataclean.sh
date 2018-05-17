#!/bin/bash

if [ $# -ne 0 -a $# -ne 2 ]; then
	echo "usage $0 or $0 type(1=work,2 backup) cleandate(yyyymmdd)"
	exit 1
fi

mytype=${1:-0}
cleandate=${2:-"`date '+%Y%m%d'`"}

##校验参数合法性
if [ $# -eq 2 ]; then
	tmpdate=`date -d "$2" +%Y%m%d`
	if [ "$tmpdate"X != "$2"X ];then
		echo "param1=$2 must in yyyymmdd format e.g `date '+%Y%m%d'`"
		echo "usage $0 or $0 type(1=work,2 backup) cleandate(yyyymmdd)"
		exit 1;
	fi
	
	if [ $mytype != "0" -a $mytype != "1" -a $mytype != "2" ]; then
		echo "param1=$1 must in (1=work,2 backup)"
		echo "usage $0 or $0 type(1=work,2 backup) cleandate(yyyymmdd)"
		exit 1;
	fi
fi

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

workdays=${workdays:-2}
backupdays=${backupdays:-100}

workroot=${workroot:-/stock/work}
dataroot=${dataroot:-/data}

[ ! -d $workroot ] && echo "directory $workroot is not exist" && exit 1;
[ ! -d $dataroot ] && echo "dataroot $workroot is not exist" && exit 1;

if [ $# -eq 0 ];then
	clean_bakdate=`date -d "$cleandate $backupdays days ago"  "+%Y%m%d"`
	##如果是星期一、二，要扣除周末两天
	if [ "`date +%w`" -le 2 ];then
		workdays=`echo "$workdays + 2" | bc`
	fi
	clean_workdate=`date -d "$cleandate $workdays days ago"  "+%Y%m%d"`
else
	clean_bakdate="$cleandate";
	clean_workdate="$cleandate";
fi

clean_bakpath="$dataroot/$clean_bakdate"
clean_workpath="$workroot/$clean_workdate"

echo clean_bakpath=$clean_bakpath clean_work=$clean_workpath

if [ $mytype -eq 0 -o $mytype -eq 1 ]; then

	echo "`date '+%Y/%m/%d %k:%M:%S'` $workroot $clean_workdate file clean BEGIN"

	ls $workroot/[g|t]*$clean_workdate* 2>/dev/null | while read tmp
	do
		echo "$tmp"
		rm -rf $tmp
		if [ $? -eq 0 ]; then
			echo "`date '+%Y/%m/%d %k:%M:%S'` $tmp file cleaned..."
		else
			echo "`date '+%Y/%m/%d %k:%M:%S'` $tmp file clean fail..."
			exit 1
		fi
		sleep 1
	done

	echo "`date '+%Y/%m/%d %k:%M:%S'` $workroot $clean_workdate file clean OK"
fi

if [ $mytype -eq 0 -o $mytype -eq 2 ]; then

	echo "`date '+%Y/%m/%d %k:%M:%S'` $clean_bakpath path clean BEGIN"

	ls $clean_bakpath  2>/dev/null | while read tmp
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
	
	if [ -d $clean_bakpath ];then
		rm -rf $clean_bakpath
		if [ $? -eq 0 ]; then
			echo "`date '+%Y/%m/%d %k:%M:%S'` $clean_bakpath path cleaned OK..."
		else
			echo "`date '+%Y/%m/%d %k:%M:%S'` $clean_bakpath path cleaned fail..."
		fi
	else
		echo "`date '+%Y/%m/%d %k:%M:%S'` $clean_bakpath path not exist..."
	fi
fi

exit 0
