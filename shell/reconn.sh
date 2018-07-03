#!/bin/bash

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

commons_file="$HOME/bin/commons"
[ ! -f $commons_file ] && echo "$commons_file is not exist" && exit 1;

. $commons_file

sysflag="gta"
if [ "`echo $0 | sed 's/tdf.sh//g'`"  != "$0" ];then
	sysflag="tdf"
fi

ints_bin="ints_$sysflag"

echo "`date '+%Y/%m/%d %k:%M:%S.%N'` $ints_bin restartup  BEGIN..."

my_name=`whoami`
my_name=${my_name:-$USER}

##拼接命令，执行获得结果及增加填写默认值，为了安全起见取，用head第一行
pscmd="ps -ef | grep $ints_bin |awk '\$1==\"$my_name\"&&\$3==1{print \$2}' | head -1"
cmdpid=`eval $pscmd`
cmdpid=${cmdpid:-0}

if [ $cmdpid -ne 0 ];then
	kill -9 $cmdpid
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` cmd $ints_bin STOP..."
fi

writeflag=${writeflag:-1}
workroot=${workroot:-/stock/work}
writeusr=${writeusr:-0}

ints_file="$HOME/conf/ints_$sysflag.json"
ints_bin="$HOME/bin/ints_$sysflag"
ints_log="$HOME/bin/log/ints_"$sysflag"_`date '+%Y%m%d'`.log"

cfg_file="$HOME/conf/cfg.json"
disp_file="$HOME/conf/disp.json"

pidof_bin="/usr/sbin/pidof"

[ ! -f $ints_file ] && echo "$ints_file is not exist" && exit 1;
[ ! -f $ints_bin ] && echo "$ints_bin is not exist" && exit 1;

[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;

[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

workd31=${workd31:-"/data/work"}

[ ! -d $workroot ] && echo "dir $workroot is not exist" && exit 1;
[ ! -d $workd31 ] && echo "dir $workd31 is not exist" && exit 1;

max_mq_msg_len=`grep "SysMqMaxLen" $cfg_file | sed 's/[^0-9]//g'`
if [ -z $max_mq_msg_len ];then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` file $cfg_file SysMqMaxLen config error"
	exit 4;
fi

ints_bin_base=`basename $ints_bin`
	
nohup stdbuf --output=L --error=L $ints_bin -w$writeflag -o$workroot -c$ints_file -r$disp_file -d$workd31 -l$max_mq_msg_len 1>$ints_log 2>&1 &
sleep 1
$pidof_bin -x $ints_bin_base

if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` $ints_bin_base is restartup FAIL..";
	echo "$ints_bin -w$writeflag -o$workroot -c$ints_file -r$disp_file -d$workd31 -l$max_mq_msg_len"
	exit 3;
fi
echo "`date '+%Y/%m/%d %k:%M:%S.%N'` $ints_bin_base is restartup SUCESS.."
