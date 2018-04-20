
#include "bintree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*将节点插入到LISTHEAD的头部*/
void Insert2List(LISTHEAD *ptHead,LIST *p)
{
	if(ptHead->pHead==NULL){
		p->pNext=NULL;
		ptHead->pHead=ptHead->pTail=p;
		return;
	}
	
	p->pNext=ptHead->pHead;
	ptHead->pHead=p;
}
/*将节点追加到LISTHEAD的尾部*/
void Append2List(LISTHEAD *ptHead,LIST *p)
{
	if(ptHead->pTail==NULL){
		p->pNext=NULL;
		ptHead->pHead=ptHead->pTail=p;
		return;
	}
	//把表项加入到最后
	ptHead->pTail->pNext=p;
	
	//调整尾指针
	ptHead->pTail=p;
	//确保只有一项
	p->pNext=NULL;
}
/*将整个链表追加到LISTHEAD的尾部*/
void AppendList2List(LISTHEAD *ptHead,LIST *p)
{
	if(ptHead->pTail==NULL){
		ptHead->pHead=p;
		while(p->pNext!=NULL) p=p->pNext;
		ptHead->pTail=p;
		return;
	}
	
	//接上链表
	ptHead->pTail->pNext=p;
	
	//找到尾指针
	while(p->pNext!=NULL) p=p->pNext;
	ptHead->pTail=p;
}

/*统计链表节点数目*/
int CountList(LIST* ptHead)
{
	int iCount=0;

	while(ptHead!=NULL){
		ptHead=ptHead->pNext;
		iCount++;
	}

	return iCount;
}
/*其中operate_list是对单个节点的操作函数*/
/*该函数很有用可以实现打印释放内存等操作*/
int TravelList(LIST *ptHead,int (*operate_list)(LIST *))
{
	int j,sum=0;

	LIST *pTemp;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;
		if((j=(*operate_list)(pTemp))<0) return -1;
		sum+=j;
	}

	return sum;
}
/*直接将数据插入头节点*/
void InsertList(LIST **pptHead,LIST *p)
{
	p->pNext=	*pptHead;
	*pptHead=	p;
}
void InsertListTail(LIST **pptHead,LIST *p)
{	
	LIST *ptHead=(*pptHead);
	if(ptHead==NULL){
		InsertList(pptHead,p);
		return ;
	}
	while(ptHead->pNext!=NULL) ptHead=ptHead->pNext;
	InsertList((LIST**)&(ptHead->pNext),(LIST*)p);
}
void AppendListTail(LIST **pptHead,LIST *p)
{	
	LIST *ptHead=(*pptHead);
	if(ptHead==NULL){
		*pptHead=p;
		return ;
	}
	while(ptHead->pNext!=NULL) ptHead=ptHead->pNext;
	ptHead->pNext=p;
}
/*其中comp_list是对插入的具体操作,的排序条件,只做升*/
/*comp_list参数1被比数，参数2链表内的数据 	   */
/*对插入的节点的pNext没有要求(链表内部自动复位)    */
/*该函数的LIST *p 参数是外部分配的内存 */
void InsertListSort(LIST **pptHead,LIST *p,int (*comp_list)(LIST *,LIST *))
{
	LIST *ptPre,*ptCur;

	/*空表则直接插入*/
	if(*pptHead==NULL){
		InsertList(pptHead,p);
		return ;
	}

	/*如果插入的节点最小，放在最前面*/
	if((*comp_list)(p,*pptHead)<=0){
		InsertList(pptHead,p);
		return ;
	}
		/*这里表明输入节点比头节点大*/
	ptPre=*pptHead;
	ptCur=ptPre->pNext;
			 /*如果链表 没有到末尾*/
	while(ptCur!=NULL){
			/*发现一个比输入数据大的节点*/
		if((*comp_list)(p,ptCur)<=0) break;
			/*链表指针向后移动*/
		ptPre=ptCur;
		ptCur=ptPre->pNext;
	}
	/*位置确定，只要插入即可*/
	InsertList(&(ptPre->pNext),p);
}

/*其中comp_list是对插入的具体比较操作,的排序条件,只做升*/
/*comp_list参数1被比数，参数2链表内的数据 	    */
/*sum_list 参数1被累加的外部数据，参数2链表内的数据 */
/*对插入的节点的pNext没有要求(链表内部自动复位)     */
/*该函数的LIST *p 参数是外部分配的内存 */
/*返回值
	FALSE:没有SUM上已有的链表成员，外部内存被链表用去
	TRUE :该部分数值被SUM到已有的内存LIST中，内存可重复利用
*/
int InsertListSumm(LIST **pptHead,LIST *p,
      int (*comp_list)(LIST *,LIST *),
      void (*sum_list)(LIST *,LIST *))
{
	int compr;

	LIST *ptPre,*ptCur;

	/*空表则直接插入*/
	if(*pptHead==NULL){
		InsertList(pptHead,p);
		return FALSE;
	}

	/*如果插入的节点最小，放在最前面*/
	compr=(*comp_list)(p,*pptHead);

        if(compr<0){
		InsertList(pptHead,p);
		return FALSE;
	}

	/*和头节点相等，将数值累加到头节点*/
        if(compr==0){
		(*sum_list)(*pptHead,p);
		return TRUE;
	}


		/*这里表明输入节点比头节点大*/
	ptPre=*pptHead;
	ptCur=ptPre->pNext;
			 /*如果链表 没有到末尾*/
	while(ptCur!=NULL){
		compr=(*comp_list)(p,ptCur);

			/*发现一个比输入数据大的节点,不能累加*/
                if(compr<0) break;
			/*数值逻辑相等，则累加数据*/
		if(compr==0){
			(*sum_list)(ptCur,p);
			return TRUE;
		}
			/*链表指针向后移动*/
		ptPre=ptCur;
		ptCur=ptPre->pNext;
	}
	/*位置确定，只要插入即可,插入链表用去内存*/

	InsertList(&(ptPre->pNext),p);

	return FALSE;
}
int InsertListSortSum(LIST **pptHead,LIST *p,
      int (*comp_list)(LIST *,LIST *),
      void (*sum_list)(LIST *,LIST *))
{
	return InsertListSumm(pptHead,p,comp_list,sum_list);
}
/*其中 *pi是要查找的数据*search_list是查询匹配条件*/
/*search_list参数1是链表内的单个ITEM，参数2是外部输入的数据 */
int SearchList(LIST *ptHead,void *pi,LIST **p,
	int (*comp_list)(void *,LIST *))
{
	while(ptHead!=NULL){
		if((*comp_list)(pi,ptHead)==0){
			*p=ptHead;
			return FOUND;
		}
	        ptHead=ptHead->pNext;
	}
	return NOTFOUND;
}

/*其中 *p要删除节点的指针*/
/*返回数值：
	TRUE删除了 FALSE未删除*/
int DeleteList(LIST **pptHead,LIST *p)
{
	LIST *ptPre,*ptCur;

	if(p==*pptHead){
		*pptHead=p->pNext;
		return TRUE;
	}

	ptPre=*pptHead;
	ptCur=ptPre->pNext;

	while(ptCur!=NULL){
		if(p==ptCur) {
			/*找到了，直接跳过该节点*/
			ptPre->pNext=p->pNext;
			return TRUE;
		}
		ptPre=ptCur;
		ptCur=ptPre->pNext;
	}
	/*链表内根本就没有指定节点*/

	return FALSE;
}
void DestroyList(LIST *ptHead)
{
	LIST *pTemp;

	while(ptHead!=NULL){
		pTemp=ptHead;
		ptHead=ptHead->pNext;
		free(pTemp);
	}
}
void FreeList(LIST *ptHead)
{
	DestroyList(ptHead);
}
void ReverseList(LIST **pptHead)
{
	LIST *pPre,*pCur;
	
	pCur=*pptHead;
	*pptHead=NULL;
	
	while(pCur!=NULL){
		pPre=pCur;
		pCur=pCur->pNext;
		InsertList(pptHead,pPre);
	}
}
void assign_insert_list(void **ppData,void *pData)
{
	InsertList((LIST**)(ppData), (LIST*)pData);
}

/*样板函数例子*/
/***
int sample_rand(void *pSource,void *pTarget)
{
        return (rand()%3-1);
}
int keycmp_sample(BINTREE *pBinValue,BINTREE *pBinNode)
{
        void *pTarget=pBinNode->pData,*pSource=pBinValue;
        
        return sample_rand(pSource,pTarget);
}
int keycmpext_sample(void *pValue, BINTREE *pBinNode)
{
        return keycmp_sample((BINTREE *)pValue,pBinNode);
}
void assign_sample(BINTREE **pptHead,void *p)
{
        *pptHead=(BINTREE *)p;
}
int operation_sample(void *pData)
{
        return 0;
}
**/
void **ppLinkNodeLast=NULL;

void link_node_i(void **pp1,void **pp2);

void (*link_node)(void **,void **)=link_node_i;
void link_node_i(void **pp1,void **pp2){;;}

int link_node_travel(void **ppD)
{
	if(ppLinkNodeLast!=NULL)link_node(ppLinkNodeLast,ppD);
	ppLinkNodeLast=ppD;
	return 0;
}

void LinkBin(BINTREE *ptHead,void (*link_node_v)(void **,void **))
{
	ppLinkNodeLast=NULL;
	link_node=link_node_v;
	TravelBin(ptHead,link_node_travel);
	link_node=link_node_i;
	ppLinkNodeLast=NULL;
}
void link_node_hand(void **pp1,void **pp2)
{
	 AppendListTail((LIST**)pp1,(LIST*)(*pp2));
}
void HandShakeBin(BINTREE *ptHead)
{
	LinkBin(ptHead,link_node_hand);
}
void AdjustBin(BINTREE **pptHead)
{
	BINTREE *pTemp,*ptPre,*ptCur;

	switch((*pptHead)->diff){
	case 2:
	ptPre=*pptHead;
	ptCur=ptPre->pRight;
	
	if(ptCur->diff==-1){
		pTemp=ptCur->pLeft;
		
		ptPre->pRight=pTemp->pLeft;
		ptCur->pLeft=pTemp->pRight;
		
		pTemp->pLeft=ptPre;
		pTemp->pRight=ptCur;
		
		switch(pTemp->diff){
		case 0:
		ptPre->diff=0;
		ptCur->diff=0;
		break;
		case 1:
		ptPre->diff=-1;
		ptCur->diff=0;
		break;
		default: /* -1 here */
		ptPre->diff=0;
		ptCur->diff=1;
		break;
		}
		 /* *pptHead left right same high here */
		pTemp->diff=0;
		(*pptHead)=pTemp;
	}
	else{  /* ptCur->diff must ==0 or ==1 ok here*/
		ptPre->pRight=ptCur->pLeft;
		ptCur->pLeft=ptPre;
	/* when ptCur->diff==1 and ptPre->diff=0 ptCur->diff=0*/
	/* when ptCur->diff==0 and ptPre->diff=1 ptCur->diff=-1*/
		ptPre->diff=1-ptCur->diff;
		ptCur->diff--;
		(*pptHead)=ptCur; /* root change to ptCur */
	}
	break;
	case -2:
	ptPre=*pptHead;
	ptCur=ptPre->pLeft;
	if(ptCur->diff==1){
		pTemp=ptCur->pRight;
		
		ptCur->pRight=pTemp->pLeft;
		ptPre->pLeft=pTemp->pRight;
		
		pTemp->pLeft=ptCur; 
		pTemp->pRight=ptPre;
		
		switch(pTemp->diff){
		case 0:
		ptPre->diff=0;
		ptCur->diff=0;
		break;
		case 1:
		ptPre->diff=0;
		ptCur->diff=-1;
		break;
		default: /* -1 */
		ptPre->diff=1;
		ptCur->diff=0;
		break;
		}
		/* *pptHead left right same high here */
		pTemp->diff=0; 
		(*pptHead)=pTemp;
	}
	else{  /* ptCur->diff must ==0 or ==-1 */
		ptPre->pLeft=ptCur->pRight;
		ptCur->pRight=ptPre;
	/* when ptCur->diff==-1 and ptPre->diff=0 ptCur->diff=0*/
	/* when ptCur->diff==0  and ptPre->diff=-1,ptCur->diff=1*/
		ptPre->diff=-1-ptCur->diff;
		ptCur->diff++;
		(*pptHead)=ptCur; /* root change to ptCur */
	}
	break;
	default: break;
	}
}
int4b InsertBin(BINTREE **pptHead, void *pValue, 
        int4b ( *pFunction)(void *,void*), 
        void ( *pAssign)(void **,void *))
{
        int4b diff,res,r;

        BINTREE *pTemp;
    

        if(*pptHead==NULL){
                if((pTemp=MallocBin())==NULL)
                        return -1;

                (*pAssign)(&(pTemp->pData),pValue); 
                *pptHead=pTemp;

                return EQUAL;
        } 
        
        diff=(*pptHead)->diff;
        
        if((res=(*pFunction)(pValue,(*pptHead)->pData))==0){
                (*pAssign)(&((*pptHead)->pData),pValue);
                return EQUAL;
        }

        if(res>0){ /* right */
        
                if((*pptHead)->pRight!=NULL){
                        r=InsertBin( &((*pptHead)->pRight),
                                pValue,pFunction,pAssign);
                        
                        if(r<0) return -1;
                        
                        if(r==HIGHER) (*pptHead)->diff++;


                }
                else { /* pRight==NULL here */
                        if((pTemp=MallocBin())==NULL){
                                printf("error mallocRight BinNode.\n");
                                return -1;
                        }
                        (*pAssign)(&(pTemp->pData),pValue); 
                        (*pptHead)->pRight=pTemp;
                        
                        (*pptHead)->diff++;

                }
        }
        else {
                
                if((*pptHead)->pLeft!=NULL){
                        
                        r=InsertBin( &((*pptHead)->pLeft),
                                pValue,pFunction,pAssign);
                        
                        if(r<0) return -1;
                        
                        if(r==HIGHER) (*pptHead)->diff--;
                }
                else{  /* pLeft==NULL */
                        if((pTemp=MallocBin())==NULL){
                                printf("error mallocLeft BinNode.\n");
                                return -1;
                        }
                        (*pAssign)(&(pTemp->pData),pValue); 
                        (*pptHead)->pLeft=pTemp;

                        (*pptHead)->diff--;
                }   
        }
        
            /* adjust here */
        AdjustBin(pptHead);
        
        if(diff==0&&(*pptHead)->diff) return HIGHER;
        
        return EQUAL;
}

BINTREE * MallocBin()
{
        BINTREE *pTemp;
        
        if((pTemp=(BINTREE *)malloc(sizeof(BINTREE)))==NULL)
                return NULL;
        
        pTemp->pLeft=pTemp->pRight=NULL;
        pTemp->diff=0;
        pTemp->pData=NULL;
        return pTemp;
}

int4b SearchBin(BINTREE *ptHead, void *pValue,
        int ( *pFunction)(void *,void *), void **pptCur)
{
        int res;
    
        if(ptHead==NULL) return NOTFOUND;
    
        if((res=(*pFunction)(pValue,ptHead->pData))==0) {               
                *pptCur=ptHead->pData;
                return FOUND;    
        }       
        
        if(res>0) return SearchBin(ptHead->pRight,pValue,pFunction,pptCur);
        else      return SearchBin(ptHead->pLeft, pValue,pFunction,pptCur);
}

int4b DestroyBin(BINTREE *ptHead)
{
        int4b j=0;
        
        if(ptHead==NULL) return 0;
        
        j+=DestroyBin(ptHead->pLeft);
        j+=DestroyBin(ptHead->pRight);
        
        free(ptHead);
        
        return j+1;
}
int TravelBin(BINTREE *ptHead, int ( *pOperation)(void **))
{
	int j=0;
	
	if(ptHead==NULL) return 0;
	
	if(ptHead->pLeft!=NULL)
		j+=TravelBin(ptHead->pLeft,pOperation);
	j+=(*pOperation)(&(ptHead->pData));
	if(ptHead->pRight!=NULL)
		j+=TravelBin(ptHead->pRight,pOperation);
	return j;
}

int4b DeleteBinMin(BINTREE **pptHead, BINTREE **pptCur)
{
        int4b diff;
        
        BINTREE *pTemp;
    
        *pptCur=NULL;

        diff=(*pptHead)->diff;
        
        if((*pptHead)->pLeft!=NULL){
                if(DeleteBinMin(&((*pptHead)->pLeft),pptCur)==LOWER)
                        (*pptHead)->diff++;
        }
        else{/*(*pptHead)->pLeft==NULL here(*pptHead)->diff in (0,1)*/
                if((*pptHead)->diff==1){
			pTemp=*pptHead;
			*pptHead=pTemp->pRight;
                }
                else{ /* (*pptHead)->diff ==0 leaves here.*/
			pTemp=*pptHead;
			*pptHead=NULL;
                }
                *pptCur=pTemp;
                return LOWER;
        }
                
        AdjustBin(pptHead);
        
        if(diff!=0&&(*pptHead)->diff==0) return LOWER;
        
        return EQUAL;
}

int4b DeleteBin(BINTREE **pptHead,void *p,
	int4b ( *pFunction)(void *,void *),BINTREE **pptCur)
{
        int4b r;
                
        *pptCur=NULL;

        if((r=(*pFunction)(p,(*pptHead)->pData))==0){

                BINTREE *ptCur=NULL,**pptPre;

                *pptCur=*pptHead;

                if((*pptHead)->pLeft==NULL){
                        /*左子树为空，右子树空或非空，叶子节点*/
                        *pptHead=(*pptHead)->pRight;
                        return LOWER;   
                }
                         /*左子树非空，右子树空*/
                if((*pptHead)->pRight==NULL){
                        *pptHead=(*pptHead)->pLeft;
                        return LOWER;
                }

                pptPre=&((*pptHead)->pRight);

                /*左子树非空，右子树非空*/
                r=DeleteBinMin(pptPre,&ptCur);

                if(ptCur==NULL) return EQUAL;

                ptCur->pLeft=(*pptHead)->pLeft;
                ptCur->pRight=(*pptHead)->pRight;

                if(r==LOWER){
                        ptCur->diff=(*pptHead)->diff-1;
                        AdjustBin(&ptCur);
                }
                else
                        ptCur->diff=  (*pptHead)->diff; 

                *pptHead=ptCur;

                return r;
        }

        if(r>0){
                r=DeleteBin(&((*pptHead)->pRight),p,pFunction,pptCur);
                if(r==LOWER){
                        (*pptHead)->diff--;
                        AdjustBin(pptHead);
                }
                return r;
        }

        r=DeleteBin(&((*pptHead)->pLeft),p,pFunction,pptCur);
        if(r==LOWER){
                (*pptHead)->diff++;
                AdjustBin(pptHead);
        }
        return r;
}
void assign_insert_bintree(void **ppData,void *pData)
{
	InsertList((LIST**)(ppData), (LIST*)pData);
}
