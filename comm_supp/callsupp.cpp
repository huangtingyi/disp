#include <stdio.h>
#include <atomic>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#include <boost/property_tree/json_parser.hpp>

#include "MessageQueue.h"

#include "wwtiny.h"
#include "callsupp.h"


//atomic_t VLock=ATOMIC_INIT(0);

int VLock=0;


struct DispRuleStruct R,T;
struct UserStruct *pRAll=NULL,*pTAll=NULL;
char sRefreshDispName[1024];

int iMaxMqCnt=0,iSendCnt=0;;
MessageQueue *ARRAY_MQ[MAX_CLIENT_CNT];


void InsertFreeList(struct UserStruct **pptHead,struct UserStruct *pTemp)
{
	pTemp->pFreeNext=*pptHead;
	*pptHead=pTemp;
}
void LockWorkThread()
{
	//atomic_inc(&VLock);
	VLock=1;
}
void UnLockWorkThread()
{
	//atomic_dec(&VLock);
	VLock=0;
}
int IsWorkThreadLock()
{
	//return atomic_read(&VLock);
	return VLock;
}

/****
#define EVENT_NUM 12

char *event_str[EVENT_NUM] =
{
    "IN_ACCESS",
    "IN_MODIFY",
    "IN_ATTRIB",
    "IN_CLOSE_WRITE",
    "IN_CLOSE_NOWRITE",
    "IN_OPEN",
    "IN_MOVED_FROM",
    "IN_MOVED_TO",
    "IN_CREATE",
    "IN_DELETE",
    "IN_DELETE_SELF",
    "IN_MOVE_SELF"
};
***/

char *GetEventStrInfo(int mask)
{
	char *p;

	switch (mask){
	case 0x01: p=(char*)"IN_ACCESS";	break;
	case 0x02: p=(char*)"IN_MODIFY";       break;
	case 0x04: p=(char*)"IN_ATTRIB";       break;
	case 0x08: p=(char*)"IN_CLOSE_WRITE";  break;
	case 0x10: p=(char*)"IN_CLOSE_NOWRITE";break;
	case 0x20: p=(char*)"IN_OPEN";         break;
	case 0x40: p=(char*)"IN_MOVED_FROM";   break;
	case 0x80: p=(char*)"IN_MOVED_TO";     break;
	case 0x100:p=(char*)"IN_CREATE";       break;
	case 0x200:p=(char*)"IN_DELETE";       break;
	case 0x400:p=(char*)"IN_DELETE_SELF";  break;
	case 0x800:p=(char*)"IN_MOVE_SELF";    break;
	default:break;
	}
	return p;
}

int WatchFileCloseWriteAndLock(char sFileName[])
{
	int fd,len,i;
	char buf[BUFSIZ],sHostTime[15],sMilli[4];
	struct inotify_event *event;

	if((fd = inotify_init())<0){
		fprintf(stderr, "inotify_init failed\n");
		return -1;
	}

	if(inotify_add_watch(fd, sFileName, IN_ALL_EVENTS)<0){
		fprintf(stderr, "inotify_add_watch %s failed\n", sFileName);
		return -1;
	}

	buf[sizeof(buf) - 1] = 0;
	while( (len = read(fd, buf, sizeof(buf) - 1)) > 0 ){

		printf("-----------------------------3 l=%d.\n",len);

		for(i=0;i<len;i+=sizeof(struct inotify_event)){

			event = (struct inotify_event *)&buf[i];

			GetHostTimeX(sHostTime,sMilli);
			printf("%s:%s %s --- %s\ti=%d,m=%d,mi=%s l=%d\n",sHostTime,sMilli," ", "",
				i,event->mask,GetEventStrInfo(event->mask),len);

			//如果不是WRITE_CLOSE事件则继续
			if((event->mask & 0x8)==0) continue;

			printf("catch WRITE-ON-CLOSE EVENT.\n");

			//锁定变量
			LockWorkThread();
		}
	}
	return 0;
}


MessageQueue *GetMqArray(int iMqId,int *pIndex)
{
	MessageQueue *p;

	*pIndex=-1;

	for(int i=0;i<iMaxMqCnt;i++){
		p=ARRAY_MQ[i];
		if(iMqId==p->m_oriKey){
			*pIndex=i;
			return p;
		}
	}
	return NULL;
}
int AddMqArray(MessageQueue *q)
{
	int iPos=iMaxMqCnt;

	ARRAY_MQ[iPos]=q;

	iMaxMqCnt++;
	return iPos;
}
void InitUserArray(char sDispName[],struct DispRuleStruct *p)
{
/*将映射表清空*/
	for(int i=0;i<MAX_STOCK_CODE;i++){
		p->AMUSER[i]=NULL;
		p->ATUSER[i]=NULL;
		p->AQUSER[i]=NULL;
		p->AOUSER[i]=NULL;
		p->ADUSER[i]=NULL;
	}
	p->PMALL=p->PTALL=p->PQALL=p->POALL=p->PDALL=NULL;
	strcpy(sRefreshDispName,sDispName);
}
void DeleteUserList(struct UserStruct *ptHead)
{
	struct UserStruct *pTemp;
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;
		delete pTemp;
	}
}
void AssignDispRule(struct DispRuleStruct *p,struct DispRuleStruct *pi)
{
	int i;	
	struct UserStruct *ptHead,*pTemp;
	
	ptHead=pRAll;
	
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pFreeNext;
		
		
		//按股票订购

		if((i=pTemp->iStockCode)!=-1){
			switch(pTemp->iSubscribed){
			case 12:p->AMUSER[i]=NULL;break;
			case 13:p->ATUSER[i]=NULL;break;
			case 14:p->AQUSER[i]=NULL;break;
			case 15:p->AOUSER[i]=NULL;break;
			case 18:p->ADUSER[i]=NULL;break;
			default:p->AMUSER[i]=NULL;break;
			}
		}
		free(pTemp);
	}
	
	//将临时链表更新到实时链表中
	pRAll=pTAll;
	pTAll=NULL;
	
	//根据实时链表设置相应参数
	ptHead=pRAll;
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pFreeNext;
		
		if((i=pTemp->iStockCode)!=-1){
			switch(pTemp->iSubscribed){
			case 12:p->AMUSER[i]=pi->AMUSER[i];break;
			case 13:p->ATUSER[i]=pi->ATUSER[i];break;
			case 14:p->AQUSER[i]=pi->AQUSER[i];break;
			case 15:p->AOUSER[i]=pi->AOUSER[i];break;
			case 18:p->ADUSER[i]=pi->ADUSER[i];break;
			default:p->AMUSER[i]=NULL;break;
			}
		}		
	}
	p->PMALL=pi->PMALL;
	p->PTALL=pi->PTALL;
	p->PQALL=pi->PQALL;
	p->POALL=pi->POALL;
	p->PDALL=pi->PDALL;

}
void FreeDispRule(struct DispRuleStruct *p)
{
	for(int i=0;i<MAX_STOCK_CODE;i++){
		DeleteUserList(p->AMUSER[i]); p->AMUSER[i]=NULL;
		DeleteUserList(p->ATUSER[i]); p->ATUSER[i]=NULL;
		DeleteUserList(p->AQUSER[i]); p->AQUSER[i]=NULL;
		DeleteUserList(p->AOUSER[i]); p->AOUSER[i]=NULL;
		DeleteUserList(p->ADUSER[i]); p->ADUSER[i]=NULL;
	}
	DeleteUserList(p->PMALL);  p->PMALL=NULL;
	DeleteUserList(p->PTALL);  p->PTALL=NULL;
	DeleteUserList(p->PQALL);  p->PQALL=NULL;
	DeleteUserList(p->POALL);  p->POALL=NULL;
	DeleteUserList(p->PDALL);  p->PDALL=NULL;

}
void RefreshUserArray(char sDispName[],struct DispRuleStruct *p)
{
	int iSubscribed,iStockCode;
	char sHostTime[15],sMilli[4];
	struct UserStruct **AUSER,*pTemp,**PPALL;

	boost::property_tree::ptree tRoot,tMainRoot,t,tSubscribed,tSubcodes;
	string user,mqid;

	GetHostTimeX(sHostTime,sMilli);
	printf("%s:%s refresh BEGIN\n",sHostTime,sMilli);

/*读取disp.json文件,先检查一下格式是否完整，不完整就不刷新*/
	try {
		boost::property_tree::read_json(sDispName,tMainRoot);
		tRoot=tMainRoot.get_child("users");
	}
	catch (...) {
		printf("检测到不完整的 disprule文件，文件名:%s.\n",sDispName);
		return;
	}

/*将映射表清空*/

	InitUserArray(sRefreshDispName,&T);

	for (auto it = tRoot.begin(); it != tRoot.end(); ++it) {

		auto each = it->second;

		try{
			user = 	each.get<string>("user");
			mqid =	each.get<string>("mqid");
			tSubscribed = each.get_child("subscribed");
			tSubcodes = each.get_child("subcodes");
		}
		catch (...){
			//如果某个用户信息不全，则认为这个用户为没有订购
			GetHostTimeX(sHostTime,sMilli);
			printf("%s:%s watch exception",sHostTime,sMilli);
			continue;
		}

                iStockCode=-1;

		for (auto i = tSubcodes.begin(); i != tSubcodes.end(); ++i) {


			iStockCode=i->second.get_value<int>();

			/*过滤无效的证券代码*/
			if(iStockCode<=0||iStockCode>MAX_STOCK_CODE) continue;

			for (auto j = tSubscribed.begin(); j != tSubscribed.end(); ++j) {

				iSubscribed=j->second.get_value<int>();

				GetHostTimeX(sHostTime,sMilli);

	printf("%s:%s user:%s subscribed:%d stockcode:%06d hello world.--------------------------------------------5.zzzz.\n",
		sHostTime,sMilli,user.c_str(),iSubscribed,iStockCode);

				switch(iSubscribed){
				case 12:AUSER=&T.AMUSER[0];break;
				case 13:AUSER=&T.ATUSER[0];break;
				case 14:AUSER=&T.AQUSER[0];break;
				case 15:AUSER=&T.AOUSER[0];break;
				case 18:AUSER=&T.ADUSER[0];break;
				default:AUSER=NULL;
				break;
				}
				//过滤有效订购代码
				if(AUSER==NULL)continue;

				pTemp=new (struct UserStruct);

				if(pTemp==NULL){
					printf("error new struct UserStruct.\n");
					exit(1);
				}

				pTemp->pNext=NULL;
				strncpy(pTemp->sUserName,user.c_str(),sizeof(pTemp->sUserName)-1);
				pTemp->sUserName[sizeof(pTemp->sUserName)-1]=0;

				pTemp->iMqId=	atoi(mqid.c_str());

				pTemp->iMqPos=-1;
				
				pTemp->iStockCode=iStockCode;
				pTemp->iSubscribed=iSubscribed;
				/*插入到表中*/
				pTemp->pNext=AUSER[iStockCode];
				AUSER[iStockCode]=pTemp;
				
				//插入到pFreeList链表中
				InsertFreeList(&pTAll,pTemp);
                	}
                }

                if(iStockCode==-1){

			for (auto i = tSubscribed.begin(); i != tSubscribed.end(); ++i) {

				iSubscribed=i->second.get_value<int>();

				GetHostTimeX(sHostTime,sMilli);

	printf("%s:%s user:%s subscribed:%d hello world.--------------------------------------------5.yyyy.\n",
		sHostTime,sMilli,user.c_str(),iSubscribed);

				switch(iSubscribed){
				case 12:PPALL=&T.PMALL;break;
				case 13:PPALL=&T.PTALL;break;
				case 14:PPALL=&T.PQALL;break;
				case 15:PPALL=&T.POALL;break;
				case 18:PPALL=&T.PDALL;break;
				default:PPALL=NULL;
				}
				//过滤无效订购代码
				if(PPALL==NULL)continue;

				pTemp=new (struct UserStruct);

				if(pTemp==NULL){
					printf("error new struct UserStruct.\n");
					exit(1);
				}

				pTemp->pNext=NULL;
				strncpy(pTemp->sUserName,user.c_str(),sizeof(pTemp->sUserName)-1);
				pTemp->sUserName[sizeof(pTemp->sUserName)-1]=0;

				pTemp->iMqId=	atoi(mqid.c_str());
				pTemp->iMqPos=-1;
				
				pTemp->iStockCode=-1;
				pTemp->iSubscribed=iSubscribed;

				/*插入到表中*/
				pTemp->pNext=*PPALL;
				*PPALL=pTemp;
				
				//插入到pFreeList链表中
				InsertFreeList(&pTAll,pTemp);
			}
                }

	}

	GetHostTimeX(sHostTime,sMilli);
	printf("%s:%s refresh ASSGIN BEGIN\n",sHostTime,sMilli);

	AssignDispRule(p,&T);

	GetHostTimeX(sHostTime,sMilli);
	printf("%s:%s refresh END\n",sHostTime,sMilli);
}
int SendMsg2Mq(string &str,struct UserStruct *pCli)
{
	MessageQueue *mq;
	char sHostTime[15],sMilli[4];

	if(pCli->iMqPos==-1){//如果没有MQ地址则到缓存中找

		if((mq=GetMqArray(pCli->iMqId,&pCli->iMqPos))==NULL){

			//若缓存中没有这个MQ，则NEW一个并OPEN它，保存到缓存里面
			if((mq=new MessageQueue(pCli->iMqId))==NULL){
				printf("new MessageQueue error.\n");
				exit(1);
			}
			mq->open(false,false,512,15000);
			pCli->iMqPos=AddMqArray(mq);
		}
	}
	else
		mq=	ARRAY_MQ[pCli->iMqPos];

	if(mq->send(str,0)==(int)(str.length())) iSendCnt++;
	else{
		GetHostTimeX(sHostTime,sMilli);
		printf("%s:%s send error user=%s pos =%d.\n",
			sHostTime,sMilli,pCli->sUserName,iSendCnt);
	}

	if((iSendCnt%50000)==0){
		GetHostTimeX(sHostTime,sMilli);		
		printf("%s:%s user=%s send count =%d.\n",
			sHostTime,sMilli,pCli->sUserName,iSendCnt);

	}

	return 0;
}
void SendMsg2Cli(int iStockCode,char cType,string& str)
{
	char sBuffer[4];
	unsigned int len,l0,l1;
	struct UserStruct *pAll,*pUser;

	string str1;

//	printf("hello world.--------------------------------------------3.5.\n");

	if(IsWorkThreadLock()){

		printf("hello world.--------------------------------------------4.\n");
		RefreshUserArray(sRefreshDispName,&R);
		UnLockWorkThread();
	}

	/*取字节长度，并判断最大数值*/
	len=str.length()+1; if(len>10230) len=10230;

	l0=len%256;l1=len/256;

	((unsigned char*)sBuffer)[0]=l1;
	((unsigned char*)sBuffer)[1]=l0;

	switch (cType){
	case 'M': pUser=R.AMUSER[iStockCode];pAll=R.PMALL;sBuffer[2]=12;break;
	case 'T': pUser=R.ATUSER[iStockCode];pAll=R.PTALL;sBuffer[2]=13;break;
	case 'Q': pUser=R.AQUSER[iStockCode];pAll=R.PQALL;sBuffer[2]=14;break;
	case 'O': pUser=R.AOUSER[iStockCode];pAll=R.POALL;sBuffer[2]=15;break;
	case 'D': pUser=R.ADUSER[iStockCode];pAll=R.PDALL;sBuffer[2]=18;break;
	default:  pUser=R.AQUSER[iStockCode];pAll=R.PQALL;sBuffer[2]=14;break;
	break;
	}

	str1=string(sBuffer,3)+str;

//	strncpy(sBuffer+3,str.c_str(),str.length());

	while(pUser!=NULL){
		SendMsg2Mq(str1,pUser);
		pUser=pUser->pNext;
	}

	while(pAll!=NULL){
		SendMsg2Mq(str1,pAll);
		pAll=pAll->pNext;
	}
}

bool ValidShStockCode(char sStockCode[])
{
	if (sStockCode[8] == 'S'&& 
		((sStockCode[0] == '6' && sStockCode[1] == '0')||
		(sStockCode[0] == '1' && sStockCode[1] == '1'))) return true;
	return false;
}

bool ValidSzStockCode(char sStockCode[])
{
	if (sStockCode[8] == 'Z'&& 
		((sStockCode[0] == '3' && sStockCode[1] == '0')||
		(sStockCode[0] == '0' && sStockCode[1] == '0')||
		(sStockCode[0] == '1' && sStockCode[1] == '2'))) return true;
	return false;
}

bool ValidStockCode(char szWinCode[])
{
	if(ValidShStockCode(szWinCode)||ValidSzStockCode(szWinCode)) return true;
	
	return false;
}
