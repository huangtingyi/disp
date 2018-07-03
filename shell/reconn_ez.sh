#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` agentcli restartup  BEGIN..."

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

commons_file="$HOME/bin/commons"
[ ! -f $commons_file ] && echo "$commons_file is not exist" && exit 1;

. $commons_file


agent_file="$HOME/conf/agentcfg.json"
disp_file="$HOME/conf/disp.json"

agent_bin="$HOME/bin/agentcli"

agentcli_log="$HOME/bin/log/agentcli_`date '+%Y%m%d'`.log"

[ ! -f $agent_file ] && echo "$agent_file is not exist" && exit 1;
[ ! -f $disp_file ] && echo "$disp_file is not exist" && exit 1;
[ ! -f $agent_bin ] && echo "$agent_bin is not exist" && exit 1;

my_name=`whoami`
my_name=${my_name:-$USER}
my_flag=""

##拼接命令，执行获得结果及增加填写默认值，为了安全起见取，用head第一行
pscmd="ps -ef | grep agentcli |awk '\$1==\"$my_name\"&&\$3==1{print \$2}' | head -1"
cmdpid=`eval $pscmd`
cmdpid=${cmdpid:-0}

if [ $cmdpid -ne 0 ];then
	kill -9 $cmdpid
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` cmd agentcli STOP..."
fi


nohup stdbuf --output=L --error=L $agent_bin -p$agent_file -r$disp_file 1>$agentcli_log 2>&1 &
sleep 1
$pidof_bin -x agentcli
if [ $? -ne 0 ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` agentcli is restartup FAIL..";
	echo "$$agent_bin -p$agent_file -r$disp_file"
	exit 3;
fi

echo "`date '+%Y/%m/%d %k:%M:%S.%N'` agentcli is restartup SUCESS.."

exit 0
