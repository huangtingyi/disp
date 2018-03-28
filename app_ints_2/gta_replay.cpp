//////////////////////////////////////////////////////////////////////////
/// @brief    基础API
///           实现不同方式的定阅和取消订阅及快照查询
//////////////////////////////////////////////////////////////////////////
#include "GTAQTSInterfaceBase.h"
#include "CallBackBase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "../Common/IoService.h"
#include "../Common/TinyThread.h"

#include "../GTA2TDF/GTA2TDF.h"


#include "../Common/public.h"

#define MY_DATE_CEIL_LONG 1000000000L

int iWriteFlag=0,iDelayMilSec=100,iMultiTimes=1;
char sDispName[1024],sSourcePath[1024];

CallBackBase *pCallBack;

void *MainReplayRunQh(void *);
void *MainReplayRunTh(void *);
void *MainReplayRunAh(void *);

void *MainReplayRunQz(void *);
void *MainReplayRunTz(void *);
void *MainReplayRunOz(void *);


int main(int argc, char *argv[])
{

	string strWork;
	strcpy(sDispName,	"./disp.json");
	strcpy(sSourcePath,	"/stock/work");
	
	strWork=string(sSourcePath);

	for (int c; (c = getopt(argc, argv, "r:w:t:m:s:")) != EOF;){

		switch (c){
		case 'r':
			strcpy(sDispName, optarg);
			break;
		case 'w':
			iWriteFlag=atoi(optarg);
			if(iWriteFlag!=1&&iWriteFlag!=2) iWriteFlag=0;
			break;
		case 't':
			iDelayMilSec=atoi(optarg);
			break;
		case 'm':
			iMultiTimes=atoi(optarg);
			if(iMultiTimes<=0) iMultiTimes=1;
			break;
		case 's':
			strcpy(sSourcePath, optarg);
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-r disp-name ]\n");
			printf("   [-w (1,writegta,2 writetdf,other nowrite) ]\n");
			printf("   [-t (query delay mil sec def=100ms) ]\n");
			printf("   [-m (multi_times) ]\n");
			printf("   [-s (source-path) ]\n");
			exit(1);
			break;
		}
	}

	//初始化刷新数组，以及刷新文件名的全局变量
	InitUserArray(sDispName,&R);

	//刷新一下参数，避免要求disp先启动，才能启动本程序
	RefreshUserArray(sDispName,&R);

	IoService	ios;
	//订阅消息回调类
	CallBackBase CallbackBase(iWriteFlag,strWork);
	CallbackBase.SetIoService(&ios);

	//启动处理数据服务
	ios.Start();


	//启动一个线程查询数据，并将数据加到工作线程中
	pCallBack=	&CallbackBase;
		
	pthread_t pthd_qh;
	pthread_attr_t attr_qh;

	//加载qh回放线程
	pthread_attr_init(&attr_qh);
	pthread_attr_setdetachstate(&attr_qh, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr_qh, 1024*512);
	pthread_create(&pthd_qh, NULL, MainReplayRunQh, NULL);

	printf("-----------------------------1.\n");
	
	//循环监视disp规则变化，如果变化则通知刷新
	WatchFileCloseWriteAndLock(sDispName);
	
	printf("-----------------------------2.\n");

	//正常代码不会运行到这里
	while(1){//主线程不能退出
		sleep(1);
	}

	//退出工作线程
	ios.Stop();

	getchar();

	return 0;
}

void *MainReplayRunQh(void *)
{
	int iCount=0;
	char sInFileName[1024],sBuffer[10240];
	UTIL_Time sStartTime;
	long lItemLen=0;
	long long lStartTime,lDiffTime,lCurTime;
	
	FILE *fpIn;
	
	SSEL2_Quotation *p=(SSEL2_Quotation*)(sBuffer+sizeof(long long));	

	sprintf(sInFileName,"%s/gta_qh.dat",sSourcePath);
	
	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		exit(1);
	}
	
	lItemLen=(sizeof(SSEL2_Quotation))+sizeof(long long);

	
	//确定开始时间
	PUTIL_GetLocalTime(&sStartTime);
	
	//将这个时间确定为9:30分
	lStartTime=	PUTIL_SystemTimeToDateTime(&sStartTime);
	lStartTime=	lStartTime%MY_DATE_CEIL_LONG;
	lDiffTime=	lStartTime-  93000000L;

	while(1){
		
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,lItemLen,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}
		if(feof(fpIn)) break;
__delay:
		UTIL_Time stTime;
		PUTIL_GetLocalTime(&stTime);
		lCurTime=PUTIL_SystemTimeToDateTime(&stTime);

		if(p->Time>(lCurTime%MY_DATE_CEIL_LONG-lDiffTime)){
			//休眠指定的时长
			usleep(iDelayMilSec*1000);
			goto __delay;
		}

		//发送multi-times次数据
		for(int j=0;j<iMultiTimes;j++){
				                        	
			//接收到数据后，先放入本地队列，等待数据处理接口处理
			SubData *subdata = new SubData;
			subdata->msgType = Msg_SSEL2_Quotation;
			subdata->cur_time = lCurTime;
			subdata->data.assign((const char*)p, sizeof(SSEL2_Quotation));
                        	
			TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message_SSEL2_Quotation,pCallBack, subdata)));
                        	
			pCallBack->m_ios->Post(task);
			
			if((++iCount)%300==0)
				printf("qh stock rt=%d,ct=%lld cur process count =%d.\n",
					p->Time,lCurTime%MY_DATE_CEIL_LONG,iCount);
		}
	}

	fclose(fpIn);
	
	return NULL;
}

