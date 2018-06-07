#!/bin/bash

if [ $# -ne 0 -a $# -ne 1 ]; then
	echo "usage $0 or $0 backupdate(yyyymmdd)"
	exit 1
fi

##校验参数合法性
if [ $# -eq 1 ]; then
	tmpdate=`date -d "$1" +%Y%m%d`
	if [ "$tmpdate"X != "$1"X ];then
		echo "param1=$1 must in yyyymmdd format e.g $tmpdate"
		echo "usage $0 or $0 backupdate(yyyymmdd)"
		exit 1;
	fi
fi

bakdate=${1:-"`date '+%Y%m%d'`"}

conf_file="$HOME/conf/config.ini"
[ ! -f $conf_file ] && echo "$conf_file is not exist" && exit 1;

. $conf_file

workroot=${workroot:-/stock/work}
dataroot=${dataroot:-/data}
replaydir=${replaydir:-/data/work}
workd31=${workd31:-/data/work}



[ ! -d $workroot ] && echo "directory $workroot is not exist" && exit 1;
[ ! -d $dataroot ] && echo "directory $dataroot is not exist" && exit 1;

bakpath="$dataroot/$bakdate"

#这里的-d 参数判断$bakpath是否存在
if [ ! -d "$bakpath" ]; then
	mkdir "$bakpath"
	if [ $? -ne 0 ]; then
		echo "mkdir $bakpath fail"
		exit 1;
	fi
fi

gta_ah_file="gta_ah_$bakdate.dat";
gta_qh_file="gta_qh_$bakdate.dat";
gta_th_file="gta_th_$bakdate.dat";
gta_oz_file="gta_oz_$bakdate.dat";
gta_qz_file="gta_qz_$bakdate.dat";
gta_tz_file="gta_tz_$bakdate.dat";

tdf_mkt_file="tdf_mkt_$bakdate.dat";
tdf_que_file="tdf_que_$bakdate.dat";
tdf_ord_file="tdf_ord_$bakdate.dat";
tdf_tra_file="tdf_tra_$bakdate.dat";

tdf_mk_file="tdf_mk_$bakdate.dat";
tdf_qu_file="tdf_qu_$bakdate.dat";
tdf_or_file="tdf_or_$bakdate.dat";
tdf_tr_file="tdf_tr_$bakdate.dat";

d31_w3_file="d31_w3_$bakdate.dat";


###定义备份函数
backup_file()
{
	source_file=$1
	target_file=$2
	
	if [ ! -f $source_file ]; then
		echo file $source_file not exist IGNORE.
		return 0;		
	fi

	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` backup $source_file BEGIN..."
	gzip -c $source_file > $target_file
	if [ $? -ne 0 ]; then
		echo "`date '+%Y/%m/%d %k:%M:%S.%N'` backup $source_file FAIL"
		return 1;
	fi
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` backup $source_file to $bakpath OK..."

	##替换文件名为replaydir下的，非压缩文件名
	target_file="$replaydir/`basename $target_file`"
	target_file=${target_file/".gz"/}
	
	if [ $source_file = $target_file ]; then
		echo "`date '+%Y/%m/%d %k:%M:%S.%N'` file $source_file in $replaydir IGNORE."
		return 0;		
	fi

	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` move $source_file BEGIN..."
	
	mv $source_file $target_file
	if [ $? -ne 0 ]; then
		echo "`date '+%Y/%m/%d %k:%M:%S.%N'` move $source_file to $target_file FAIL"
		return 1;
	fi
	echo "`date '+%Y/%m/%d %k:%M:%S.%N'` move $source_file to $path OK..."

}

backup_file $workroot/$gta_ah_file $bakpath/$gta_ah_file.gz || exit 1;
backup_file $workroot/$gta_qh_file $bakpath/$gta_qh_file.gz || exit 1;
backup_file $workroot/$gta_th_file $bakpath/$gta_th_file.gz || exit 1;
backup_file $workroot/$gta_oz_file $bakpath/$gta_oz_file.gz || exit 1;
backup_file $workroot/$gta_qz_file $bakpath/$gta_qz_file.gz || exit 1;
backup_file $workroot/$gta_tz_file $bakpath/$gta_tz_file.gz || exit 1;    

backup_file $workroot/$tdf_mkt_file $bakpath/$tdf_mkt_file.gz || exit 1;
backup_file $workroot/$tdf_que_file $bakpath/$tdf_que_file.gz || exit 1;
backup_file $workroot/$tdf_ord_file $bakpath/$tdf_ord_file.gz || exit 1;
backup_file $workroot/$tdf_tra_file $bakpath/$tdf_tra_file.gz || exit 1;

backup_file $workroot/$tdf_mk_file $bakpath/$tdf_mk_file.gz || exit 1;
backup_file $workroot/$tdf_qu_file $bakpath/$tdf_qu_file.gz || exit 1;
backup_file $workroot/$tdf_or_file $bakpath/$tdf_or_file.gz || exit 1;
backup_file $workroot/$tdf_tr_file $bakpath/$tdf_tr_file.gz || exit 1;

backup_file $workd31/$d31_w3_file $replaydir/$d31_w3_file.gz || exit 1;


echo "`date '+%Y/%m/%d %k:%M:%S.%N'` backup $bakdate file All OK..."

exit 0
