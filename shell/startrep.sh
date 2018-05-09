#!/bin/bash


if [ $# -ne 1 -a $# -ne 2 -a $# -ne 3 ]; then
	echo "Usage $0 replaypath [ replay_date ] [replaytime: fmt,HHMMSS or HHMMSSNNN] "
	exit 1
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


echo "`date '+%Y/%m/%d %k:%M:%S'` system startupREPLAY  BEGIN..."

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

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
dat2cli_bin="$HOME/bin/dat2cli"
moni_bin="$HOME/bin/moni.sh"
pidof_bin="/usr/sbin/pidof"

replay_gta_log="$HOME/bin/log/replay_gta_`date '+%Y%m%d'`.log"
dat2cli_log="$HOME/bin/log/dat2cli_`date '+%Y%m%d'`.log"
moni_log="$HOME/bin/log/moni_`date '+%Y%m%d'`.log"

[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $user_file ] && echo "$user_file is not exist" && exit 1;

[ ! -f $replay_gta_bin ] && echo "$replay_gta_bin is not exist" && exit 1;
[ ! -f $dat2cli_bin ] && echo "$dat2cli_bin is not exist" && exit 1;
[ ! -f $moni_bin ] && echo "$moni_bin is not exist" && exit 1;
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;


$pidof_bin -x replay_gta_bin && echo "replay_gta is running" && exit 2;
$pidof_bin -x dat2cli && echo "dat2cli is running" && exit 2;
$pidof_bin -x moni.sh && echo "moni.sh is running" && exit 2;

##清除mq队列
ipcs -q | grep "0x" | awk '{print $2}' | while read tmp
do
	ipcrm -q $tmp 1>/dev/null 2>&1
	if [ $? -eq 0 ]; then
		echo "ipcrm -q $tmp sucess"
	fi
done

##清除信号量
ipcs -s | grep "0x" | awk '{print $2}' | while read tmp
do
	ipcrm -s $tmp 1>/dev/null 2>&1
	if [ $? -eq 0 ]; then
		echo "ipcrm -s $tmp sucess"
	fi
done

##清空disp.json文件
cat > $disp_file << 'EOF'
{
    "users": ""
}
EOF

cd $HOME/bin


##./replay_gta -d20180412 -r/home/hty/bin/disp.json -w0 -t200 -s/data/20180412

nohup $replay_gta_bin -s$replaypath -d$replaydate -b$replaytime -r$disp_file -o$workroot -w$writeflag -t$replaydelay -m$replaymulti 1>$replay_gta_log 2>&1 &
sleep 1
$pidof_bin -x replay_gta
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` $replay_gta_bin is startup FAIL..";
	echo "$replay_gta_bin -s$replaypath -d$replaydate -b$replaytime -r $disp_file -o $workroot -w$writeflag -t$replaydelay -m$replaymulti"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` replay_gta is startREPLAY SUCESS.."

nohup $dat2cli_bin -w$writeusr -o$workroot -p$cfg_file -r$disp_file -u$user_file 1>$dat2cli_log 2>&1 &
sleep 1
$pidof_bin -x dat2cli
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` dat2cli is startup FAIL..";
	echo "$dat2cli_bin -w $writeusr -o $workroot -p $cfg_file -r $disp_file -u $user_file"
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








