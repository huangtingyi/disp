
#include "index_supp.h"

long alAmntLevel[MAX_LEVEL_CNT]={
	0,
	5*10000*100,
	10*10000*100,
	20*10000*100,
	40*10000*100,
	60*10000*100,
	80*10000*100,
	100*10000*100,
	200*10000*100,
	500*10000*100
};
long alJmpLevel[MAX_JMP_LEVEL_CNT]={
	20*10000*100,
	50*10000*100,
	100*10000*100
};
struct IndexStatStruct *INDEX_HEAD=NULL,*INDEX_ETF=NULL;
struct IndexStatStruct *AISTAT[MAX_STOCK_CODE];


void TDF_TRANSACTION2TinyTransaction(struct TDF_TRANSACTION *pi,struct TinyTransactionStruct *po)
{
	po->pNext=NULL;
	po->pAskOrder=po->pBidOrder=NULL;
	po->nActionDay=	pi->nActionDay;
	po->nTime=	pi->nTime;
	po->iStockCode=	atoi(pi->szCode);
	po->nPrice=	(int)pi->nPrice;
	po->nVolume=	pi->nVolume;
	po->nAskOrder=	pi->nAskOrder;
	po->nBidOrder=	pi->nBidOrder;

	po->nBSFlag=	pi->nBSFlag;

	po->nAskOrderSeq=0;
	po->nBidOrderSeq=0;

	po->nAskJmpSeq=0;
	po->nBidJmpSeq=0;
}
void TDF_TRANSACTION2TinyOrderS(struct TDF_TRANSACTION *pi,struct TinyOrderStruct *po)
{
	po->pNext=NULL;
	po->nActionDay=	pi->nActionDay;
	po->nTime=	pi->nTime;
	po->iStockCode=	atoi(pi->szCode);
	po->nPrice=	(int)pi->nPrice;
	po->nVolume=	pi->nVolume;
	po->nOrder=	pi->nAskOrder;
	po->nBroker=	0;
	po->nBSFlag=	pi->nBSFlag;
	po->nOrderSeq=	0;
	po->lOrderAmnt=(long)(pi->nPrice)*pi->nVolume/100;

	po->iCloseFlag=0;
	po->nOriOrdPrice=0;
	po->nOriOrdVolume=0;
	po->lOriOrdAmnt=0;

	po->nLastPrice=po->nPrice;
	po->nAskJmpSeq=0;
	po->nBidJmpSeq=0;
}
void TDF_TRANSACTION2TinyOrderB(struct TDF_TRANSACTION *pi,struct TinyOrderStruct *po)
{
	po->pNext=NULL;
	po->nActionDay=	pi->nActionDay;
	po->nTime=	pi->nTime;
	po->iStockCode=	atoi(pi->szCode);
	po->nPrice=	(int)pi->nPrice;
	po->nVolume=	pi->nVolume;
	po->nOrder=	pi->nBidOrder;
	po->nBroker=	0;
	po->nBSFlag=	pi->nBSFlag;
	po->nOrderSeq=	0;
	po->lOrderAmnt=(long)(pi->nPrice)*pi->nVolume/100;

	po->iCloseFlag=0;
	po->nOriOrdPrice=0;
	po->nOriOrdVolume=0;
	po->lOriOrdAmnt=0;

	po->nLastPrice=po->nPrice;
	po->nAskJmpSeq=0;
	po->nBidJmpSeq=0;
}
void TDF_ORDER2TinyOrder(struct TDF_ORDER *pi,struct TinyOrderStruct *po)
{
	po->pNext=NULL;
	po->nActionDay=	pi->nActionDay;
	po->nTime=	pi->nTime;
	po->iStockCode=	atoi(pi->szCode);
	po->nPrice=	0;
	po->nVolume=	0;
	po->nOrder=	pi->nOrder;
	po->nBroker=	pi->nBroker;
	po->nBSFlag=	pi->chFunctionCode;
	po->nOrderSeq=	0;
	po->lOrderAmnt= 0;

	po->iCloseFlag=	0;
	po->nOriOrdPrice=(int)pi->nPrice;
	po->nOriOrdVolume=pi->nVolume;
	po->lOriOrdAmnt=(long)(pi->nPrice)*pi->nVolume/100;

	po->nLastPrice=0;
	po->nAskJmpSeq=0;
	po->nBidJmpSeq=0;

}

int data_search_bintree_order_2(void *pValue,void*pData)
{
	return *((int*)pValue)-((struct TinyOrderStruct *)pData)->nOrder;
}
struct IndexStatStruct *NewInitIndexStat(int iStockCode,int nActionDay,int nPreT0,int nT0)
{
	struct IndexStatStruct *p;

	if((p=(struct IndexStatStruct *)malloc(sizeof(struct IndexStatStruct)))==NULL){
		printf("malloc IndexStatStruct error.\n");
		return NULL;
	}
	bzero((void*)p,sizeof(struct IndexStatStruct));

	p->iStockCode=	iStockCode;
	p->nActionDay=	nActionDay;
	p->nPreT0=	nPreT0;
	p->nT0=		nT0;

	return p;
}
//#define MY_TINY_ORDER(x) ((struct TinyOrderStruct *)x)

int data_search_bintree_stock_code_order(void *pValue,void*pData)
{
	return ((struct TinyOrderStruct *)pValue)->nOrder-
		((struct TinyOrderStruct *)pData)->nOrder;
}
void assign_insert_bintree_stock_code_order_e(void **ppData,void *pData)
{
	LISTHEAD *pS0O;
	struct TinyOrderStruct *p=(struct TinyOrderStruct *)pData;

	pS0O=(LISTHEAD*)(p->pNext);

	Append2List(pS0O,(LIST*)p);

	*ppData=pData;
}

//订单合并到交易合并订单树中
int AddTinyOrder2Tree(BINTREE **PP,int *piCnt,struct TinyOrderStruct *p)
{
	struct TinyOrderStruct *pTemp;

	if(SearchBin(*PP,p,data_search_bintree_stock_code_order,
		(void**)&pTemp)==NOTFOUND){

		if((pTemp=(struct TinyOrderStruct*)malloc(
			sizeof(struct TinyOrderStruct)))==NULL){
			printf("malloc TinyOrderStruct error.\n");
			return -1;
		}
		memcpy((void*)pTemp,(void*)p,sizeof(struct TinyOrderStruct));

		if(InsertBin(PP,(void *)pTemp,
			data_search_bintree_stock_code_order,
			assign_insert_bintree_stock_code_order_e)<0){
			printf("insert bin malloc error th.\n");
			return -1;
		}
		(*piCnt)++;
	}
	else{
		//来源于深圳的情况，将nLastPrice设置为初始价格
		if(pTemp->nLastPrice==0)
			pTemp->nLastPrice=p->nPrice;

		pTemp->nVolume+=	p->nVolume;
		//考虑到统计的笔数，其实是统计多少个订单所以这里不累计OrderNo
		//pTemp->nOrderNum+=	p->nOrderNum;
		pTemp->lOrderAmnt+=	p->lOrderAmnt;

	}
	return 0;
}

//订单合并到交易合并订单列表中
int AddTinyOrder2List(LISTHEAD *pS1O,struct TinyOrderStruct *p)
{
	struct TinyOrderStruct *pTemp;

	if((pTemp=(struct TinyOrderStruct*)malloc(
		sizeof(struct TinyOrderStruct)))==NULL){
		printf("malloc TinyOrderStruct error.\n");
		return -1;
	}
	memcpy((void*)pTemp,(void*)p,sizeof(struct TinyOrderStruct));

	Append2List(pS1O,(LIST*)pTemp);

	return 0;
}


int AddTransaction2IndexStat(struct TDF_TRANSACTION *pi,int nT0,
	struct IndexStatStruct *pIndexStat)
{
	struct TinyOrderStruct ob,os;

	TDF_TRANSACTION2TinyOrderS(pi,&os);
	TDF_TRANSACTION2TinyOrderB(pi,&ob);

	if(MY_BELONG_TRANSACTION_T0(pi->nTime,nT0)){
//	if(pi->nTime<nT0||
//		(nT0==MY_CLOSE_MARKET_TIME&&pi->nTime==MY_CLOSE_MARKET_TIME)){
	

		//将链表头的指针带入
		os.pNext=(struct TinyOrderStruct*)&(pIndexStat->S0O);
		//卖出订单合并到交易合并订单树中
		if(AddTinyOrder2Tree(&(pIndexStat->M_ORDER),&pIndexStat->iS0OCnt,&os)<0){
			printf("error add ask tiny order to tree\n");
			return -1;
		}

		//将链表头的指针带入
		//ob.pNext=&(pIndexStat->PreS0M);
		ob.pNext=(struct TinyOrderStruct*)&(pIndexStat->S0O);
		//买入订单合并到交易合并订单树中
		if(AddTinyOrder2Tree(&(pIndexStat->M_ORDER),&pIndexStat->iS0OCnt,&ob)<0){
			printf("error add bid tiny order to tree \n");
			return -1;
		}
	}
	else{
		//卖出订单添加到交易合并订单链表中
		if(AddTinyOrder2List(&(pIndexStat->S1O),&os)<0){
			printf("error add ask tiny order to list\n");
			return -1;
		}
		//买入订单添加到交易合并订单链表中
		if(AddTinyOrder2List(&(pIndexStat->S1O),&ob)<0){
			printf("error add bid tiny order to list\n");
			return -1;
		}
	}
	return 0;
}

void AskOrder2D31(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexItemStruct *pD31,struct IndexStatStruct *pInd)
{
	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_LEVEL_CNT;i++){
		if(alAmntLevel[i]>lAmnt) break;

		pD31->alAskAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
		pD31->aiAskVolume[i]+=	pt->nVolume;

		//只统计委托单的第一笔成交
		if(pt->nAskOrderSeq==1){
			if(pInd->iStockCode==2320&&
				pInd->nT0==93100000&&
				i==5)
				printf("================================%d================\n",p->nOrder);

			pD31->aiAskOrderNum[i]++;
		}
	}
}
void BidOrder2D31(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexItemStruct *pD31)
{

	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_LEVEL_CNT;i++){
		if(alAmntLevel[i]>lAmnt) break;
		pD31->alBidAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
		pD31->aiBidVolume[i]+=	pt->nVolume;

		//只统计委托单的第一笔成交
		if(pt->nBidOrderSeq==1)pD31->aiBidOrderNum[i]++;
	}
}

void AskOrder2D31Zb(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexItemStruct *pD31)
{
	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_LEVEL_CNT;i++){
		if(alAmntLevel[i]>lAmnt) break;
		pD31->alAskAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
		pD31->aiAskVolume[i]+=	pt->nVolume;

		//只统计委托单的第一笔成交
		if(pt->nAskOrderSeq==1)pD31->aiAskOrderNum[i]++;
/*		if(i>=9&&pt->iStockCode==2320){
			printf("--------------------------------------------.\n");
		}
*/
	}
}

void BidOrder2D31Zb(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexItemStruct *pD31)
{
	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_LEVEL_CNT;i++){
		if(alAmntLevel[i]>lAmnt) break;
		pD31->alBidAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
		pD31->aiBidVolume[i]+=	pt->nVolume;

		//只统计委托单的第一笔成交
		if(pt->nBidOrderSeq==1)pD31->aiBidOrderNum[i]++;
	}
}
void AskOrder2D31Ex(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexExtStruct *pD31)
{
	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_JMP_LEVEL_CNT;i++){
		if(alJmpLevel[i]>lAmnt) break;
		pD31->alAskAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
	}
}
void BidOrder2D31Ex(struct TinyOrderStruct *p,struct TinyTransactionStruct *pt,
	struct D31IndexExtStruct *pD31,struct IndexStatStruct *pInd)
{
	long lAmnt=p->nOriOrdVolume?(long)p->nOriOrdVolume*(long)pt->nPrice/100:p->lOrderAmnt;

	for(int i=0;i<MAX_JMP_LEVEL_CNT;i++){
		if(alJmpLevel[i]>lAmnt) break;
		if(i==0&&pInd->nT0==93800000&&
			pInd->iStockCode==603912){
				printf("=xxxxxxxxxxxxxx========%d================\n",p->nOrder);
		}
		pD31->alBidAmount[i]+=	(long)pt->nVolume*pt->nPrice/100;
	}
}
int InitTinyTransactionField(struct IndexStatStruct *p,struct TinyTransactionStruct *pTemp)
{

	struct TinyOrderStruct *pAskOrder,*pBidOrder;

	//找到卖方订单金额
	if(SearchBin(p->M_ORDER,(void*)&(pTemp->nAskOrder),
		data_search_bintree_order_2,(void**)&pAskOrder)==NOTFOUND){
		printf("hello 3 code=%d,askord=%d time=%d,flag=%c\n",
			p->iStockCode,pTemp->nAskOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
		printf("hello 2\n");
		return -1;
	}
	//找到买方订单金额
	if(SearchBin(p->M_ORDER,(void*)&(pTemp->nBidOrder),
		data_search_bintree_order_2,(void**)&pBidOrder)==NOTFOUND){
		printf("hello 3 code=%d,bidord=%d time=%d,flag=%c\n",
			p->iStockCode,pTemp->nBidOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
		return -1;
	}

	//这个逻辑已经移到加载逻辑中
	pAskOrder->nOrderSeq++;
	pBidOrder->nOrderSeq++;

	pTemp->nAskOrderSeq=pAskOrder->nOrderSeq;
	pTemp->nBidOrderSeq=pBidOrder->nOrderSeq;
	pTemp->pAskOrder=pAskOrder;
	pTemp->pBidOrder=pBidOrder;
	
	if(pTemp->nPrice<pAskOrder->nLastPrice)	pTemp->nAskJmpSeq++;

	if(pTemp->nPrice>pBidOrder->nLastPrice&&
		pBidOrder->nLastPrice!=0) pTemp->nBidJmpSeq++;

	pAskOrder->nLastPrice=pBidOrder->nLastPrice=pTemp->nPrice;

	return 0;
}

//b)根据S0T列表中，循环查找M_ORDER表中，生成 D31IndexItem数据；
int GenD31Stat(struct IndexStatStruct *p)
{
	struct D31IndexItemStruct *pD31Zd=&(p->Zd),*pD31Zb=&(p->Zb);
	struct D31IndexExtStruct *pD31Ex=&p->Ex;

	struct TinyOrderStruct *pAskOrder,*pBidOrder;
	struct TinyTransactionStruct *ptHead,*pTemp;

	struct TinyQuotationStruct *ptPre,*ptCur;

	ptHead=(struct TinyTransactionStruct *)p->S0T.pHead;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		pAskOrder=pTemp->pAskOrder;
		pBidOrder=pTemp->pBidOrder;
		
		if(pTemp->nAskOrder==1691843&&pTemp->nBidOrder==1732854){
			printf("shenme luoji error.\n");
		}
		if(pTemp->nAskOrder==374174&&pTemp->nBidOrder==371812){
			printf("shenme luoji error =============2.\n");
		}
		//统计逐单信息
		AskOrder2D31(pAskOrder,pTemp,pD31Zd,p);
		BidOrder2D31(pBidOrder,pTemp,pD31Zd);

		if(pD31Zd->aiBidVolume[0]!=pD31Zd->aiAskVolume[0]&&
			p->nT0==94400000&&p->iStockCode==2908){
			printf("shenqi lulu askord=%d,bidord=%d.\n",
				pTemp->nAskOrder,pTemp->nBidOrder);
		}

		//统计逐笔信息
		if(pTemp->nBSFlag=='S')
			AskOrder2D31Zb(pAskOrder,pTemp,pD31Zb);
		else	BidOrder2D31Zb(pBidOrder,pTemp,pD31Zb);

		
		//设置订单临时跳买、跳卖标识
		//只有非零的才设置上去，避免设置为非零后又被后面的交易记录信息清零
		if(pTemp->nAskJmpSeq>0)	pAskOrder->nAskJmpSeq=pTemp->nAskJmpSeq;
		if(pTemp->nBidJmpSeq>0) pBidOrder->nBidJmpSeq=pTemp->nBidJmpSeq;

	}

	//再做一个循环，检查跳买和跳卖的标识，如果存在跳买跳卖则统计
	ptHead=(struct TinyTransactionStruct *)p->S0T.pHead;
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		pAskOrder=pTemp->pAskOrder;
		pBidOrder=pTemp->pBidOrder;

		//d31_new[跳卖i] = trans[(trans['委卖金额'] >= i * 10000) &
		//(trans['跳卖次数']>0)].groupby(['time_str'])['成交金额'].sum().reindex(index = trade_time)/10000
		if(pAskOrder->nAskJmpSeq>0) AskOrder2D31Ex(pAskOrder,pTemp,pD31Ex);
		//d31_new[跳买i] = trans[(trans['委买金额'] >= i * 10000) &
		//(trans['跳买次数']>0)].groupby(['time_str'])['成交金额'].sum().reindex(index = trade_time)/10000
		if(pBidOrder->nBidJmpSeq>0) BidOrder2D31Ex(pBidOrder,pTemp,pD31Ex,p);
			
	}
	
	//做一个循环，将计算跳买，跳卖的中间字段清零
	//避免跨分钟时的跳卖，跳买标志信息传递
	ptHead=(struct TinyTransactionStruct *)p->S0T.pHead;
	while(ptHead!=NULL){		
		ptHead->pAskOrder->nAskJmpSeq=ptHead->pBidOrder->nBidJmpSeq=0;
		ptHead=ptHead->pNext;
	}


	//取时间段内的最后一笔行情进行处理
	/*****
	//上半部分指标开始
	int	nTenBidVolume;			//十档买量（手）
	int	nTenAskVolume;			//十档卖量（手）
	int8b	lTenBidAmnt;			//十档买额（分）
	int8b	lTenAskAmnt;			//十档卖额（分）
	int	nTotalBidVolume;		//叫买总量（手）
	int	nTotalAskVolume;		//叫卖总量（手）
	int8b	lTotalBidAmnt;			//叫买总额（分）
	int8b	lTotalAskAmnt;			//叫卖总额（分）
	int	nWtAvgBidPrice;			//加权平均叫买价（分）
	int	nWtAvgAskPrice;			//加权平均叫卖价（分）
	//下半部分指标开始
	int	nLastClose;			//昨收盘价
	int	nCurPrice;			//最新价
	int8b	lAvgTotalBidAmnt;		//平均叫买总额（当日平均，分）
	int8b	lAvgTotalAskAmnt;		//平均叫卖总额（当日平均，分）
	*****/

	ptCur=(struct TinyQuotationStruct *)p->S0Q.pHead;
	//统计行情数，以及每笔行情的叫卖总额、叫买总额的累计
	while(ptCur!=NULL){
		ptPre=ptCur;
		ptCur=ptCur->pNext;

		p->iQuotationCnt++;
		p->lAddupTotalBidAmnt+=	ptPre->nTotalBidVol*ptPre->nWtAvgBidPrice/100;
		p->lAddupTotalAskAmnt+=	ptPre->nTotalAskVol*ptPre->nWtAvgAskPrice/100;

		if(p->lAddupTotalBidAmnt<0||
			p->lAddupTotalAskAmnt<0){
			printf("design errror.\n");
		}

		if((LIST*)ptPre==p->S0Q.pTail){
//			p->lLastTotalBidAmnt=	ptPre->nTotalBidVol*ptPre->nWtAvgBidPrice/100;
//			p->lLastTotalAskAmnt=	ptPre->nTotalAskVol*ptPre->nWtAvgAskPrice/100;
			break;
		}
		//除了最后一个节点都释放了内存
		free(ptPre);
	}
	ptCur=(struct TinyQuotationStruct *)p->S0Q.pTail;
	if(ptCur!=NULL){
		for(int i=0;i<10;i++){
			pD31Ex->nTenBidVolume+=	ptCur->nBidVol[i];
			pD31Ex->nTenAskVolume+=	ptCur->nAskVol[i];
			pD31Ex->lTenBidAmnt+=	(int8b)ptCur->nBidVol[i]*ptCur->nBidPrice[i]/100;
			pD31Ex->lTenAskAmnt+=	(int8b)ptCur->nAskVol[i]*ptCur->nAskPrice[i]/100;
		}
		pD31Ex->nLastClose=	ptCur->nPreClose;
		pD31Ex->nCurPrice=	ptCur->nMatch;

		pD31Ex->nTotalBidVolume=ptCur->nTotalBidVol;
		pD31Ex->nTotalAskVolume=ptCur->nTotalAskVol;
		pD31Ex->lTotalBidAmnt=	ptCur->nTotalBidVol*ptCur->nWtAvgBidPrice/100;
		pD31Ex->lTotalAskAmnt=	ptCur->nTotalAskVol*ptCur->nWtAvgAskPrice/100;

		pD31Ex->nWtAvgBidPrice=	ptCur->nWtAvgBidPrice;
		pD31Ex->nWtAvgAskPrice=	ptCur->nWtAvgAskPrice;

		p->iSamplingCnt++;
		p->lAddupSamplingBidAmnt+=pD31Ex->lTotalBidAmnt;
		p->lAddupSamplingAskAmnt+=pD31Ex->lTotalAskAmnt;

		pD31Ex->lAvgTotalBidAmnt=p->lAddupSamplingBidAmnt/p->iSamplingCnt;
		pD31Ex->lAvgTotalAskAmnt=p->lAddupSamplingAskAmnt/p->iSamplingCnt;

/*
		if(p->iQuotationCnt<0){
			pD31Ex->lAvgTotalBidAmnt=(p->lAddupTotalBidAmnt-p->lLastTotalBidAmnt)/(p->iQuotationCnt-1);
			pD31Ex->lAvgTotalAskAmnt=(p->lAddupTotalAskAmnt-p->lLastTotalAskAmnt)/(p->iQuotationCnt-1);
		}
		else{
			pD31Ex->lAvgTotalBidAmnt=p->lAddupTotalBidAmnt/p->iQuotationCnt;
			pD31Ex->lAvgTotalAskAmnt=p->lAddupTotalAskAmnt/p->iQuotationCnt;
		}
*/
		//释放最后一个节点
		free(ptCur);
	}

	//将LISTHEAD清空
	p->S0Q.pHead=p->S0Q.pTail=NULL;
	return 0;
}

int WriteD31Stat(FILE *fp,struct IndexStatStruct *p)
{
	int i;
	struct D31IndexItemStruct *pD31;

	fprintf(fp,"code=%06d,t=%09d,zd,zb\n",p->iStockCode,p->nT0);


	pD31=&(p->Zb);
	for(i=0;i<MAX_LEVEL_CNT;i++)
		fprintf(fp,"zb:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n",
			alAmntLevel[i]/1000000,
			pD31->alBidAmount[i],pD31->aiBidVolume[i],pD31->aiBidOrderNum[i],
			pD31->alAskAmount[i],pD31->aiAskVolume[i],pD31->aiAskOrderNum[i]);

	bzero((void*)pD31,sizeof(struct D31IndexItemStruct));

	pD31=&(p->Zd);
	for(i=0;i<MAX_LEVEL_CNT;i++)
		fprintf(fp,"zd:i=%-5ld\tbm=%-10ld\tbv=%-6d\tbo=%d\tam=%-10ld\tav=%-6d\tao=%d\n",
			alAmntLevel[i]/1000000,
			pD31->alBidAmount[i],pD31->aiBidVolume[i],pD31->aiBidOrderNum[i],
			pD31->alAskAmount[i],pD31->aiAskVolume[i],pD31->aiAskOrderNum[i]);

	bzero((void*)pD31,sizeof(struct D31IndexItemStruct));

	return 0;
}
int WriteD31Stat1(FILE *fp,struct IndexStatStruct *p,int iWriteFlag)
{
	int i;
//	fprintf(fp,"code=%06d,t=%09d,zd,zb\n",p->iStockCode,p->nT0);
//	$sz_code $prefix $tmp_time ${arr_level[i]} $bid_amnt $bid_volume $bid_num $ask_amnt $ask_volume $ask_num

	if(iWriteFlag==1){
		struct D31IndexItemStruct *pD31;
		pD31=&p->Zb;
		for(i=0;i<MAX_LEVEL_CNT;i++){
			fprintf(fp,"%-6d,%s,%-4d,%-4d,%-10ld,%-6d,%-6d,%-10ld,%-6d,%-6d\n",
				p->iStockCode,"z",p->nT0/100000,(int)(alAmntLevel[i]/1000000),
				pD31->alBidAmount[i],pD31->aiBidVolume[i],pD31->aiBidOrderNum[i],
				pD31->alAskAmount[i],pD31->aiAskVolume[i],pD31->aiAskOrderNum[i]);
		}
		pD31=&p->Zd;
		for(i=0;i<MAX_LEVEL_CNT;i++){
			fprintf(fp,"%-6d,%s,%-4d,%-4d,%-10ld,%-6d,%-6d,%-10ld,%-6d,%-6d\n",
				p->iStockCode,"w",p->nT0/100000,(int)(alAmntLevel[i]/1000000),
				pD31->alBidAmount[i],pD31->aiBidVolume[i],pD31->aiBidOrderNum[i],
				pD31->alAskAmount[i],pD31->aiAskVolume[i],pD31->aiAskOrderNum[i]);

		}
	}
	if(iWriteFlag==2){
		struct D31IndexExtStruct  *pEx=&p->Ex;
		fprintf(fp,"%-6d,%s,%-4d,%-10d,%-10d,%-12ld,%-12ld,%-10d,%-10d,%-12ld,%-12ld,\
%-10d,%-10d,%-10d,%-10d,%-12ld,%-12ld,%-12ld,%-12ld,%-12ld,%-12ld,%-12ld,%-12ld\n",
			p->iStockCode,"e",p->nT0/100000,
			pEx->nTenBidVolume,	//十档买量（手）
			pEx->nTenAskVolume,	//十档卖量（手）
			pEx->lTenBidAmnt,	//十档买额（分）
			pEx->lTenAskAmnt,	//十档卖额（分）
			pEx->nTotalBidVolume,	//叫买总量（手）
			pEx->nTotalAskVolume,	//叫卖总量（手）
			pEx->lTotalBidAmnt,	//叫买总额（分）
			pEx->lTotalAskAmnt,	//叫卖总额（分）
			pEx->nWtAvgBidPrice,	//加权平均叫买价（分）
			pEx->nWtAvgAskPrice,	//加权平均叫卖价（分）
			pEx->nLastClose,	//昨收盘价
			pEx->nCurPrice,		//最新价
			pEx->lAvgTotalBidAmnt,	//平均叫买总额（当日平均，分）
			pEx->lAvgTotalAskAmnt,	//平均叫卖总额（当日平均，分）
			pEx->alBidAmount[0],	//跳买额度20w，单位（分）
			pEx->alAskAmount[0],	//跳卖额度20w，单位（分）
			pEx->alBidAmount[1],	//跳买额度50w，单位（分）
			pEx->alAskAmount[1],	//跳卖额度50w，单位（分）
			pEx->alBidAmount[2],	//跳买额度100w，单位（分）
			pEx->alAskAmount[2]	//跳卖额度100w，单位（分）
			);

	}

	if(iWriteFlag==3){
		//申明为静态的，就不需要每次都bzero了
		static struct D31ItemStruct t;
		struct D31IndexItemStruct *pZb=&p->Zb;
		struct D31IndexItemStruct *pZd=&p->Zd;
		struct D31IndexExtStruct  *pEx=&p->Ex;

		char sTempTime[15];
		time_t tTempTime;
		
		long lTime;

		lTime=nGetHostTime();
		
		sprintf(sTempTime,"%d%06d",p->nActionDay,p->nT0/1000);
		tTempTime=tGetTime(sTempTime);

		t.nStockCode=(unsigned int) p->iStockCode;
		t.nTradeTime=(unsigned int) tTempTime;

		for(i=0;i<MAX_LEVEL_CNT;i++){
			t.afZbBidAmount[i]=(float)pZb->alBidAmount[i]/1000000;
			t.afZbBidVolume[i]=(float)pZb->aiBidVolume[i]/100;
			t.anZbBidOrderNum[i]=(unsigned short int)pZb->aiBidOrderNum[i];
			t.afZbAskAmount[i]=(float)pZb->alAskAmount[i]/1000000;
			t.afZbAskVolume[i]=(float)pZb->aiAskVolume[i]/100;
			t.anZbAskOrderNum[i]=(unsigned short int)pZb->aiAskOrderNum[i];

			t.afZdBidAmount[i]=(float)pZd->alBidAmount[i]/1000000;
			t.afZdBidVolume[i]=(float)pZd->aiBidVolume[i]/100;
			t.anZdBidOrderNum[i]=(unsigned short int)pZd->aiBidOrderNum[i];
			t.afZdAskAmount[i]=(float)pZd->alAskAmount[i]/1000000;
			t.afZdAskVolume[i]=(float)pZd->aiAskVolume[i]/100;
			t.anZdAskOrderNum[i]=(unsigned short int)pZd->aiAskOrderNum[i];
		}
		t.fTenBidVolume=	(float)pEx->nTenBidVolume/100;
		t.fTenAskVolume=	(float)pEx->nTenAskVolume/100;
		t.fTenBidAmnt=		(float)pEx->lTenBidAmnt/1000000;
		t.fTenAskAmnt=		(float)pEx->lTenAskAmnt/1000000;
		t.fTotalBidVolume=	(float)pEx->nTotalBidVolume;
		t.fTotalAskVolume=	(float)pEx->nTotalAskVolume;
		t.fTotalBidAmnt=	(float)pEx->lTotalBidAmnt/1000000;
		t.fTotalAskAmnt=	(float)pEx->lTotalAskAmnt/1000000;
		t.fWtAvgBidPrice=	(float)pEx->nWtAvgBidPrice/10000;
		t.fWtAvgAskPrice=	(float)pEx->nWtAvgAskPrice/10000;
		t.fLastClose=		(float)pEx->nLastClose/10000;
		t.fCurPrice=		(float)pEx->nCurPrice/10000;
		t.fAvgTotalBidAmnt=	(float)pEx->lAvgTotalBidAmnt/1000000;
		t.fAvgTotalAskAmnt=	(float)pEx->lAvgTotalAskAmnt/1000000;
		
		t.fBidAmount20=		(float)pEx->alBidAmount[0]/1000000;
		t.fAskAmount20=         (float)pEx->alAskAmount[0]/1000000;
		t.fBidAmount50=         (float)pEx->alBidAmount[1]/1000000;
		t.fAskAmount50=         (float)pEx->alAskAmount[1]/1000000;
		t.fBidAmount100=	(float)pEx->alBidAmount[2]/1000000;
		t.fAskAmount100=        (float)pEx->alAskAmount[2]/1000000;
		
		fwrite((const void*)&lTime,sizeof(lTime),1,fp);
		fwrite((const void*)&t,sizeof(t),1,fp);
		fflush(fp);
	}
	bzero((void*)&p->Zb,sizeof(struct D31IndexItemStruct));
	bzero((void*)&p->Zd,sizeof(struct D31IndexItemStruct));
	bzero((void*)&p->Ex,sizeof(struct D31IndexExtStruct));
	return 0;
}

struct IndexStatStruct *GetIndexStat(int iStockCode,char sFileName[],long lCurPos,
	int nBgnActionDay,int nPreT0,int nT0)
{
	struct IndexStatStruct *pTemp;

	//检查股票代码的合法性
	if(iStockCode<=0||iStockCode>=MAX_STOCK_CODE){
		printf("error stockcode file=%s,pos=%ld,stockcode=%06d\n",
			sFileName,lCurPos,iStockCode);
		return NULL;
	}

	if((pTemp=AISTAT[iStockCode])==NULL){
		if((AISTAT[iStockCode]=NewInitIndexStat(iStockCode,
			nBgnActionDay,nPreT0,nT0))==NULL){
			printf("error new_init_index_stat file=%s,pos=%ld\n",
				sFileName,lCurPos);
			return NULL;
		}
		pTemp=AISTAT[iStockCode];

		//新生成节点，插入到全局结构表中
		InsertList((LIST**)&INDEX_HEAD,(LIST*)pTemp);
	}
	return pTemp;
}

int InitEtfList(char sFileName[],int iEtfCode,LISTHEAD *ptHead)
{
	FILE *fp;
	char sBuffer[256];
	struct EtfStockStruct *pTemp;
	
	if((fp=fopen(sFileName,"r"))==NULL){
		printf("error open file %s to read.\n",sFileName);
		return -1;
	}
	
	//char *fgets(char *buf, int bufsize, FILE *stream);
	
	while(1){
		fgets(sBuffer,256,fp);
		
		if(feof(fp)) break;
			
		if((pTemp=(struct EtfStockStruct *)malloc(sizeof(struct EtfStockStruct)))==NULL){
			printf("malloc EtfStockStruct error.\n");
			fclose(fp);
			return -1;
		}
		bzero((void*)pTemp,sizeof(struct EtfStockStruct));
		
		pTemp->iEtfCode=iEtfCode;
		pTemp->iStockCode=atoi(sBuffer);
		pTemp->pNext=NULL;

		Append2List(ptHead,(LIST*)pTemp);
	}
	
	
	fclose(fp);
	return 0;
}
struct IndexStatStruct *GetEtfIndexStat(int iEtfCode,char sEtfPath[],
	int nBgnActionDay,int nPreT0,int nT0)
{
	char sFileName[512];
	struct IndexStatStruct *pTemp;

	//检查股票代码的合法性
	if(iEtfCode<=0||iEtfCode>=MAX_STOCK_CODE){
		printf("error etfcode etfcode=%06d\n",iEtfCode);
		return NULL;
	}

	if((pTemp=AISTAT[iEtfCode])==NULL){
		if((AISTAT[iEtfCode]=NewInitIndexStat(iEtfCode,
			nBgnActionDay,nPreT0,nT0))==NULL){
			printf("error new_init_index_stat etfcode=%d\n",iEtfCode);
			return NULL;
		}
		pTemp=AISTAT[iEtfCode];

		sprintf(sFileName,"%s/%06d.etf",sEtfPath,iEtfCode);
		
		if(InitEtfList(sFileName,iEtfCode,&pTemp->ETF)<0) return NULL;

		//新生成节点，插入到全局结构表中
		InsertList((LIST**)&INDEX_ETF,(LIST*)pTemp);
	}
	return pTemp;
}

int InitIndexEtfList(char sEtfList[],char sEtfPath[])
{
	int iEtfCode;
	char *p=&sEtfList[0],sFileName[512];
	struct IndexStatStruct *pTemp;

	while(1){
		iEtfCode=atoi(p);
		
		//检查股票代码的合法性
		if(iEtfCode<=0||iEtfCode>=MAX_STOCK_CODE){
			printf("error etfcode etfcode=%06d\n",iEtfCode);
			return -1;
		}

		if((pTemp=(struct IndexStatStruct *)malloc(
			sizeof(struct IndexStatStruct)))==NULL){
			printf("malloc IndexStatStruct error.\n");
			return -1;
		}
		bzero((void*)pTemp,sizeof(struct IndexStatStruct));
		pTemp->iStockCode=iEtfCode;
		AISTAT[iEtfCode]=pTemp;

		sprintf(sFileName,"%s/%06d.etf",sEtfPath,iEtfCode);
		
		if(InitEtfList(sFileName,iEtfCode,&pTemp->ETF)<0) return -1;

		//新生成节点，插入到全局结构表中
		InsertList((LIST**)&INDEX_ETF,(LIST*)pTemp);

		if((p=strchr(p,','))==NULL) break;
		
		//跳过','号到下一个代码
		p++;
	}
	return 0;
}

int MoveS1T2S0T(struct IndexStatStruct *p,int nPreT0,int nT0)
{
	LISTHEAD *pS0T=&(p->S0T),*pS1T=&(p->S1T);
	struct TinyTransactionStruct *ptHead,*pTemp;

	//如果是第一秒开始，则释放S0T

	if((nT0%10000)/1000==1) Destroy2List(pS0T);

	ptHead=(struct TinyTransactionStruct *)pS1T->pHead;

	pS1T->pHead=pS1T->pTail=NULL;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		if(MY_BELONG_TRANSACTION_T0(pTemp->nTime,nT0)){
			
//		if(pTemp->nTime<nT0||
//			(pTemp->nTime==MY_CLOSE_MARKET_TIME&&nT0==MY_CLOSE_MARKET_TIME)){
/***
			填写一下以下4个字段
			int	nAskOrderSeq;	//卖方委托单成交序号，卖成笔数
			int	nBidOrderSeq;	//买方委托单成交序号，买成笔数
			struct TinyOrderStruct *pAskOrder; //叫卖订单指针
			struct TinyOrderStruct *pBidOrder; //叫买订单指针

***/
			if(InitTinyTransactionField(p,pTemp)<0) return -1;

			Append2List(pS0T,(LIST*)pTemp);
		}
		else	Append2List(pS1T,(LIST*)pTemp);
	}
	p->nT0=		nT0;
	p->nPreT0=	nPreT0;
	
	return 0;

}
void MoveS1Q2S0Q(struct IndexStatStruct *p,int nPreT0,int nT0)
{
	LISTHEAD *pS0Q=&(p->S0Q),*pS1Q=&(p->S1Q);
	struct TinyQuotationStruct *ptHead,*pTemp;

	ptHead=(struct TinyQuotationStruct *)pS1Q->pHead;

	pS1Q->pHead=pS1Q->pTail=NULL;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		if(pTemp->nTime<=nT0)
			Append2List(pS0Q,(LIST*)pTemp);
		else	Append2List(pS1Q,(LIST*)pTemp);
	}
}

int MoveS1O2M_ORDER(struct IndexStatStruct *p,int nT0)
{
	LISTHEAD *pS1O=&(p->S1O);
	struct TinyOrderStruct *ptHead,*pTemp,*pTmp;

	ptHead=(struct TinyOrderStruct *)pS1O->pHead;

	pS1O->pHead=pS1O->pTail=NULL;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		//如果还是下一个时段的，则继续放到原链表中
		if(!MY_BELONG_TRANSACTION_T0(pTemp->nTime,nT0)){
//		if(pTemp->nTime>=nT0){
			Append2List(pS1O,(LIST*)pTemp);
			continue;
		}
		//如果树中已有这个订单号，则直接合并入，并释放内存
		if(SearchBin(p->M_ORDER,(void*)pTemp,
			data_search_bintree_stock_code_order,
			(void**)&pTmp)==FOUND){
			pTmp->nVolume+=		pTemp->nVolume;
			//考虑到统计的笔数，其实是统计多少个订单所以这里不累计OrderNo
			//pTemp->nOrderNum+=	p->nOrderNum;
			pTmp->lOrderAmnt+=	pTemp->lOrderAmnt;
			free(pTemp);
			continue;
		}

		//将链表头的指针带入
		pTemp->pNext=(struct TinyOrderStruct*)&(p->S0O);
		if(InsertBin(&p->M_ORDER,(void *)pTemp,
			data_search_bintree_stock_code_order,
			assign_insert_bintree_stock_code_order_e)<0){
			printf("insert bin malloc error th.\n");
			return -1;
		}
	}
	return 0;
}
void SumIndexStatZx(struct D31IndexItemStruct *po,struct D31IndexItemStruct *pi)
{
	/**
	int8b	alBidAmount[MAX_LEVEL_CNT];	//主买额度，单位（分）
	int	aiBidVolume[MAX_LEVEL_CNT];	//主买量，单位（手）
	int	aiBidOrderNum[MAX_LEVEL_CNT];	//主买笔数，单位（笔）
	int8b	alAskAmount[MAX_LEVEL_CNT];	//主卖额度，单位（分）
	int	aiAskVolume[MAX_LEVEL_CNT];	//主卖量，单位（手）
	int	aiAskOrderNum[MAX_LEVEL_CNT];	//主卖笔数，单位（笔）
	**/
	int i;
	
	for(i=0;i<MAX_LEVEL_CNT;i++){
		po->alBidAmount[i]+=	pi->alBidAmount[i];
		po->aiBidVolume[i]+=	pi->aiBidVolume[i];
		po->aiBidOrderNum[i]+=	pi->aiBidOrderNum[i];
		po->alAskAmount[i]+=	pi->alAskAmount[i];
		po->aiAskVolume[i]+=	pi->aiAskVolume[i];
		po->aiAskOrderNum[i]+=	pi->aiAskOrderNum[i];
	}
}
void SumIndexStatEx(struct D31IndexExtStruct *po,struct D31IndexExtStruct *pi)
{
	int i;

	//上半部分指标开始
	po->nTenBidVolume+=	pi->nTenBidVolume;
	po->nTenAskVolume+=	pi->nTenAskVolume;
	po->lTenBidAmnt+=	pi->lTenBidAmnt;
	po->lTenAskAmnt+=	pi->lTenAskAmnt;
	po->nTotalBidVolume+=	pi->nTotalBidVolume;
	po->nTotalAskVolume+=	pi->nTotalAskVolume;
	po->lTotalBidAmnt+=	pi->lTotalBidAmnt;
	po->lTotalAskAmnt+=	pi->lTotalAskAmnt;
	po->nWtAvgBidPrice+=	pi->nWtAvgBidPrice;
	po->nWtAvgAskPrice+=	pi->nWtAvgAskPrice;

	//下半部分指标开始
	po->nLastClose+=		pi->nLastClose;
	po->nCurPrice+=		pi->nCurPrice;
	po->lAvgTotalBidAmnt+=	pi->lAvgTotalBidAmnt;
	po->lAvgTotalAskAmnt+=  pi->lAvgTotalAskAmnt;

	for(i=0;i<MAX_JMP_LEVEL_CNT;i++){
		po->alBidAmount[i]+=	pi->alBidAmount[i];
		po->alAskAmount[i]+=	pi->alAskAmount[i];
	}
}
//将ETF列表中的数据，汇总到ETF结构中去
int SumEtfStat(struct IndexStatStruct *p)
{
	struct EtfStockStruct *pTemp=(struct EtfStockStruct *)(p->ETF.pHead);
	struct IndexStatStruct *pi;
	
	//设置一下全局参数
	if(pTemp!=NULL){
		
		if((pi=AISTAT[pTemp->iStockCode])==NULL) return -1;
	
		if(p->nT0!=pi->nT0){
			p->nActionDay=	pi->nActionDay;
			p->nPreT0=	pi->nPreT0;
			p->nT0=		pi->nT0;
		}
	}

	while(pTemp!=NULL){
		
		if((pi=AISTAT[pTemp->iStockCode])==NULL) return -1;
		
		SumIndexStatZx(&p->Zb,&pi->Zb);
		SumIndexStatZx(&p->Zd,&pi->Zd);
		SumIndexStatEx(&p->Ex,&pi->Ex);
		
		pTemp=pTemp->pNext;		
	}
	
	return 0;
}
int GenD31StatAll()
{
	struct IndexStatStruct *pIndexStat=INDEX_HEAD;

	while(pIndexStat!=NULL){
		if(GenD31Stat(pIndexStat)<0){
			printf("gen d31 state error.\n");
			return -1;
		}
		pIndexStat=pIndexStat->pNext;
	}
	
	//做一个循环，统计ETF数据
	pIndexStat=INDEX_ETF;
	while(pIndexStat!=NULL){
		SumEtfStat(pIndexStat);
		pIndexStat=pIndexStat->pNext;		
	}
	
	return 0;
}
int WriteD31StatList(FILE *fpD31,char sCodeStr[],int iWriteFlag,
	struct IndexStatStruct *pIndexStat)
{
	char sTempCode[8];
	while(pIndexStat!=NULL){

		sprintf(sTempCode,"%06d",pIndexStat->iStockCode);
		//输出所有的票，或者指定票在设置的列表中
		if(strlen(sCodeStr)==0||
			strstr(sCodeStr,sTempCode)!=NULL){

			if(WriteD31Stat1(fpD31,pIndexStat,iWriteFlag)<0){
				printf("write d31 error.\n");
				return -1;
			}
		}

		pIndexStat=pIndexStat->pNext;
	}
	return 0;
}
int WriteD31StatAll(FILE *fpD31,char sCodeStr[],int iWriteFlag)
{
	if(WriteD31StatList(fpD31,sCodeStr,iWriteFlag,INDEX_HEAD)<0) return -1;
	if(WriteD31StatList(fpD31,sCodeStr,iWriteFlag,INDEX_ETF)<0) return -1;

	return 0;
}
//将所有预加载的T0之前的数据，放到统计缓存中
int AddPreT0Data2Ready(int nPreT0,int nT0)
{
	struct IndexStatStruct *pIndexStat=INDEX_HEAD;

	while(pIndexStat!=NULL){

		//这里必须先将订单数据加到查找树中，因为后一个函数需要查找引用		
		if(MoveS1O2M_ORDER(pIndexStat,nT0)<0) return -1;

		//将T1的交易数据，放到T0列表，并设置好InitTinyTransactionField各字段
		if(MoveS1T2S0T(pIndexStat,nPreT0,nT0)<0) return -1;

		MoveS1Q2S0Q(pIndexStat,nPreT0,nT0);


		pIndexStat=pIndexStat->pNext;
	}
	return 0;
}
