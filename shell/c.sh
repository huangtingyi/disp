#!/bin/sh

if [ $# -ne 1 -a $# -ne 2 ]; then
	echo "Usage $0 d31csv [type=1,2]"
	exit 1
fi

d31_file="$1"

type=${2:-1}

cmd_bin="cvnew.sh"

if [ $type -ne 1 ];then
	cmd_bin="cvext.sh"
fi

[ ! -f $d31_file ] && echo "$d31_file is not exist" && exit 1;

time_str=""

for(( i=30;i<60;i++))
{
	time_str=`printf "09:%02.0f" $i`
	
	$cmd_bin $d31_file $time_str
}

for(( i=0;i<60;i++))
{
	time_str=`printf "10:%02.0f" $i`
	
	$cmd_bin $d31_file $time_str
}

for(( i=0;i<=30;i++))
{
	time_str=`printf "11:%02.0f" $i`
	
	$cmd_bin $d31_file $time_str
}

for(( i=1;i<60;i++))
{
	time_str=`printf "13:%02.0f" $i`
	
	$cmd_bin $d31_file $time_str
}

for(( i=0;i<60;i++))
{
	time_str=`printf "14:%02.0f" $i`
	
	$cmd_bin $d31_file $time_str
}

$cmd_bin $d31_file "15:00"



exit 0
