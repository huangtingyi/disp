#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system startup  BEGIN..."

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

ethdev=${ethdev:-eno1}
writeflag=${writeflag:-1}
workroot=${workroot:-/stock/work}
writeusr=${writeusr:-0}

gta_file="$HOME/conf/ints_gta.json"
tdf_file="$HOME/conf/ints_tdf.json"
ints_gta_bin="$HOME/bin/ints_gta"
ints_tdf_bin="$HOME/bin/ints_tdf"
index_gta_bin="$HOME/bin/index_gta"
index_tdf_bin="$HOME/bin/index_tdf"

disp_file="$HOME/conf/disp.json"
user_file="$HOME/conf/user_privilege.json"
cfg_file="$HOME/conf/cfg.json"

dat2cli_bin="$HOME/bin/dat2cli"
moni_bin="$HOME/bin/moni.sh"
pidof_bin="/usr/sbin/pidof"

ints_gta_log="$HOME/bin/log/ints_gta`date '+%Y%m%d'`.log"
ints_tdf_log="$HOME/bin/log/ints_tdf`date '+%Y%m%d'`.log"
index_gta_log="$HOME/bin/log/index_gta`date '+%Y%m%d'`.log"
index_tdf_log="$HOME/bin/log/index_tdf`date '+%Y%m%d'`.log"

dat2cli_log="$HOME/bin/log/dat2cli_`date '+%Y%m%d'`.log"
moni_log="$HOME/bin/log/moni_`date '+%Y%m%d'`.log"

if [ $sysflag = "gta" ]; then
	ints_file=$gta_file
	ints_bin=$ints_gta_bin
	ints_log=$ints_gta_log
	index_bin=$index_gta_bin
	index_log=$index_gta_log
else
	ints_file=$tdf_file
	ints_bin=$ints_tdf_bin
	ints_log=$ints_tdf_log
	index_bin=$index_tdf_bin
	index_log=$index_tdf_log
fi

[ ! -f $ints_file ] && echo "$ints_file is not exist" && exit 1;
[ ! -f $ints_bin ] && echo "$ints_bin is not exist" && exit 1;
[ ! -f $index_bin ] && echo "$index_bin is not exist" && exit 1;

[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $user_file ] && echo "$user_file is not exist" && exit 1;

[ ! -f $dat2cli_bin ] && echo "$dat2cli_bin is not exist" && exit 1;
[ ! -f $moni_bin ] && echo "$moni_bin is not exist" && exit 1;
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

workd31=${workd31:-"/data/work"}
etflist=${etflist:-"510050,510180,510300,510500"}
etfpath=${etfpath:-$HOME/conf/etf}

[ ! -d $workroot ] && echo "dir $workroot is not exist" && exit 1;
[ ! -d $workd31 ] && echo "dir $workd31 is not exist" && exit 1;
[ ! -d $etfpath ] && echo "dir $etfpath is not exist" && exit 1;



my_name=`whoami`
my_name=${my_name:-$USER}
my_flag=""

ints_bin_base=`basename $ints_bin`
index_bin_base=`basename $index_bin`

pids=`$pidof_bin -x $ints_bin_base $index_bin_base dat2cli moni.sh`

##如果有指定进程在运行，则检查是否有本用户的进程存在，如果有则提示退出
mypid=`check_mypid_exist $pids`
if [ $mypid -ne 0 ];then
	belong_cmd=`belong_cmd_mypid $mypid`
	echo "`date '+%Y/%m/%d %k:%M:%S'` pid $mypid cmd=$belong_cmd user=$my_name is running"
	exit 2
fi

##清除mq队列
remove_mymq_all

##清除信号量
remove_mysem_all

##清空disp.json文件
cat > $disp_file << 'EOF'
{
    "users": ""
}
EOF

cd $HOME/bin

if [ $sysflag = "gta" ]; then
	
nohup $ints_bin -w$writeflag -o$workroot -c$ints_file -r$disp_file -d$workd31 1>$ints_log 2>&1 &
sleep 1
$pidof_bin -x $ints_bin_base

if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` $ints_bin_base is startup FAIL..";
	echo "$ints_bin -w$writeflag -o$workroot -c$ints_file -r$disp_file -d$workd31"
	exit 3;
fi
echo "`date '+%Y/%m/%d %k:%M:%S'` $ints_bin_base is startup SUCESS.."
	
##启动D31统计程序
nohup $index_bin -w3 -s$workroot -o$workd31 -L$etflist -E$etfpath 1>$index_log 2>&1 &
sleep 1
$pidof_bin -x $index_bin_base
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` $index_bin_base is startup FAIL..";
	echo "$index_bin -w3 -s$workroot -o$workd31 -L$etflist -E$etfpath"
	exit 3;
fi
echo "`date '+%Y/%m/%d %k:%M:%S'` $index_bin_base is startup SUCESS.."

nohup $dat2cli_bin -w$writeusr -o$workroot -p$cfg_file -r$disp_file -u$user_file 1>$dat2cli_log 2>&1 &
sleep 1
$pidof_bin -x dat2cli
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` dat2cli is startup FAIL..";
	echo "$dat2cli_bin -w$writeusr -o$workroot -p$cfg_file -r$disp_file -u$user_file"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` dat2cli is startup SUCESS.."

nohup $moni_bin $ethdev 1>$moni_log 2>&1 &

sleep 1
$pidof_bin -x moni.sh
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` moni.sh startup FAIL..";
	echo "$moni_bin $ethdev"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` moni.sh is startup SUCESS.."

sleep 1;
echo "`date '+%Y/%m/%d %k:%M:%S'` system startup  OK..."

exit 0
