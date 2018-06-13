
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "wwtiny.h"
#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"

#include "gta_supp.h"

#include "dmpfmt.h"

int (*print_MY_TYPE)(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);

void InitFunctionFromType(int iType,long *plItemLen)
{
	long lItemLen;

	switch (iType){
		case MY_TYPE_SSEL2_Quotation	:
		lItemLen=(sizeof(SSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Quotation;
		break;
		case MY_TYPE_SSEL2_Transaction:
		lItemLen=(sizeof(SSEL2_Transaction));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Transaction;
		break;
		case MY_TYPE_SSEL2_Auction:
		lItemLen=(sizeof(SSEL2_Auction));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Auction;
		break;
		case MY_TYPE_SZSEL2_Quotation:
		lItemLen=(sizeof(SZSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Quotation;
		break;
		case MY_TYPE_SZSEL2_Transaction:
		lItemLen=(sizeof(SZSEL2_Transaction));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Transaction;
		break;
		case MY_TYPE_SZSEL2_Order:
		lItemLen=(sizeof(SZSEL2_Order));
		print_MY_TYPE=print_MY_TYPE_SZSEL2_Order;
		break;
		case MY_TYPE_TDF_MKT:
		lItemLen=(sizeof(TDF_MARKET_DATA));
		print_MY_TYPE=print_MY_TYPE_TDF_MKT;
		break;
		case MY_TYPE_TDF_TRA:
		lItemLen=(sizeof(TDF_TRANSACTION));
		print_MY_TYPE=print_MY_TYPE_TDF_TRA;
		break;
		case MY_TYPE_TDF_ORD:
		lItemLen=(sizeof(TDF_ORDER));
		print_MY_TYPE=print_MY_TYPE_TDF_ORD;
		break;
		case MY_TYPE_TDF_QUE:
		lItemLen=(sizeof(TDF_ORDER_QUEUE));
		print_MY_TYPE=print_MY_TYPE_TDF_QUE;
		break;
		case MY_TYPE_D31_STD:
		lItemLen=(sizeof(struct D31ItemStruct));
		print_MY_TYPE=print_MY_TYPE_D31_STD;
		break;
		case MY_TYPE_D31_EXT:
		lItemLen=(sizeof(struct D31ItemStruct));
		print_MY_TYPE=print_MY_TYPE_D31_EXT;
		break;
		default:
		lItemLen=(sizeof(SSEL2_Quotation));
		print_MY_TYPE=print_MY_TYPE_SSEL2_Quotation;
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
	printf("   [-t type (1-qh,2-th,3-ah,4-qz,5-tz,6-oz,7-dstd,8-dext,mtoq|MTOQ=>mkt,trs,ord,que) ]\n");
	printf("   [-d delay sec (-e,-l multi-code effect) ]\n");
	printf("   [-l codelist (e.g \"000001,603912,002415\") ]\n");
	printf("   [-f time_flag (def=1,1-createtime,2-picktime,3-localtime,4-packtime) ]\n");
}

int main(int argc, char *argv[])
{
	int iType=1,ret,i=0,iSkipCnt=0,iFilterCnt=0,iMaxCount=100;
	int iExceedCnt=0,iDelayCnt=0,iDelaySec=10,iTimeFlag=1;
	long lBgnTime=0,lEndTime=999999999999,lCurTime=0,lItemLen=0;

	char sInFileName[1024],sCodeStr[1024],sBuffer[10240],sOutBuf[10240];

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
			iType=atoi(optarg);
			if(iType<=0||iType>8) iType=1;
			//增加TDF的四种类型
			switch (optarg[0]){
				case 'm':case 'M':iType='M';break;
				case 't':case 'T':iType='T';break;
				case 'o':case 'O':iType='O';break;
				case 'q':case 'Q':iType='Q';break;
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

	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		return -1;
	}
	
	//根据类型，绑定函数指针到特定的处理函数
	InitFunctionFromType(iType,&lItemLen);
	
	lItemLen+=sizeof(long long);

	if(fseek(fpIn,lItemLen*iSkipCnt,SEEK_SET)<0){
		perror("seek file");
		return -1;
	}

	while(1){
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,lItemLen,1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}

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
