#!/bin/sh

if [ $# -gt 1 ]; then
	echo "Usage $0 [clean|install]"
	exit 1
fi

makeflag=$1

makedir()
{
	dir=$1
	case $makeflag in 
	clean*)
		echo "clean $dir ..."
		cd $dir
		case $dir in
		app_dat2cli|app_agentcli)
			rm -rf MakeFiles CMakeCache.txt CMakeFiles
			cd ..
		;;
		*)
			make clean;
			cd ..
		;;
		esac
		;;
	*)
		echo "build $dir ..."
		cd $dir
		case $dir in
		app_dat2cli|app_agentcli)
			rm -rf MakeFiles CMakeCache.txt CMakeFiles
			cmake CMakeLists.txt
			make
			cd ..
		;;
		*)
			make clean;
			make
			cd ..
		;;
		esac
		;;
	esac
}
installdir()
{
	dir=$1
	
	case $makeflag in 
	install*)
		[ -d ../bin ] || mkdir ../bin
		[ -d ../bin/log ] || mkdir ../bin/log
		echo "install $dir ..."
		sleep 1
		case $dir in
		bin)
			cp bin/dat2cli		../bin
			cp bin/gta_ints         ../bin
			cp bin/gta_ints_2       ../bin
			cp bin/gta_replay       ../bin
			cp bin/gta_stat         ../bin
			cp bin/index_stat       ../bin
			cp bin/gta_test         ../bin
			cp bin/agentcli		../bin
			cp bin/tdf_ints		../bin
			;;
		shell)
			cp shell/chkipsend.sh	../bin
			cp shell/putip.exp	../bin
			cp shell/moni.sh	../bin
			cp shell/startup.sh	../bin
			cp shell/shutdown.sh	../bin
			cp shell/killsysxl.sh	../bin
			cp shell/databackup.sh	../bin
			cp shell/dataclean.sh	../bin
			cp shell/logbackup.sh	../bin
			cp shell/logclean.sh	../bin
			cp shell/startrep.sh	../bin
			cp shell/cvnew.sh	../bin
			cp shell/cvd31.sh	../bin
			cp shell/iptables.sh	../bin
			cp shell/startdf.sh	../bin
			;;
		conf)   
			cp conf/cfg.json	../bin
			cp conf/disp.json       ../bin
			cp conf/gta_ints.json   ../bin
			cp conf/user_privilege.json ../bin
			cp conf/config.ini   	../bin
			cp conf/tdf_ints.json   ../bin
			;;
		*)
			;;
		esac
		echo "install $dir ended ..."
		;;
	*)
		;;
	esac
}
main()
{
	case $makeflag in 
	install*)
		installdir bin
		installdir shell
		installdir conf
		;;
	*)
		makedir app_dat2cli
		makedir app_ints
		makedir app_ints_2 
		makedir app_test
		makedir	test
		makedir app_agentcli
		makedir app_tdf
		;;
	esac
}
main
exit 0
