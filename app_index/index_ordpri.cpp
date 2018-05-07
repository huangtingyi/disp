
#include "index_supp.h"

/**
10.00卖单成交，说明委托卖单委托价 <=10.00元，当发现成交价>=10.00元 且卖单委托单号大于 该成交单号的交易，则关闭该成交单；
10.00的买单成交，说明委托买单的委托价>=10元，当发现成交价<=10.00元 且买单委托单号大于 该成交单号的交易，则关闭该成交单。
	int nAskDownPrice;	//卖出最低成交价,单位（分）
	int nBidUpPrice;	//买入最高成交价,单位（分）
	int nMaxAskOrder;	//卖出最大订单号，对应被动盘最低价
	int nMaxBidOrder;	//买入最大订单号，对应被动盘最高价
**/
int data_search_bintree_price_order(void *pValue,void*pData)
{
	return ((struct PriceOrderStruct *)pValue)->nPrice-
		((struct PriceOrderStruct *)pData)->nPrice;
}
void assign_insert_bintree_price_order(void **ppData,void *pData)
{
	*ppData=pData;
}


//订单合并到交易合并订单树中
int AddPriceOrder2Tree(BINTREE **PP,int *piCnt,struct PriceOrderStruct *p)
{
	struct PriceOrderStruct *pTemp;

	if(SearchBin(*PP,p,data_search_bintree_price_order,
		(void**)&pTemp)==NOTFOUND){

		if((pTemp=(struct PriceOrderStruct*)malloc(
			sizeof(struct PriceOrderStruct)))==NULL){
			printf("malloc PriceOrderStruct error.\n");
			return -1;
		}
		memcpy((void*)pTemp,(void*)p,sizeof(struct PriceOrderStruct));

		if(InsertBin(PP,(void *)pTemp,
			data_search_bintree_price_order,
			assign_insert_bintree_price_order)<0){
			printf("insert bin malloc error th.\n");
			return -1;
		}
		(*piCnt)++;
	}
	else{
		if(pTemp->nOrder<p->nOrder){
			pTemp->nOrder=p->nOrder;
			pTemp->nOrderTime=p->nOrderTime;
		}
	}

	return 0;
}
int AdjustIndexStatS0(struct IndexStatStruct *p,struct TinyTransactionStruct *pt)
{
	struct PriceOrderStruct t;

	t.nPrice=pt->nPrice;
	t.nOrderTime=pt->nTime;

	t.nOrder=pt->nAskOrder;


	if(AddPriceOrder2Tree(&(p->ASK_MAX),&p->iAskMaxCnt,&t)<0) return -1;

	t.nOrder=pt->nBidOrder;

	if(pt->iStockCode==601168&&t.nOrder==179900&&t.nPrice==69300){
		printf("hello 4\n");
	}

	if(AddPriceOrder2Tree(&(p->BID_MAX),&p->iBidMaxCnt,&t)<0) return -1;

	return 0;
}
int data_search_bintree_price_order_1(void *pValue,void*pData)
{
	return ((struct TinyOrderStruct *)pValue)->nPrice-
		((struct PriceOrderStruct *)pData)->nPrice;
}
int iErrorFlag=0;
int IsMyLeave(BINTREE *p)
{
	if(p->pLeft==NULL&&p->pRight==NULL) return true;
	return false;
}
int GetTreeHigh(BINTREE *p)
{
	int iLeft,iRight;

	if(p==NULL) return 0;

	iLeft=GetTreeHigh(p->pLeft)+1;
	iRight=GetTreeHigh(p->pRight)+1;

	if(iLeft>iRight) return iLeft;

	return iRight;
}
int check_my_balance(BINTREE *ptHead)
{
	int iLeft,iRight;

	iLeft=GetTreeHigh(ptHead->pLeft);
	iRight=GetTreeHigh(ptHead->pRight);

	if(ptHead->diff!=(iRight-iLeft)){
		iErrorFlag=1;
		printf("logic error 7 left=%d,right=%d diff=%d head=%p.\n",iLeft,iRight,ptHead->diff,(void*)ptHead);
	}

	return 0;
}
int TravelMyBin(BINTREE *ptHead, int ( *pOperation)(BINTREE *))
{
	int j=0;

	if(ptHead==NULL) return 0;

	if(ptHead->pLeft!=NULL)
		j+=TravelMyBin(ptHead->pLeft,pOperation);
	j+=(*pOperation)(ptHead);
	if(ptHead->pRight!=NULL)
		j+=TravelMyBin(ptHead->pRight,pOperation);
	return j;
}
//遍历S0O,根据原则，删除订单数据
void DeleteCloseOrder(struct IndexStatStruct *p,int nLastTime)
{
        struct TinyOrderStruct *ptHead,*pTemp,*pTinyOrder;
        struct PriceOrderStruct *pPriceOrder;
        BINTREE *pBin,*BS_MAX;

        LISTHEAD *pS0O=&(p->S0O);

        ptHead=(struct TinyOrderStruct *)pS0O->pHead;

        pS0O->pHead=pS0O->pTail=NULL;

        while(ptHead!=NULL){
        	pTemp=ptHead;
        	ptHead=ptHead->pNext;

        	//如果时间是在某个时间段之内的，则不删除
        	if(pTemp->nTime>nLastTime){
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
        		continue;
		}
        	if(SearchBin(p->M_ORDER,pTemp,data_search_bintree_stock_code_order,
			(void**)&pTinyOrder)==NOTFOUND){
			printf("logic error search.\n");
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
			continue;
		}

		if(pTinyOrder->nBSFlag=='B')
			BS_MAX=p->BID_MAX;
		else if(pTinyOrder->nBSFlag=='S')
			BS_MAX=p->ASK_MAX;
		else	BS_MAX=NULL;

		//如果未找到，可能是本价格还未加入
		if(SearchBin(BS_MAX,pTemp,data_search_bintree_price_order_1,
			(void**)&pPriceOrder)==NOTFOUND){
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
			continue;
		}

		//同样价格、同样方向，有更大订单号成交，则本订单号放弃
		if(pTemp->nOrder>=pPriceOrder->nOrder){
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
			continue;
		}
		//如果这个成交最大订单的时间还没过期，则继续放在内存中
		if(pPriceOrder->nOrderTime>=nLastTime){
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
			continue;
		}

		if(pTemp->nOrder==16590){
			printf("hello 1.\n");
			if(pTemp->nPrice==1010000&&
				pTemp->nOrder==16590){
				printf("hello 2.\n");
			}
		}

//		TravelMyBin(p->M_ORDER,check_my_balance);

		DeleteBin(&(p->M_ORDER),(void*)pTemp,
			data_search_bintree_stock_code_order,&pBin);

//		TravelMyBin(p->M_ORDER,check_my_balance);

		if(iErrorFlag==1){
			printf("hello world.------------------------------------%d,%d.\n",pTemp->iStockCode,pTemp->nOrder);
			exit(1);
		}

		if(pBin==NULL){
			printf("logic error search.\n");
			pTemp->pNext=NULL;
			Append2List(pS0O,(LIST*)pTemp);
			continue;
		}

		p->iFreeS0OCnt++;
		p->iS0OCnt--;

		free(pBin);
		free(pTemp);
	}
}

