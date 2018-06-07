#!/bin/bash

if [ $# -ne 1 -a $# -ne 2 -a $# -ne 3 ]; then
	echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
	exit 1
fi

sysflag="gta"
if [ "`echo $0 | sed 's/tdf.sh//g'`"  != "$0" ];then
	sysflag="tdf"
fi

##当参数为1,2,3 个时，设置默认参数
replaypath=$1

replaylen=${#replaypath}
replaybgn=`echo $replaylen-8 | bc`
replaytmp=${replaypath:$replaybgn:8}
replaydate=${2:-$replaytmp}
replaytime=${3:-"091500"}

##检查参数合法性
[ ! -d $replaypath ] && echo "directory $replaypath is not exist" && exit 1;

##检查是否是8位的全数字
if [ ${#replaydate} -ne 8 ];then
	echo "param2=$2 length mast be 8 e.g 20180101"
	echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
	exit 1
fi
if [ "`echo "$replaydate" | sed 's/[^0-9]//g'`"  != $replaydate ];then
	echo "param2=$2 mast be digit e.g 20180101"
	echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
	exit 1
fi

##检查是否是 5,6,8,9 位的全数字
case "${#replaytime}" in 
	5|6|8|9)
	if [ "`echo "$replaytime" | sed 's/[^0-9]//g'`"  != $replaytime ];then
		echo "param3=$3 mast be digit e.g 93001,093001,93000500,093000500"
		echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
		exit 1
	fi
	;;
	*)
	echo "param3=$3 length mast be 5,6,8,9 e.g 93001,093001,93000500,093000500"
	echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
	exit 1
	;;
esac

if [ ${#replaytime} -eq 5 -o ${#replaytime} -eq 6 ];then
	replaytime="$replaytime""000"
fi


echo "`date '+%Y/%m/%d %k:%M:%S.%N'` system startupREPLAY  BEGIN..."

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

commons_file="$HOME/bin/commons"
[ ! -f $commons_file ] && echo "$commons_file is not exist" && exit 1;

. $commons_file


ethdev=${ethdev:-eno1}
writeflag=${replaywriteflag:-1}
workroot=${replayroot:-/stock/work/replay}
writeusr=${replaywriteusr:-0}
	
replaydelay=${replaydelay:-200}
replaymulti=${replaymulti:-1}

disp_file="$HOME/conf/disp.json"
user_file="$HOME/conf/user_privilege.json"
cfg_file="$HOME/conf/cfg.json"

replay_gta_bin="$HOME/bin/replay_gta"
replay_tdf_bin="$HOME/bin/replay_tdf"

dat2cli_bin="$HOME/bin/dat2cli"
moni_bin="$HOME/bin/moni.sh"
pidof_bin="/usr/sbin/pidof"

replay_gta_log="$HOME/bin/log/replay_gta_`date '+%Y%m%d'`.log"
replay_tdf_log="$HOME/bin/log/replay_tdf_`date '+%Y%m%d'`.log"

dat2cli_log="$HOME/bin/log/dat2cli_`date '+%Y%m%d'`.log"
moni_log="$HOME/bin/log/moni_`date '+%Y%m%d'`.log"

[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $user_file ] && echo "$user_file is not exist" && exit 1;

if [ $sysflag = "gta" ]; then
	replay_bin=$replay_gta_bin
	replay_log=$replay_gta_log
else
	replay_bin=$replay_tdf_bin
	replay_log=$replay_tdf_log
fi

replay_bin_base=`basename $replay_bin`

[ ! -f $replay_bin ] && echo "$replay_bin is not exist" && exit 1;
[ ! -f $dat2cli_bin ] && echo "$dat2cli_bin is not exist" && exit 1;
[ ! -f $moni_bin ] && echo "$moni_bin is not exist" && exit 1;
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

my_name=`whoami`
my_name=${my_name:-$USER}
my_flag=""

max_mq_msg_len=`grep "SysMqMaxLen" $cfg_file | sed 's/[^0-9]//g'`
if [ -z $max_mq_msg ];then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` file $cfg_file SysMqMaxLen config error"
	exit 4;
fi

pids=`$pidof_bin -x $replay_bin_base dat2cli moni.sh`

##如果有指定进程在运行，则检查是否有本用户的进程存在，如果有则提示退出
mypid=`check_mypid_exist $pids`
if [ $mypid -ne 0 ];then
	belong_cmd=`belong_cmd_mypid $mypid`
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` pid $mypid cmd=$belong_cmd user=$my_name is running"
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


##./replay_gta -d20180412 -r/home/hty/bin/disp.json -w0 -t200 -s/data/20180412

nohup stdbuf --output=L --error=L $replay_bin -s$replaypath -d$replaydate -b$replaytime -r$disp_file -o$workroot -w$writeflag -t$replaydelay -lmax_mq_msg_len -m$replaymulti 1>$replay_log 2>&1 &
sleep 1
$pidof_bin -x $replay_bin_base
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` $replay_bin_base is startup FAIL..";
	echo "$replay_bin -s$replaypath -d$replaydate -b$replaytime -r$disp_file -o$workroot -w$writeflag -t$replaydelay -m$replaymulti"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S.%N'` $replay_bin_base is startREPLAY SUCESS.."

nohup $dat2cli_bin -w$writeusr -o$workroot -p$cfg_file -r$disp_file -u$user_file 1>$dat2cli_log 2>&1 &
sleep 1
$pidof_bin -x dat2cli
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` dat2cli is startup FAIL..";
	echo "$dat2cli_bin -w$writeusr -o$workroot -p$cfg_file -r$disp_file -u$user_file"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S.%N'` dat2cli is startup SUCESS.."

nohup $moni_bin $ethdev 1>$moni_log 2>&1 &

sleep 1
$pidof_bin -x moni.sh
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` moni.sh startup FAIL..";
	echo "$moni_bin $ethdev"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S.%N'` moni.sh is startup SUCESS.."

sleep 1;
echo "`date '+%Y/%m/%d %k:%M:%S.%N'` system startup  OK..."

exit 0
