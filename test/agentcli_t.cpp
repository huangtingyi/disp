#include "CallBackBase.h"

void signalProcess(int signal)
{
	if (signal == SIGUSR1){
		printf("signal SIGUSR1...\n");
		return;
	}
	exit(0);
}

void SetProcessSignal()
{
	// 设置信号处理
	signal(SIGQUIT, signalProcess);
	signal(SIGTERM, signalProcess);
	signal(SIGINT, signalProcess);
	signal(SIGSEGV, signalProcess);
	signal(SIGILL, signalProcess);
	signal(SIGABRT, signalProcess);
	signal(SIGFPE, signalProcess);
	signal(SIGPIPE, signalProcess);
	signal(SIGUSR1, signalProcess);
}

CallBackBase *pCallBase;

int main(int argc, char *argv[])
{
	int iPort=0,iMaxCnt=3000,iSleepSec=3,iTimeOutSec=3;
	char sIp[32],sUserName[32],sPassword[32];	
	
	strcpy(sIp,"");
	strcpy(sUserName,"");
	strcpy(sPassword,"");

	for (int c; (c = getopt(argc, argv, "I:P:u:p:c:s:m:?:")) != EOF;){
		switch (c){
		case 'I':
			strncpy(sIp, optarg,sizeof(sIp)-1);
			sIp[sizeof(sIp)-1]=0;
			break;
		case 'P':
			iPort=atoi(optarg);
			break;
		case 'u':
			strncpy(sUserName, optarg,sizeof(sUserName)-1);
			sUserName[sizeof(sUserName)-1]=0;
			break;
		case 'p':
			strncpy(sPassword, optarg,sizeof(sPassword)-1);
			sPassword[sizeof(sPassword)-1]=0;
			break;
		case 'c':
			iMaxCnt=atoi(optarg);
			break;
		case 's':
			iSleepSec=atoi(optarg);
			break;
		case 'm':
			iTimeOutSec=atoi(optarg);
			if(iTimeOutSec<=0) iTimeOutSec=3;
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-I ip ]\n");
			printf("   [-P port ]\n");
			printf("   [-u username ]\n");
			printf("   [-p password ]\n");
			printf("   [-c maxcnt ]\n");
			printf("   [-s sleepsec ]\n");
			printf("   [-m timeoutsec ]\n");
			exit(1);
			break;
		}
	}

	printf("hello world ip=%s,port=%d,username=%s,password=%s.\n",
		sIp,iPort,sUserName,sPassword);

	SetProcessSignal();
	
	pCallBase=new CallBackBase(sIp,iPort,sUserName,sPassword,iMaxCnt);
	
	//连接服务器
	if(pCallBase->Connect()==false){
		printf("connect server error ip=%s,port=%d.\n",sIp,iPort);
		return -1;
	}
	
	printf("connect server ip=%s,port=%d OK.\n",sIp,iPort);

	//发送登录信息
	pCallBase->Login();
	
	//将所有的登录一下
	pCallBase->SubscribeAll();
	
	//等待退出标志，如果为真则退出
	//做一个最大等待时间控制，如果超过最大等待时间则退出
	time_t tBgnTime,tCurTime;
	
	tBgnTime=tGetHostTime();

	while(pCallBase->m_exit==false){
		usleep(10*1000);
		tCurTime=tGetHostTime();
		
		//超时,打印信息就直接退出了
		if((tCurTime-tBgnTime)>=iTimeOutSec){
			printf("TIME OUT T=%d RecCnt=%d GetCnt=%d EXIT.\n",
				iTimeOutSec,iMaxCnt,pCallBase->m_iTotalCnt);
			return 1;
		}
	}
	
	printf("CATCH EXIT SIGNAL.\n");

	//为了避免退出时，好多消息堵塞在队列中
	//将所有的代码退订
	pCallBase->SubscribeNo();
	
	//休眠三秒
	while(iSleepSec>0){
		static int iTotalCnt=0;

		sleep(1);

		//打印出统计信息
		if(iTotalCnt!=pCallBase->m_iTotalCnt){
			pCallBase->DisplayStat(D31_ITEM);
			iTotalCnt=pCallBase->m_iTotalCnt;
		}

		iSleepSec--;
	}
	//pCallBase->DisplayStat(D31_ITEM);
	
	
	printf("TEST OK APP EXIT.\n");

	return 0;
}
