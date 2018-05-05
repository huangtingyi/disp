
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
struct IndexStatStruct *INDEX_HEAD=NULL;
struct IndexStatStruct *AISTAT[MAX_STOCK_CODE];


void TDF_TRANSACTION2TinyTransaction(struct TDF_TRANSACTION *pi,struct TinyTransactionStruct *po)
{
	po->pNext=NULL;
//	po->pAskOrder=po->pBidOrder=NULL;
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

//	po->nAskJmpSeq=0;
//	po->nBidJmpSeq=0;
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

	po->nLastPrice=0;
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

	po->nLastPrice=0;
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

//�����ϲ������׺ϲ���������
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

		pTemp->nVolume+=	p->nVolume;
		//���ǵ�ͳ�Ƶı�������ʵ��ͳ�ƶ��ٸ������������ﲻ�ۼ�OrderNo
		//pTemp->nOrderNum+=	p->nOrderNum;
		pTemp->lOrderAmnt+=	p->lOrderAmnt;

	}
	return 0;
}

//�����ϲ������׺ϲ������б���
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

	if(pi->nTime<nT0||
		(nT0==MY_CLOSE_MARKET_TIME&&pi->nTime==MY_CLOSE_MARKET_TIME)){

		//������ͷ��ָ�����
		os.pNext=(struct TinyOrderStruct*)&(pIndexStat->S0O);
		//���������ϲ������׺ϲ���������
		if(AddTinyOrder2Tree(&(pIndexStat->M_ORDER),&pIndexStat->iS0OCnt,&os)<0){
			printf("error add ask tiny order to tree\n");
			return -1;
		}

		//������ͷ��ָ�����
		//ob.pNext=&(pIndexStat->PreS0M);
		ob.pNext=(struct TinyOrderStruct*)&(pIndexStat->S0O);
		//���붩���ϲ������׺ϲ���������
		if(AddTinyOrder2Tree(&(pIndexStat->M_ORDER),&pIndexStat->iS0OCnt,&ob)<0){
			printf("error add bid tiny order to tree \n");
			return -1;
		}
	}
	else{
		//�����������ӵ����׺ϲ�����������
		if(AddTinyOrder2List(&(pIndexStat->S1O),&os)<0){
			printf("error add ask tiny order to list\n");
			return -1;
		}
		//���붩�����ӵ����׺ϲ�����������
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

		//ֻͳ��ί�е��ĵ�һ�ʳɽ�
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

		//ֻͳ��ί�е��ĵ�һ�ʳɽ�
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

		//ֻͳ��ί�е��ĵ�һ�ʳɽ�
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

		//ֻͳ��ί�е��ĵ�һ�ʳɽ�
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
/***
void InsertJmpOrder(struct TinyOrderStruct **pptHead,struct TinyOrderStruct *p)
{
	p->pJmpOrder= *pptHead;
	*pptHead=p;
}
void InsertAskOrderTransaction(struct TinyTransactionStruct **pptHead,struct TinyTransactionStruct *p)
{
	p->pAskOrder= *pptHead;
	*pptHead=p;
}
void InsertBidOrderTransaction(struct TinyTransactionStruct **pptHead,struct TinyTransactionStruct *p)
{
	p->pBidOrder= *pptHead;
	*pptHead=p;
}
***/
//b)����S0T�б��У�ѭ������M_ORDER���У����� D31IndexItem���ݣ�
int GenD31Stat(struct IndexStatStruct *p)
{
	struct D31IndexItemStruct *pD31Zd=&(p->Zd),*pD31Zb=&(p->Zb);
	struct D31IndexExtStruct *pD31Ex=&p->Ex;
	BINTREE *M_ORDER=p->M_ORDER;

	struct TinyOrderStruct *pAskOrder,*pBidOrder;
	struct TinyTransactionStruct *ptHead,*pTemp;

	struct TinyQuotationStruct *ptPre,*ptCur;

	ptHead=(struct TinyTransactionStruct *)p->S0T.pHead;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		//�ҵ������������
		if(SearchBin(M_ORDER,(void*)&(pTemp->nAskOrder),
			data_search_bintree_order_2,(void**)&pAskOrder)==NOTFOUND){
			printf("hello 3 code=%d,askord=%d time=%d,flag=%c\n",
				p->iStockCode,pTemp->nAskOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
			printf("hello 2\n");
			return -1;
		}
		//�ҵ��򷽶������
		if(SearchBin(M_ORDER,(void*)&(pTemp->nBidOrder),
			data_search_bintree_order_2,(void**)&pBidOrder)==NOTFOUND){

			printf("hello 3 code=%d,bidord=%d time=%d,flag=%c\n",
				p->iStockCode,pTemp->nBidOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
			return -1;
		}

		//����߼��Ժ�Ҫ�Ƶ������߼���
		pAskOrder->nOrderSeq++;
		pBidOrder->nOrderSeq++;

		pTemp->nAskOrderSeq=pAskOrder->nOrderSeq;
		pTemp->nBidOrderSeq=pBidOrder->nOrderSeq;

		if(pTemp->nAskOrder==1691843&&pTemp->nBidOrder==1732854){
			printf("shenme luoji error.\n");
		}
		if(pTemp->nAskOrder==374174&&pTemp->nBidOrder==371812){
			printf("shenme luoji error =============2.\n");
		}
		//ͳ������Ϣ
		AskOrder2D31(pAskOrder,pTemp,pD31Zd,p);
		BidOrder2D31(pBidOrder,pTemp,pD31Zd);

		if(pD31Zd->aiBidVolume[0]!=pD31Zd->aiAskVolume[0]&&
			p->nT0==94400000&&p->iStockCode==2908){
			printf("shenqi lulu askord=%d,bidord=%d.\n",
				pTemp->nAskOrder,pTemp->nBidOrder);
		}

		//ͳ�������Ϣ
		if(pTemp->nBSFlag=='S')
			AskOrder2D31Zb(pAskOrder,pTemp,pD31Zb);
		else	BidOrder2D31Zb(pBidOrder,pTemp,pD31Zb);

		//�۸��µ� = (trans['�ɽ��۸�'] < trans['�ɽ��۸�'].shift(1)) &
		//(trans['�������'] == trans['�������'].shift(1))
		if(pTemp->nPrice<pAskOrder->nLastPrice){
			pAskOrder->nAskJmpSeq++;
//			pTemp->nAskJmpSeq=pAskOrder->nAskJmpSeq;
		}
		//�۸����� = (trans['�ɽ��۸�'] > trans['�ɽ��۸�'].shift(1)) &
		//(trans['�������'] == trans['�������'].shift(1))
		if(pTemp->nPrice>pBidOrder->nLastPrice&&
			pBidOrder->nLastPrice!=0){
			pBidOrder->nBidJmpSeq++;
//			pTemp->nBidJmpSeq=pBidOrder->nBidJmpSeq;
		}

		//�������¼�
		pAskOrder->nLastPrice=pBidOrder->nLastPrice=pTemp->nPrice;
	}

	//����һ��ѭ�����������������ı��£������������������ͳ��
	ptHead=(struct TinyTransactionStruct *)p->S0T.pHead;
	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		//�ҵ������������
		if(SearchBin(M_ORDER,(void*)&(pTemp->nAskOrder),
			data_search_bintree_order_2,(void**)&pAskOrder)==NOTFOUND){
			printf("hello 3 code=%d,askord=%d time=%d,flag=%c\n",
				p->iStockCode,pTemp->nAskOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
			printf("hello 2\n");
			return -1;
		}
		//�ҵ��򷽶������
		if(SearchBin(M_ORDER,(void*)&(pTemp->nBidOrder),
			data_search_bintree_order_2,(void**)&pBidOrder)==NOTFOUND){

			printf("hello 3 code=%d,bidord=%d time=%d,flag=%c\n",
				p->iStockCode,pTemp->nBidOrder,pTemp->nTime,(char)(pTemp->nBSFlag));
			return -1;
		}

		//d31_new[����i] = trans[(trans['ί�����'] >= i * 10000) &
		//(trans['��������']>0)].groupby(['time_str'])['�ɽ����'].sum().reindex(index = trade_time)/10000
		if(pAskOrder->nAskJmpSeq>0) AskOrder2D31Ex(pAskOrder,pTemp,pD31Ex);
		//d31_new[����i] = trans[(trans['ί����'] >= i * 10000) &
		//(trans['�������']>0)].groupby(['time_str'])['�ɽ����'].sum().reindex(index = trade_time)/10000
		if(pBidOrder->nBidJmpSeq>0) BidOrder2D31Ex(pBidOrder,pTemp,pD31Ex,p);
	}

	//ȡʱ����ڵ����һ��������д���
	/*****
	//�ϰ벿��ָ�꿪ʼ
	int	nTenBidVolume;			//ʮ���������֣�
	int	nTenAskVolume;			//ʮ���������֣�
	int8b	lTenBidAmnt;			//ʮ�����֣�
	int8b	lTenAskAmnt;			//ʮ������֣�
	int	nTotalBidVolume;		//�����������֣�
	int	nTotalAskVolume;		//�����������֣�
	int8b	lTotalBidAmnt;			//�����ܶ�֣�
	int8b	lTotalAskAmnt;			//�����ܶ�֣�
	int	nWtAvgBidPrice;			//��Ȩƽ������ۣ��֣�
	int	nWtAvgAskPrice;			//��Ȩƽ�������ۣ��֣�
	//�°벿��ָ�꿪ʼ
	int	nLastClose;			//�����̼�
	int	nCurPrice;			//���¼�
	int8b	lAvgTotalBidAmnt;		//ƽ�������ܶ����ƽ�����֣�
	int8b	lAvgTotalAskAmnt;		//ƽ�������ܶ����ƽ�����֣�
	*****/

	ptCur=(struct TinyQuotationStruct *)p->S0Q.pHead;
	//ͳ�����������Լ�ÿ������Ľ����ܶ�����ܶ���ۼ�
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
		//�������һ���ڵ㶼�ͷ����ڴ�
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
		//�ͷ����һ���ڵ�
		free(ptCur);
	}

	//��LISTHEAD���
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
			pEx->nTenBidVolume,	//ʮ���������֣�
			pEx->nTenAskVolume,	//ʮ���������֣�
			pEx->lTenBidAmnt,	//ʮ�����֣�
			pEx->lTenAskAmnt,	//ʮ������֣�
			pEx->nTotalBidVolume,	//�����������֣�
			pEx->nTotalAskVolume,	//�����������֣�
			pEx->lTotalBidAmnt,	//�����ܶ�֣�
			pEx->lTotalAskAmnt,	//�����ܶ�֣�
			pEx->nWtAvgBidPrice,	//��Ȩƽ������ۣ��֣�
			pEx->nWtAvgAskPrice,	//��Ȩƽ�������ۣ��֣�
			pEx->nLastClose,	//�����̼�
			pEx->nCurPrice,		//���¼�
			pEx->lAvgTotalBidAmnt,	//ƽ�������ܶ����ƽ�����֣�
			pEx->lAvgTotalAskAmnt,	//ƽ�������ܶ����ƽ�����֣�
			pEx->alBidAmount[0],	//������20w����λ���֣�
			pEx->alAskAmount[0],	//�������20w����λ���֣�
			pEx->alBidAmount[1],	//������50w����λ���֣�
			pEx->alAskAmount[1],	//�������50w����λ���֣�
			pEx->alBidAmount[2],	//������100w����λ���֣�
			pEx->alAskAmount[2]	//�������100w����λ���֣�
			);

	}

	if(iWriteFlag==3){
		//����Ϊ��̬�ģ��Ͳ���Ҫÿ�ζ�bzero��
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

	//����Ʊ����ĺϷ���
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

		//�����ɽڵ㣬���뵽ȫ�ֽṹ����
		InsertList((LIST**)&INDEX_HEAD,(LIST*)pTemp);
	}
	return pTemp;
}

void MoveS1T2S0T(struct IndexStatStruct *p,int nPreT0,int nT0)
{
	LISTHEAD *pS0T=&(p->S0T),*pS1T=&(p->S1T);
	struct TinyTransactionStruct *ptHead,*pTemp;

	Destroy2List(pS0T);

	ptHead=(struct TinyTransactionStruct *)pS1T->pHead;

	pS1T->pHead=pS1T->pTail=NULL;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;

		if(pTemp->nTime<nT0||
			(pTemp->nTime==MY_CLOSE_MARKET_TIME&&nT0==MY_CLOSE_MARKET_TIME))
			Append2List(pS0T,(LIST*)pTemp);
		else	Append2List(pS1T,(LIST*)pTemp);
	}
	p->nT0=		nT0;
	p->nPreT0=	nPreT0;

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

		//���������һ��ʱ�εģ�������ŵ�ԭ������
		if(pTemp->nTime>=nT0){
			Append2List(pS1O,(LIST*)pTemp);
			continue;
		}
		//�������������������ţ���ֱ�Ӻϲ��룬���ͷ��ڴ�
		if(SearchBin(p->M_ORDER,(void*)pTemp,
			data_search_bintree_stock_code_order,
			(void**)&pTmp)==FOUND){
			pTmp->nVolume+=		pTemp->nVolume;
			//���ǵ�ͳ�Ƶı�������ʵ��ͳ�ƶ��ٸ������������ﲻ�ۼ�OrderNo
			//pTemp->nOrderNum+=	p->nOrderNum;
			pTmp->lOrderAmnt+=	pTemp->lOrderAmnt;
			free(pTemp);
			continue;
		}

		//������ͷ��ָ�����
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
	return 0;
}

int WriteD31StatAll(FILE *fpD31,char sCodeStr[],int iWriteFlag)
{
	char sTempCode[8];
	struct IndexStatStruct *pIndexStat=INDEX_HEAD;

	while(pIndexStat!=NULL){

		sprintf(sTempCode,"%06d",pIndexStat->iStockCode);
		//������е�Ʊ������ָ��Ʊ�����õ��б���
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
void MoveS1X2S0XAll(int nPreT0,int nT0)
{
	struct IndexStatStruct *pIndexStat=INDEX_HEAD;

	while(pIndexStat!=NULL){
		MoveS1T2S0T(pIndexStat,nPreT0,nT0);
		MoveS1Q2S0Q(pIndexStat,nPreT0,nT0);
		pIndexStat=pIndexStat->pNext;
	}
}

int MoveS1O2M_ORDERAll(int nT0)
{
	struct IndexStatStruct *pIndexStat=INDEX_HEAD;

	while(pIndexStat!=NULL){
		if(MoveS1O2M_ORDER(pIndexStat,nT0)<0) return -1;
		pIndexStat=pIndexStat->pNext;
	}
	return 0;
}