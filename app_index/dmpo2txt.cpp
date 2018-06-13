
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "wwtiny.h"
#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#include "TDFAPIStruct.h"

#include <iostream>
#include <string>
using namespace std;

#include "mktdata.pb.h"

#include "dmpfmt.h"

int (*convert_MY_TYPE)(char *pi,long lRecLen,char *po);
int (*print_MY_TYPE)(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);

#define MY_GET_REC_LEN(p)  (((unsigned char*)p)[8]*256L+((unsigned char*)p)[9])

char MyGetRecType(char *p)
{
	static char *TypeMapStr=(char*)"MTQO";

	if(p[10]>=12&&p[10]<=15)
		return TypeMapStr[p[10]-12];

	if(p[0]==18) return 'D';

	return 'M';
}
int MyMatchRecType(char *p,int iType)
{
	switch (iType){
	case 'M':
		if(p[0]==12) return 1;
	break;
	case 'T':
		if(p[0]==13) return 1;
	break;
	case 'Q':
		if(p[0]==14) return 1;
	break;
	case 'O':
		if(p[0]==15) return 1;
	break;
	case 7:
	case 8:
		if(p[0]==18) return 1;
	break;
	default:
	break;
	}
	return 0;
}
long lGetSkipPos(char sFileName[],int iSkipCnt)
{
	long lCurPos=0;
	FILE *fp;
	char sBuf[16];
	unsigned char *p=(unsigned char *)(sBuf+8);

	if(iSkipCnt<=0) return 0L;

	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1L;
	}

	while(iSkipCnt>0){
		if(fread((void*)sBuf,10,1,fp)!=1)
			break;

		lCurPos+=(long)(p[0]*256L+p[1]);

		if(fseek(fp,lCurPos,SEEK_SET)<0){
			perror("seek file");
			fclose(fp);
			return -1;
		}
		iSkipCnt--;
	}

	fclose(fp);

	return lCurPos;
}
void InitFunctionFromType(int iType,long *plItemLen)
{
	long lItemLen;

	switch (iType){
		case MY_TYPE_TDF_MKT:
		lItemLen=(sizeof(TDF_MARKET_DATA));
		print_MY_TYPE=print_MY_TYPE_TDF_MKT;
		convert_MY_TYPE=convert_MY_TYPE_TDF_MKT;
		break;
		case MY_TYPE_TDF_TRA:
		lItemLen=(sizeof(TDF_TRANSACTION));
		print_MY_TYPE=print_MY_TYPE_TDF_TRA;
		convert_MY_TYPE=convert_MY_TYPE_TDF_TRA;
		break;
		case MY_TYPE_TDF_ORD:
		lItemLen=(sizeof(TDF_ORDER));
		print_MY_TYPE=print_MY_TYPE_TDF_ORD;
		convert_MY_TYPE=convert_MY_TYPE_TDF_ORD;
		break;
		case MY_TYPE_TDF_QUE:
		lItemLen=(sizeof(TDF_ORDER_QUEUE));
		print_MY_TYPE=print_MY_TYPE_TDF_QUE;
		convert_MY_TYPE=convert_MY_TYPE_TDF_QUE;
		break;
		case MY_TYPE_D31_STD:
		lItemLen=(sizeof(struct D31ItemStruct));
		print_MY_TYPE=print_MY_TYPE_D31_STD;
		convert_MY_TYPE=convert_MY_TYPE_D31_ITEM;
		break;
		case MY_TYPE_D31_EXT:
		lItemLen=(sizeof(struct D31ItemStruct));
		print_MY_TYPE=print_MY_TYPE_D31_EXT;
		convert_MY_TYPE=convert_MY_TYPE_D31_ITEM;
		break;
		default:
			lItemLen=(sizeof(TDF_MARKET_DATA));
			print_MY_TYPE=print_MY_TYPE_TDF_MKT;
			convert_MY_TYPE=convert_MY_TYPE_TDF_MKT;
		break;
	}
	*plItemLen=lItemLen;
}
void PrintUsage(char *argv[])
{
	printf("Usage: %s \n", argv[0]);
	printf("   [-i infile-name ]\n");
	printf("   [-s skipcnt (def=0)]\n");
	printf("   [-c count (def=100,-1=all) ]\n");
	printf("   [-b begin-time (def=0) ]\n");
	printf("   [-e end-time (def=99999999999) ]\n");
	printf("   [-t type (mtoq|MTOQ=>mkt,trs,ord,que,7,8 =>d31std,d31ext) ]\n");
	printf("   [-d delay sec (-e,-l multi-code effect) ]\n");
	printf("   [-l codelist (e.g \"000001,603912,002415\") ]\n");
	printf("   [-f time_flag (def=1,1-createtime,2-picktime) ]\n");
}

int main(int argc, char *argv[])
{
	int iType=1,ret,i=0,iSkipCnt=0,iFilterCnt=0,iMaxCount=100;
	int iExceedCnt=0,iDelayCnt=0,iDelaySec=10,iTimeFlag=1;
	long lBgnTime=0,lEndTime=999999999999,lCurTime=0,lItemLen=0,lCurPos,lRecLen;

	char sInFileName[1024],sCodeStr[1024],sBuffer[1024],sProtoBuf[1024],sOutBuf[10240];

	FILE *fpIn;

	for (int c; (c = getopt(argc, argv, "i:s:c:b:e:l:t:d:f:")) != EOF;){

		switch (c){
		case 'i':strcpy(sInFileName, optarg);	break;
		case 's':iSkipCnt=atoi(optarg);		break;
		case 'c':iMaxCount=atoi(optarg);	break;
		case 'b':lBgnTime=atol(optarg);		break;
		case 'e':lEndTime=atol(optarg);		break;
		case 'l':strcpy(sCodeStr, optarg);	break;
		case 't':
			//增加TDF的四种类型
			switch (optarg[0]){
				case 'm':case 'M':iType='M';break;
				case 't':case 'T':iType='T';break;
				case 'o':case 'O':iType='O';break;
				case 'q':case 'Q':iType='Q';break;
				case '7':case '8':iType=optarg[0]-'0';break;
				default:iType='M';break;
			}
			break;
		case 'd':iDelaySec=atoi(optarg);	break;
		case 'f':iTimeFlag=atoi(optarg);	break;
		default:
			PrintUsage(argv);
			exit(1);
			break;
		}
	}

	if(argc==1){
		PrintUsage(argv);
		exit(1);
	}

	//根据类型，绑定函数指针到特定的处理函数
	InitFunctionFromType(iType,&lItemLen);

	lItemLen+=sizeof(long long);

	if((lCurPos=lGetSkipPos(sInFileName,iSkipCnt))<0) return -1;


	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		return -1;
	}

	if(fseek(fpIn,lCurPos,SEEK_SET)<0){
		perror("seek file");
		return -1;
	}

	while(1){
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,11,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}

		//拿到记录长度
		lRecLen=MY_GET_REC_LEN(sBuffer)-1;
		
		if(MyMatchRecType(sBuffer,iType)==0){
				
			if(fseek(fpIn,lRecLen,SEEK_CUR)<0){
				perror("seek file");
				return -1;
			}
			iSkipCnt++;
			continue;
		}

		if(fread((void*)sProtoBuf,lRecLen,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}
		//将protobuf格式解码为二进制结构
		if(convert_MY_TYPE(sProtoBuf,lRecLen,sBuffer+8)<0){
			perror("seek file");
			return -1;

		}
		//将整理好的格式，转换为文本
		if((ret=print_MY_TYPE(sBuffer,sCodeStr,iTimeFlag,lBgnTime,&lCurTime,sOutBuf))!=0){
			if(ret==1)	iFilterCnt++;
			else		iSkipCnt++;
			continue;
		}
		//如果发现所选的股票的时间大于命令参数指定时间
		if(lCurTime>lEndTime){

			//如果只选一只股票，则直接break
			if(strlen(sCodeStr)==6) break;

			//如果选多支股票，或全选，则容许跳过10秒,
			//为了避免因为一只股票超时，而导致其他股票的endtime之前的数据没取了
			if(lCurTime>(lEndTime+iDelaySec*1000L)) break;

			iExceedCnt++;
			continue;
		}
		printf("%s",sOutBuf);

		//如果发现股票的时间到达了，统计一下超过这个时间之后的还有多少条选中信息
		if(iExceedCnt>0) iDelayCnt++;

		if((++i)>=iMaxCount) break;

	}

	printf("total skip cnt = %d filter cnt =%d exceed cnt =%d delay cnt =%d output cnt =%d.\n",
		iSkipCnt,iFilterCnt,iExceedCnt,iDelayCnt,i);

	fclose(fpIn);

	return 0;
}
