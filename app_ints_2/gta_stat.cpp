
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>


#include "QTSDataType.h"
#include "QTSStruct.h"
#include "QTSDataFieldDefine.h"


int main(int argc, char *argv[])
{
	int iType,i=0;
	char sInFileName[1024],sOutFileName[1024];
	char sBuffer[10240];
	
	SSEL2_Quotation *p=(SSEL2_Quotation *)(sBuffer+sizeof(long long));
	long long *pt=(long long *)sBuffer;
	
	FILE *fpIn,*fpOut;	
	
	for (int c; (c = getopt(argc, argv, "i:o:t:")) != EOF;){

		switch (c){
		case 'i':
			strcpy(sInFileName, optarg);
			break;
		case 'o':
			strcpy(sOutFileName, optarg);
			break;
		case 't':
			iType=atoi(optarg);
			if(iType<=0||iType>6) iType=1;
			break;
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-i infile-name ]\n");
			printf("   [-o outfile-name ]\n");
			printf("   [-t type (1-qh,2-th,3-ah,4-qz,5-tz,6-oz) ]\n");
			exit(1);
			break;
		}
	}


	if((fpIn=fopen(sInFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sInFileName);
		return -1;
	}
/*
	if((fpOut=fopen(sOutFileName,"w"))==NULL)){
		printf("error open file %s to write.\n",sOutFileName
		return -1;
	}
*/	
	while(1){
		//size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
		if(fread((void*)sBuffer,sizeof(SSEL2_Quotation)+sizeof(long long),1,fpIn)!=1){
			printf("end of file break.\n");
			break;
		}
		printf("t1=%lld\tt2=%\lld\tt3=%d\tcd=%s.\n",*pt,p->PacketTimeStamp,p->Time,p->Symbol);
		
		i++;
		
		if(i>100) break;
		
	}
	
	fclose(fpIn);
//	fclose(fpOut);
	
	return 0;
}
