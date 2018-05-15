#!/bin/bash

echo "`date '+%Y/%m/%d %k:%M:%S'` system shutdown  BEGIN..."

pidof_bin="/usr/sbin/pidof"
[ ! -f $pidof_bin ] && echo "$pidof_bin is not exist" && exit 1;

commons_file="$HOME/bin/commons"
[ ! -f $commons_file ] && echo "$commons_file is not exist" && exit 1;

. $commons_file

pids=`$pidof_bin -x dat2cli ints_gta ints_tdf replay_gta replay_tdf agentcli moni.sh`

if [ -z "$pids" ]; then
	echo "`date '+%Y/%m/%d %k:%M:%S'` system is not start..,exit";
	exit 0;
fi

my_name=`who am i | awk '{print $1}'`
my_name=${my_name:-$USER}
my_flag=""

for i in $pids
do
	##确保只杀自己的进程
	my_flag=`comfirm_mypid $my_name $i`
	
	##不是自己的进程，则打印信息继续
	if [ $my_flag -eq 0 ]; then
	
		belong_user=`belong_user_mypid $i`
		belong_cmd=`belong_cmd_mypid $i`
		echo "`date '+%Y/%m/%d %k:%M:%S'` pid $i cmd=$belong_cmd user=$belong_user is not belong ---$my_name-- IGNORE.."
		continue;
	fi
	
	kill -SIGTERM $i
	sleep 1
	echo "`date '+%Y/%m/%d %k:%M:%S'` send SIGTERM to $i and it is stopped.."
done

##为了避免客户端程序，因为服务端的连接强制关闭后的重连机制造成的dat2cli主进程fork出子进程
##这里追加循环，专门杀reconnect dat2cli进程，避免第二天启动时因为存在这个进程无法启动系统
pids=`$pidof_bin -x dat2cli`
if [ -n "$pids" ]; then
	for i in $pids
	do
		##确保只杀自己的进程
		my_flag=`comfirm_mypid $my_name $i`
		if [ $my_flag -ne 0 ]; then
			kill -SIGTERM $i
			echo "`date '+%Y/%m/%d %k:%M:%S'` send SIGTERM to $i (reconn=dat2cli)and it is stopped.."
		fi
	done
fi

sleep 1;

echo "`date '+%Y/%m/%d %k:%M:%S'` system shutdown  OK..."

exit 0
