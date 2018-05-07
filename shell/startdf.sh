#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system startup  BEGIN..."

conf_file="$HOME/bin/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

ethdev=${ethdev:-eno1}
writeflag=${writeflag:-1}
workroot=${workroot:-/stock/work}
writeusr=${writeusr:-0}

disp_file="$HOME/conf/disp.json"
user_file="$HOME/conf/user_privilege.json"
tdf_file="$HOME/conf/ints_tdf.json"
cfg_file="$HOME/conf/cfg.json"

ints_tdf_bin="$HOME/bin/ints_tdf"
dat2cli_bin="$HOME/bin/dat2cli"
moni_bin="$HOME/bin/moni.sh"
pidof_bin="/usr/sbin/pidof"

ints_tdf_log="$HOME/bin/log/ints_tdf_`date '+%Y%m%d'`.log"
dat2cli_log="$HOME/bin/log/dat2cli_`date '+%Y%m%d'`.log"
moni_log="$HOME/bin/log/moni_`date '+%Y%m%d'`.log"

[ ! -f $tdf_file ] && echo "$tdf_file is not exist" && exit 1;
[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $user_file ] && echo "$user_file is not exist" && exit 1;

[ ! -f $ints_tdf_bin ] && echo "$ints_tdf_bin is not exist" && exit 1;
[ ! -f $dat2cli_bin ] && echo "$dat2cli_bin is not exist" && exit 1;
[ ! -f $moni_bin ] && echo "$moni_bin is not exist" && exit 1;
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;


$pidof_bin -x ints_tdf && echo "ints_tdf is running" && exit 2;
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

nohup $ints_tdf_bin -w$writeflag -o$workroot -c$tdf_file -r$disp_file -u$user_file 1>$ints_tdf_log 2>&1 &
sleep 1
$pidof_bin -x ints_tdf
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` ints_tdf is startup FAIL..";
	echo "$ints_tdf_bin -w$writeflag -o$workroot -c$tdf_file -r$disp_file -u$user_file"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` ints_tdf is startup SUCESS.."

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








