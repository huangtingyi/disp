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
			rm -fr MakeFiles CMakeCache.txt CMakeFiles
			echo "rm -fr MakeFiles CMakeCache.txt CMakeFiles"
			cd ..
			if [ $dir = "app_dat2cli" ]; then
				rm -fr bin/dat2cli
				echo "rm -rf bin/dat2cli"
			else
				rm -fr bin/agentcli
				echo "rm -fr bin/agentcli"
			fi
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
		[ -d ../conf ] || mkdir ../conf
		[ -d ../bin/log ] || mkdir ../bin/log
		echo "install $dir ..."
		sleep 1
		case $dir in
		bin)
			cp bin/dat2cli		../bin
			cp bin/ints_gta         ../bin
			cp bin/ints_gta_t       ../bin
			cp bin/replay_gta       ../bin
			cp bin/index_gta        ../bin
			cp bin/dmpi2txt         ../bin
			cp bin/dmpo2txt         ../bin
			cp bin/gta2tdf		../bin
			cp bin/ints_tdf		../bin
			cp bin/ints_tdf_t	../bin
			cp bin/agentcli		../bin
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
			cp conf/cfg.json	../conf
			cp conf/disp.json       ../conf
			cp conf/ints_tdf.json   ../conf
			cp conf/user_privilege.json ../conf
			cp conf/config.ini   	../conf
			cp conf/ints_tdf.json   ../conf
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
		makedir app_gta
		makedir app_index
		makedir	test
		makedir app_agentcli
		makedir app_tdf
		;;
	esac
}
main
exit 0
