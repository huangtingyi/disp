#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "wwtiny.h"

char ParseToken[PARSE_ARRAY_LEN][PARSE_ITEM_LEN];

/*  *pterrcode==0 means no error happen */
time_t tGetTime(char sAnswerTime[15])
{
	int iMaxDays=0;
	char sTempYear[5],sTempMon[3],sTempMDay[3],sTempHour[3],
	     sTempMin[3],sTempSec[3];
	struct tm s;
	time_t t_ret;

	bzero(&s,sizeof(struct tm));

	strncpy(sTempYear,sAnswerTime,4);sTempYear[4]=0;
	sTempMon[0]=sAnswerTime[4];
	sTempMon[1]=sAnswerTime[5]; sTempMon[2]=0;

	sTempMDay[0]=sAnswerTime[6];
	sTempMDay[1]=sAnswerTime[7]; sTempMDay[2]=0;

	sTempHour[0]=sAnswerTime[8];
	sTempHour[1]=sAnswerTime[9]; sTempHour[2]=0;

	sTempMin[0]=sAnswerTime[10];
	sTempMin[1]=sAnswerTime[11]; sTempMin[2]=0;

	sTempSec[0]=sAnswerTime[12];
	sTempSec[1]=sAnswerTime[13]; sTempSec[2]=0;

	s.tm_year=atoi(sTempYear)-1900;
	if(s.tm_year<=0)	return -1;

	s.tm_mon=atoi(sTempMon)-1;
	if(s.tm_mon<0||s.tm_mon>11) return -1;

	switch (s.tm_mon+1){
	case 1:	iMaxDays=31;break;
	case 2: iMaxDays=28;
		if(LeapYear(sAnswerTime)) iMaxDays++;
	break;
	case 3: iMaxDays=31;break;
	case 4: iMaxDays=30;break;
	case 5: iMaxDays=31;break;
	case 6: iMaxDays=30;break;
	case 7: iMaxDays=31;break;
	case 8: iMaxDays=31;break;
	case 9: iMaxDays=30;break;
	case 10:iMaxDays=31;break;
	case 11:iMaxDays=30;break;
	case 12:iMaxDays=31;break;
	}
	
	s.tm_mday=atoi(sTempMDay);
	if(s.tm_mday<1||s.tm_mday>iMaxDays) return -1;

	s.tm_hour=atoi(sTempHour);
	if(s.tm_hour<0||s.tm_hour>23) return -1;

	s.tm_min=atoi(sTempMin);
	if(s.tm_min<0||s.tm_min>59) return -1;

	s.tm_sec=atoi(sTempSec);
	if(s.tm_sec<0||s.tm_sec>59) return -1;

	s.tm_isdst=0;

	t_ret=mktime(&s);

	if(t_ret==-1) return -1;

	return t_ret;
}
int sFormatTime( time_t ttime,char sTime[15])
{
	struct tm *tm;

	if((tm = (struct tm*)localtime(&ttime))==NULL) return -1;

	if(tm->tm_isdst == 1){
		ttime -= 3600;
		if((tm=(struct tm *)localtime(&ttime))==NULL)return -1;
	}

	if(strftime(sTime,15,"%Y%m%d%H%M%S",tm)==(size_t)0)return -1;

	return 0;
}
int ArcTime(time_t t,char sTime[15])
{
	return sFormatTime(t,sTime);
}
int ChkTime(char sAnswerTime[15])
{
	time_t tTime;
	char sTempDate[15];

	if((tTime=tGetTime(sAnswerTime))<0) return -1;
	
	if(ArcTime(tTime,sTempDate)<0) return -1;
	
	if(strcmp(sAnswerTime,sTempDate)!=0) return -1;
	
	return 0;
}
int ChkDate(char sDate[9])
{
	char sTempDate[15];
	
	strncpy(sTempDate,sDate,8);
	strcpy(sTempDate+8,"000000");
	
	return ChkTime(sTempDate);
}
int GetHostTime(char sHostTime[15])
{
	struct tm *tm;
	time_t	hosttime;

	time(&hosttime);

	if((tm=(struct tm*)localtime(&hosttime))==NULL) return -1;

	if(strftime(sHostTime,15,"%Y%m%d%H%M%S",tm)==(size_t)0)	return -1;

	return 0;
}

int GetHostTimeX(char sHostTime[15],char sMiniSec[4])
{
	struct tm *tm;
	struct timeval t;
	time_t tHostTime;

	strcpy(sMiniSec,"");

	if(gettimeofday(&t,NULL)==-1) return -1;

	tHostTime=(time_t)(t.tv_sec);
	if((tm=(struct tm*)localtime(&tHostTime))==NULL) return -1;

	if(strftime(sHostTime,15,"%Y%m%d%H%M%S",tm)==(size_t)0)	return -1;

	sprintf(sMiniSec,"%03ld",t.tv_usec/1000);

	return 0;
}
time_t tGetHostTime()
{
	char sHostTime[15];
	GetHostTime(sHostTime);
	return tGetTime(sHostTime);
}
int LeapYear(char sTempDate[15])
{
	int  iTempYear;
	char sTempYear[5];

	strncpy(sTempYear,sTempDate,4); sTempYear[4]=0;

	iTempYear=atoi(sTempYear);
	
	if((iTempYear%4)!=0) return FALSE;

	if((iTempYear%100)!=0) return TRUE;

	if((iTempYear%400)==0) return TRUE;

	return FALSE;

}
/* 将YYYYMMDDHH24MISS格式的字串增加iSecs秒(负值时，表示减) */
int AddTimes(char sTempDate[15], int iOffset)
{
	time_t t;

	if((t = tGetTime(sTempDate)) < 0)
		return -1;

	t = t + iOffset;

	return sFormatTime(t, sTempDate);
}

/* 将YYYYMMDDHH24MISS格式的字串增加iSecs秒(负值时，表示减) */
int AddMinutes(char sTempDate[15], int iOffset)
{
	return AddTimes(sTempDate,iOffset*60);
}

/* 将YYYYMMDDHH24MISS格式的字串增加iSecs秒(负值时，表示减) */
int AddHours(char sTempDate[15], int iOffset)
{
	return AddTimes(sTempDate,iOffset*60*60);
}
int AddDates(char sTempDate[15], int iOffset)
{
	return AddTimes(sTempDate,iOffset*86400);
}
int AddDays(char sTempDate[15], int iOffset)
{
	return AddTimes(sTempDate,iOffset*86400);
}
int AddWeeks(char sTempDate[15], int iOffset)
{
	return AddTimes(sTempDate,iOffset*86400*7);
}
int AddTen(char sTempDate[15])
{
	static int aiMonthDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	char sTempMonth[3];
	int	iTempMonth,iOffset;

	switch(sTempDate[7]){
	case '0':
	case '1':
		return AddTimes(sTempDate,86400*10);
	case '2':
		break;
	default:
		return -1;
	}

	strncpy(sTempMonth,sTempDate+4,2); 
	iTempMonth=atoi(sTempMonth);

	iOffset=aiMonthDays[iTempMonth-1]-20;

	if(iTempMonth==2){
		if(LeapYear(sTempDate)) iOffset++;
	}

	return AddTimes(sTempDate,iOffset*86400);
}
int AddMonths(char sTempDate[15], int iMonths)
{
	int iModx=0, iMody=0;
	char sTempYear[5],sTempMon[3],sTempMDay[3], sTemp[15];
	struct tm s;
	time_t t_ret;

	strcpy(sTemp, sTempDate);
	bzero(&s,sizeof(struct tm));

	strncpy(sTempYear,sTempDate,4);
	sTempYear[4]=0;

	sTempMon[0]=sTempDate[4];
	sTempMon[1]=sTempDate[5];
	sTempMon[2]=0;

	sTempMDay[0]=sTempDate[6];
	sTempMDay[1]=sTempDate[7];
	sTempMDay[2]=0;

	s.tm_year=atoi(sTempYear)-1900;
	if(s.tm_year<=0) return -1;

	s.tm_mon=atoi(sTempMon)-1;
	if(s.tm_mon<0||s.tm_mon>11) return -1;

	s.tm_mday=atoi(sTempMDay);
	if(s.tm_mday<0||s.tm_mday>31) return -1;

	s.tm_isdst=0;

	iModx = (s.tm_mon + iMonths) % 12;
	iMody = (s.tm_mon + iMonths) / 12;
	if(iModx < 0){
		iModx = iModx + 12;
		iMody--;
	}
	s.tm_mon = iModx;
	s.tm_year += iMody;

	if((t_ret = mktime(&s)) < 0) return -1;

	if (sFormatTime(t_ret, sTempDate) < 0) return -1;

	strcpy(sTempDate+8, sTemp+8);

	return 0;
}

int AddQuarters(char sTempDate[15], int iOffset)
{
	return AddMonths(sTempDate,iOffset*3);
}
int AddYears(char sTempDate[15], int iOffset)
{
	return AddMonths(sTempDate,iOffset*12);
}
/*yyyymmddhh24miss----------->yyyy/mm/dd hh24:mi:ss*/
/*注意本函数不加串结束符*/
int formatdatetime_simp_compl(char sAnswerTime[],char sOutput[])
{
	char sTempYear[5],sTempMon[3],sTempMDay[3],sTempHour[3],
	     sTempMin[3],sTempSec[3];

	struct tm s;
	time_t t_ret;

	bzero(&s,sizeof(struct tm));

	strncpy(sTempYear,sAnswerTime,4);sTempYear[4]=0;
	sTempMon[0]=sAnswerTime[4];
	sTempMon[1]=sAnswerTime[5];
	sTempMDay[0]=sAnswerTime[6]; sTempMon[2]=0;
	sTempMDay[1]=sAnswerTime[7]; sTempMDay[2]=0;
	sTempHour[0]=sAnswerTime[8];
	sTempHour[1]=sAnswerTime[9]; sTempHour[2]=0;

	sTempMin[0]=sAnswerTime[10];
	sTempMin[1]=sAnswerTime[11]; sTempMin[2]=0;

	sTempSec[0]=sAnswerTime[12];
	sTempSec[1]=sAnswerTime[13]; sTempSec[2]=0;

	s.tm_year=atoi(sTempYear)-1900;
	s.tm_mon=atoi(sTempMon)-1;
	s.tm_mday=atoi(sTempMDay);
	s.tm_hour=atoi(sTempHour);
	s.tm_min=atoi(sTempMin);
	s.tm_sec=atoi(sTempSec);
	s.tm_isdst=0;

	t_ret=mktime(&s);
	if(t_ret==-1){
	    printf("the answerTime is %s.\n",sAnswerTime);
	}
	strncpy(sOutput,sTempYear,4); 	sOutput[4]='/';
	strncpy(sOutput+5,sTempMon,2); 	sOutput[7]='/';
	strncpy(sOutput+8,sTempMDay,2);	sOutput[10]=' ';
	strncpy(sOutput+11,sTempHour,2);	sOutput[13]=':';
	strncpy(sOutput+14,sTempMin,2);	sOutput[16]=':';

	strncpy(sOutput+17,sTempSec,2);
	sOutput[19]=0;
	return t_ret;
}
/*yyyy/mm/dd hh24:mi:ss------>yyyymmddhh24miss*/
/*注意本函数不加串结束符*/
int formatdatetime_compl_simp(char sAnswerTime[],char sOutput[])
{
	char sTempYear[5],sTempMon[3],sTempMDay[3],sTempHour[3],
	     sTempMin[3],sTempSec[3];

	struct tm s;
	time_t t_ret;

	bzero(&s,sizeof(struct tm));

	strncpy(sTempYear,sAnswerTime,4);sTempYear[4]=0;

	if(sAnswerTime[4]!='/') return -1;

	sTempMon[0]=sAnswerTime[5];
	sTempMon[1]=sAnswerTime[6]; sTempMon[2]=0;

	if(sAnswerTime[7]!='/') return -1;

	sTempMDay[0]=sAnswerTime[8];
	sTempMDay[1]=sAnswerTime[9]; sTempMDay[2]=0;

	if(sAnswerTime[10]!=' ') return -1;

	sTempHour[0]=sAnswerTime[11];
	sTempHour[1]=sAnswerTime[12]; sTempHour[2]=0;

	if(sAnswerTime[13]!=':') return -1;

	sTempMin[0]=sAnswerTime[14];
	sTempMin[1]=sAnswerTime[15]; sTempMin[2]=0;

	if(sAnswerTime[16]!=':') return -1;

	sTempSec[0]=sAnswerTime[17];
	sTempSec[1]=sAnswerTime[18]; sTempSec[2]=0;

	s.tm_year=atoi(sTempYear)-1900;
	s.tm_mon=atoi(sTempMon)-1;
	s.tm_mday=atoi(sTempMDay);
	s.tm_hour=atoi(sTempHour);
	s.tm_min=atoi(sTempMin);
	s.tm_sec=atoi(sTempSec);
	s.tm_isdst=0;

	t_ret=mktime(&s);
	if(t_ret==-1){
	    printf("the answerTime is %s.\n",sAnswerTime);
	}
	strncpy(sOutput,sTempYear,4);
	strncpy(sOutput+4,sTempMon,2);
	strncpy(sOutput+6,sTempMDay,2);
	strncpy(sOutput+8,sTempHour,2);
	strncpy(sOutput+10,sTempMin,2);
	strncpy(sOutput+12,sTempSec,2);
	sOutput[14]=0;
	
	return t_ret;
}
/*yyyy-mm-dd hh24:mi:ss------>yyyymmddhh24miss*/
/*注意本函数不加串结束符*/
int formatdatetime_long_simp(char sAnswerTime[],char sOutput[])
{
	char sTempYear[5],sTempMon[3],sTempMDay[3],sTempHour[3],
		sTempMin[3],sTempSec[3];

	struct tm s;
	time_t t_ret;

	bzero(&s,sizeof(struct tm));

	strncpy(sTempYear,sAnswerTime,4);sTempYear[4]=0;

	if(sAnswerTime[4]!='-') return -1;

	sTempMon[0]=sAnswerTime[5];
	sTempMon[1]=sAnswerTime[6]; sTempMon[2]=0;

	if(sAnswerTime[7]!='-') return -1;

	sTempMDay[0]=sAnswerTime[8];
	sTempMDay[1]=sAnswerTime[9]; sTempMDay[2]=0;

	if(sAnswerTime[10]!=' ') return -1;

	sTempHour[0]=sAnswerTime[11];
	sTempHour[1]=sAnswerTime[12]; sTempHour[2]=0;

	if(sAnswerTime[13]!=':') return -1;

	sTempMin[0]=sAnswerTime[14];
	sTempMin[1]=sAnswerTime[15]; sTempMin[2]=0;

	if(sAnswerTime[16]!=':') return -1;

	sTempSec[0]=sAnswerTime[17];
	sTempSec[1]=sAnswerTime[18]; sTempSec[2]=0;

	s.tm_year=atoi(sTempYear)-1900;
	s.tm_mon=atoi(sTempMon)-1;
	s.tm_mday=atoi(sTempMDay);
	s.tm_hour=atoi(sTempHour);
	s.tm_min=atoi(sTempMin);
	s.tm_sec=atoi(sTempSec);
	s.tm_isdst=0;

	t_ret=mktime(&s);
	if(t_ret==-1){
		printf("the answerTime is %s.\n",sAnswerTime);
	}
	strncpy(sOutput,sTempYear,4);
	strncpy(sOutput+4,sTempMon,2);
	strncpy(sOutput+6,sTempMDay,2);
	strncpy(sOutput+8,sTempHour,2);
	strncpy(sOutput+10,sTempMin,2);
	strncpy(sOutput+12,sTempSec,2);
	sOutput[14]=0;
	return t_ret;
}

/*yyyy-mm-dd------>yyyymmdd*/
int formatdate_long_simp(char sAnswerTime[],char sOutput[])
{
	char sTempYear[5], sTempMon[3], sTempMDay[3];

	struct tm s;
	time_t t_ret;

	bzero(&s, sizeof(struct tm));

	strncpy(sTempYear,sAnswerTime,4);sTempYear[4]=0;

	if(sAnswerTime[4] != '-') return -1;

	sTempMon[0] = sAnswerTime[5];
	sTempMon[1] = sAnswerTime[6]; sTempMon[2] = 0;

	if(sAnswerTime[7]!='-') return -1;

	sTempMDay[0] = sAnswerTime[8];
	sTempMDay[1] = sAnswerTime[9]; sTempMDay[2]=0;

	s.tm_year = atoi(sTempYear)-1900;
	s.tm_mon = atoi(sTempMon)-1;
	s.tm_mday = atoi(sTempMDay);
	s.tm_hour = 0;
	s.tm_min = 0;
	s.tm_sec = 0;
	s.tm_isdst=0;

	t_ret=mktime(&s);
	if(t_ret==-1){
	    printf("the answerTime is %s.\n",sAnswerTime);
	}
	sprintf(sOutput, "%04d%02d%02d", s.tm_year, s.tm_mon, s.tm_mday);

	return t_ret;
}

/*hh24:mi:ss------>hh24miss*/
int formattime_long_simp(char sAnswerTime[],char sOutput[])
{
	char sTempHour[3], sTempMin[3], sTempSec[3];

	struct tm s;
	time_t t_ret;

	bzero(&s,sizeof(struct tm));

	sTempHour[0]=sAnswerTime[1];
	sTempHour[1]=sAnswerTime[2]; sTempHour[2]=0;

	if(sAnswerTime[3]!=':') return -1;

	sTempMin[0]=sAnswerTime[4];
	sTempMin[1]=sAnswerTime[5]; sTempMin[2]=0;

	if(sAnswerTime[6]!=':') return -1;

	sTempSec[0]=sAnswerTime[7];
	sTempSec[1]=sAnswerTime[8]; sTempSec[2]=0;

	s.tm_year = 100;
	s.tm_mon = 1;
	s.tm_mday = 1;
	s.tm_hour = atoi(sTempHour);
	s.tm_min = atoi(sTempMin);
	s.tm_sec = atoi(sTempSec);
	s.tm_isdst = 0;

	t_ret=mktime(&s);
	if(t_ret==-1){
		printf("the answerTime is %s.\n",sAnswerTime);
	}
	sprintf(sOutput, "%02d%02d%02d", s.tm_hour, s.tm_min, s.tm_sec);

	return t_ret;
}

void TrimLeft(char * s)
{
	int i=0;

	if(s[0]!=' ') return;

	while (s[i]==' ') i++;
	strcpy(s,s+i);
}
void TrimRight(char *s)
{
	int i;
	
	i = strlen(s)-1;
	
	while ( (i>=0) && (s[i]==' ') ) i--;
	
	s[i+1]='\0';
}
void AllTrim(char *s)
{
/*
由于这个函数调用非常频繁所以没有用函数调用
	TrimLeft(s);
	TrimRight(s);
*/

	int i=0;
	if(s[0]!=' ') goto trimall_next;

	while (s[i]==' ') i++;
	strcpy(s,s+i);

trimall_next:
	i = strlen(s)-1;
	
	while ( (i>=0) && (s[i]==' ') )	i--;

	s[i+1]='\0';
}
void TrimDate(char sDate[15])
{
	if(strcmp(sDate,"20500101000000")>0)
		strcpy(sDate,"20500101000000");
	if(strcmp(sDate,"19000101000000")<0)
		strcpy(sDate,"19000101000000");
}
void TrimLeftX(char * s,char X)
{
	int i=0;

	if(s[0]!=X) return;

	while (s[i]==X) i++;

	strcpy(s,s+i);
}
void TrimRightX(char *s,char X)
{
	int i;
	
	i = strlen(s);
	
	while ( (i>=0) && (s[i]==X) ) i--;
	
	s[i+1]='\0';
}
void TrimAllX(char *s,char X)
{
/*
由于这个函数调用非常频繁所以没有用函数调用
	TrimLeft(s);
	TrimRight(s);
*/

	int i=0;

	if(s[0]!=X) goto trimall_next;

	while (s[i]==X) i++;

	strcpy(s,s+i);

trimall_next:
	i = strlen(s)-1;
	
	while ( (i>=0) && (s[i]==X) )	i--;

	s[i+1]='\0';
}
void RightPad(char sTemp[],int iLen,char c)
{
	int i,l;
	l=strlen(sTemp);
	if(l>=iLen) return;
	for(i=l;i<iLen;i++)
		sTemp[i]=c;
	sTemp[i]=0;
}
void LeftPad(char sTemp[],int iLen,char c)
{
	int i,l;
	l=strlen(sTemp);
	if(l>=iLen) return;
	
	for(i=0;i<=iLen;i++){
		if(l>=i)
			sTemp[iLen-i]=sTemp[l-i];
		else
			sTemp[iLen-i]=c;
	}
}
void TrimAllS(char *s,char *S)
{

	int i=0;

	if(strchr(S,s[i])!=NULL) goto trimall_next;

	while (strchr(S,s[i])!=NULL) i++;

	strcpy(s,s+i);

trimall_next:
	i = strlen(s)-1;
	
	while ( (i>=0) && (strchr(S,s[i])!=NULL) )	i--;

	s[i+1]='\0';
}
void TrimAll(char *s)
{
	TrimAllS(s,(char*)" \n\r\t");
}
void Upper(char *s)
{
	while((*s)!=0){
		if((*s)>='a'&&(*s)<='z') 
			(*s)+=('A'-'a');
		s++;
	}
}
void Lower(char *s)
{
	while((*s)!=0){
		if((*s)>='A'&&(*s)<='Z')
			(*s)+=('a'-'A');
		s++;
	}
}
void WaitSec(int iSec)
{
	time_t tReturnTime,tCurTime;

	time(&tCurTime);
	tReturnTime=tCurTime+iSec;
	while(tCurTime<tReturnTime){
		sleep(1);
		time(&tCurTime);
	}
}
int MyHexStrToInt(char sTemp[])
{
	static int iFirstFlag=TRUE,aiAnti[256];
	int i,j,iRet=0,iLen;
	unsigned char *p=(unsigned char*)sTemp;
	
	if(iFirstFlag==TRUE){
		iFirstFlag=FALSE;
		for(i=0;i<256;i++){
			switch(i){
			case 'a':
			case 'A':aiAnti[i]=10;break;
			case 'b':
			case 'B':aiAnti[i]=11;break;
			case 'c':
			case 'C':aiAnti[i]=12;break;
			case 'd':
			case 'D':aiAnti[i]=13;break;
			case 'e':
			case 'E':aiAnti[i]=14;break;
			case 'f':
			case 'F':aiAnti[i]=15;break;
			
			default :aiAnti[i]=0;			
			}
			if(i>='0'&&i<='9') aiAnti[i]=i-'0';	
		}
	}
	iLen=strlen(sTemp);
	
	if(iLen==0||iLen>8) return 0;
	
	for(i=0;i<iLen;i++){
		j=aiAnti[ p[i] ];
		iRet=iRet*16+j;
	}
	
	return iRet;
}
int  Str2Int(char sTemp[])
{
	int iKey;
	char *pc;
	
        if((pc=strstr(sTemp,"0x"))!=NULL||
                (pc=strstr(sTemp,"0X"))!=NULL)
                iKey=MyHexStrToInt(pc+2);
        else
                iKey=atoi(sTemp);
	return iKey;
}

int SearchOffString(char sRecord[],int iOffset,int iSeparator,char sTemp[])
{
	int i;
	char	*b=sRecord,*e;

	if(iOffset<0||iOffset>100) return NOTFOUND;

	for(i=0;i<iOffset;i++){
		if((b=strchr(b,iSeparator))==NULL)
			return NOTFOUND;
		b++;/*跳过分割符*/
	}

	if((e=strchr(b,iSeparator))==NULL){
		if((e=strchr(b,'\n'))==NULL) return NOTFOUND;
	}

	strncpy(sTemp,b,e-b);
	sTemp[e-b]=0;

	AllTrim(sTemp);

	return NOTFOUND;
}

int ParseArgv(char sRecord[], int chr)
{
	int i;
	char *si, *so;

/*	memset(ParseToken, 0, PARSE_ARRAY_LEN*PARSE_ITEM_LEN);*/

	for(i=0,si=sRecord;i<PARSE_ARRAY_LEN;i++){
		if((so = strchr(si, chr)) == NULL){
			strcpy(ParseToken[i], si);
			break;
		}
/*		if((so = strchr(si+1, chr)) == NULL){
			strncpy(ParseToken[i], si, sRecord-si);
			ParseToken[i][sRecord-si] = 0;
			break;
		}*/

		strncpy(ParseToken[i], si, so-si);
		ParseToken[i][so-si] = 0;
		AllTrim(ParseToken[i]);
		si = so+1;
	}

	return i+1;
}

/*判断是否有其他的选项,TRUE存在,FALSE不存在*/
int ExistOtherOption(int argc,char *argv[],char sOptionFilter[])
{
	int i;
		
	for(i=1;i<argc;i++){

		/*是参数选项并是指定参数*/
		if(argv[i][0]=='-' && strchr(sOptionFilter,argv[i][1])==NULL)
			return TRUE;
	}
	return FALSE;
}
/*是否存在某个选项 TRUE存在,FALSE不存在*/
int ExistOption(int argc,char *argv[],char sOptionFilter[])
{
	char sOption[1024];

	if(SearchOptionStr(argc,argv,sOptionFilter,sOption)==NOTFOUND)
		return NOTFOUND;

	return FOUND;

}

/*获取某选项是否存在,FOUND,NOTFOUND,选项后的串给出*/
int SearchOptionStr(int argc,char *argv[],char sOptionFilter[],char sOption[])
{
	int i,iRet=NOTFOUND;
	
	strcpy(sOption,"");
		
	for(i=1;i<argc;i++){

		/*是参数选项并是指定参数*/
		if(argv[i][0]=='-' && strchr(sOptionFilter,argv[i][1])!=NULL){

			iRet=FOUND;

			if(argc<=i) return iRet;

			if(argv[i+1][0]=='-') return iRet;

			strcpy(sOption,argv[i+1]);
		}

	}
	return iRet;

}
/*获取某选项是否存在,FOUND,NOTFOUND,选项后的串给出*/
int SearchOptionInt(int argc,char *argv[],char sOptionFilter[],int *piOption)
{
	char sOption[1024];

	if(SearchOptionStr(argc,argv,sOptionFilter,sOption)==NOTFOUND)
		return NOTFOUND;
	
	*piOption=atoi(sOption);
	return FOUND;
}
int LLike(char sInput[],char sVal[])
{
	return (strncmp(sInput,sVal,strlen(sVal))==0)?true:false;
}
int RLike(char sInput[],char sVal[])
{
	int l=strlen(sInput),l1=strlen(sVal);
	
	if(l<l1) return false;
	
	return (strcmp(sInput+l-l1,sVal)==0)?true:false;
	
}
void ReverseAll(char *s)
{
	char c;
	int i=0,l1;

	l1=strlen(s)-1;

	while(i<l1){
		c=s[i];
		s[i++]=s[l1];
		s[l1--]=c;
	}
}
void DelSubStr(char sInput[],char sSub[])
{
	int l=0;
	char *p,*p1;

	p=sInput;

	while(1){
		if((p1=strstr(p,sSub))==NULL){
			strcpy(sInput+l,p);
			break;
		}
		strncpy(sInput+l,p,p1-p);
		l+=p1-p;
		p=p1+strlen(sSub);
	}
}
void Replace(char sInput[],char x,char y)
{
	int i,l=strlen(sInput);

	for(i=0;i<l;i++)
		if(sInput[i]==x) sInput[i]=y;
}
void ReplaceStr(char sInput[],char x[],char y[])
{
	char sTemp[2048],*p;

	p=sInput;
	
	while(1){
		if((p=strstr(p,x))==NULL) break;
		
		strncpy(sTemp,sInput,p-sInput);
		sTemp[p-sInput]=0;
		strcat(sTemp,y);
		strcat(sTemp,p+strlen(x));
		
		strcpy(sInput,sTemp);
		p+=strlen(y);
	}
}
int LikeSupp(char sStr[],char P[],char c0,char c1)
{
	char c,d;
	int i=0,j=0;

	while(P[i]!=0){
		c=P[i];
		if(c==c0){
			int j1=j+1;

			if(P[i+1]==0) return true;	

			for(;sStr[j1]!=0;j1++){
				if(LikeSupp(sStr+j1,P+i+1,c0,c1)==true)
					return true;
			}
			return false;
		}

		if(c=='['){

			int k=0,n=0,m=0;
			char sCharSet[256],b,e,t;

			k=i+1;
			while(P[k]!=']'&&P[k]!=0) k++;

			if(P[k]!=']') return false;
			
			n=i+1;
			while(n<k){
				if(P[n]==','){
					n++;
					continue;
				}
				if(P[n]=='-'){
					b=P[n-1];e=P[n+1];
					if(e<b||e<0||b<0){
						n++;
						continue;
					}
					for(t=b+1;t<e;t++)
						sCharSet[m++]=t;
				}
				else
					sCharSet[m++]=P[n];
				n++;
				
			}
			/*条过']'*/
			n++;
			
			sCharSet[m]=0;
			if(strchr(sCharSet,sStr[j])==NULL) return false;
			i=n;j++;
		}
		else{
			d=sStr[j];
			if(c!=d&&c!=c1) return false;
			i++;j++;
		}	
	}

	if(sStr[j]!=0)  return false;

	return true;
}
int Like(char sStr[],char P[])
{
	return LikeSupp(sStr,P,'%','_');
}
int LikeX(char sStr[],char P[])
{
	return LikeSupp(sStr,P,'*','?');
}

int HostTimeEff(char sEffDate[],char sExpDate[])
{
	static int iFirstFlag=true,iCnt=0;
	static char sHostTime[15];

	if(iFirstFlag==true){
		iFirstFlag=false;
		GetHostTime(sHostTime);
	}
	/*每300次调用，重新初始化一下sHostTime变量*/
	if((++iCnt)>=300){
		iFirstFlag=true;
		iCnt=0;
	}
	
	if(strcmp(sHostTime,sEffDate)>=0&&
		strcmp(sHostTime,sExpDate)<0) return true;
	return false;
}
