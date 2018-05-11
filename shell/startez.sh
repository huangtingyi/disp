#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system startup  BEGIN..."

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

commons_file="$HOME/bin/commons"
[ ! -f $commons_file ] && echo "$commons_file is not exist" && exit 1;

. $commons_file

ethdev=${ethdev:-eno1}
workroot=${workroot:-/stock/work}
writeusr=${writeusr:-0}

agent_file="$HOME/conf/agentcfg.json"
disp_file="$HOME/conf/disp.json"
user_file="$HOME/conf/user_privilege.json"
cfg_file="$HOME/conf/cfg.json"

agent_bin="$HOME/bin/agentcli"
dat2cli_bin="$HOME/bin/dat2cli"
moni_bin="$HOME/bin/moni.sh"
pidof_bin="/usr/sbin/pidof"

agentcli_log="$HOME/bin/log/agentcli_`date '+%Y%m%d'`.log"
dat2cli_log="$HOME/bin/log/dat2cli_`date '+%Y%m%d'`.log"
moni_log="$HOME/bin/log/moni_`date '+%Y%m%d'`.log"

[ ! -f $agent_file ] && echo "$agent_file is not exist" && exit 1;
[ ! -f $cfg_file ] && echo "$cfg_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $user_file ] && echo "$user_file is not exist" && exit 1;

[ ! -f $agent_bin ] && echo "$agent_bin is not exist" && exit 1;
[ ! -f $dat2cli_bin ] && echo "$dat2cli_bin is not exist" && exit 1;
[ ! -f $moni_bin ] && echo "$moni_bin is not exist" && exit 1;
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

my_name=`who am i | awk '{print $1}'`
my_flag=""


#$pidof_bin -x agentcli && echo "agentcli is running" && exit 2;
#$pidof_bin -x dat2cli && echo "dat2cli is running" && exit 2;
#$pidof_bin -x moni.sh && echo "moni.sh is running" && exit 2;

pids=`$pidof_bin -x agentcli dat2cli moni.sh`

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

nohup $agent_bin -p$agent_file -r$disp_file 1>$agentcli_log 2>&1 &
sleep 1
$pidof_bin -x agentcli
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` agentcli is startup FAIL..";
	echo "$$agent_bin -p$agent_file -r$disp_file"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S'` agentcli is startup SUCESS.."

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








