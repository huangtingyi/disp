#ifndef __DMPFMT_H__
#define __DMPFMT_H__

#define MY_TYPE_SSEL2_Quotation		1
#define MY_TYPE_SSEL2_Transaction	2
#define MY_TYPE_SSEL2_Auction		3
#define MY_TYPE_SZSEL2_Quotation	4
#define MY_TYPE_SZSEL2_Transaction	5
#define MY_TYPE_SZSEL2_Order		6

#define MY_TYPE_TDF_MKT			'M'
#define MY_TYPE_TDF_TRA			'T'
#define MY_TYPE_TDF_ORD			'O'
#define MY_TYPE_TDF_QUE			'Q'

#define MY_TYPE_D31_STD			7
#define MY_TYPE_D31_EXT			8


#define MY_DATE_CEIL_LONG 1000000000L

#include "QTSStruct.h"
#include "TDFAPIStruct.h"

#include "d31_item.h"

#include "mktdata.pb.h"
#include "d31data.pb.h"

int print_MY_TYPE_SSEL2_Quotation(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_SSEL2_Transaction(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_SSEL2_Auction(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_SZSEL2_Quotation(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_SZSEL2_Transaction(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_SZSEL2_Order(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_TDF_MKT(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_TDF_TRA(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_TDF_ORD(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_TDF_QUE(char *buf,char sCodeList[],int iTimeFlag,
        long lBgnTime,long *plCurTime,char *outbuf);
        
        
void MktData2TDF_MARKET_DATA(MktData &md, TDF_MARKET_DATA *po);
int convert_MY_TYPE_TDF_MKT(char *pi,long lRecLen,char *po);
void Transaction2TDF_TRANSACTION(Transaction &tr, TDF_TRANSACTION *po);
int convert_MY_TYPE_TDF_TRA(char *pi,long lRecLen,char *po);
void Order2TDF_ORDER(Order &od, TDF_ORDER *po);
int convert_MY_TYPE_TDF_ORD(char *pi,long lRecLen,char *po);
void Order_queue2TDF_ORDER_QUEUE(Order_queue &oq, TDF_ORDER_QUEUE *po);
int convert_MY_TYPE_TDF_QUE(char *pi,long lRecLen,char *po);

int print_MY_TYPE_D31_STD(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);
int print_MY_TYPE_D31_EXT(char *buf,char sCodeList[],int iTimeFlag,
	long lBgnTime,long *plCurTime,char *outbuf);

void D31Item2D31_ITEM(D31Item &di, struct D31ItemStruct *po);
int convert_MY_TYPE_D31_ITEM(char *pi,long lRecLen,char *po);


#endif
