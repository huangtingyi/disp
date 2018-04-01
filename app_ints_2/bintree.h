
#ifndef __BIN_TREE__
#define __BIN_TREE__

typedef long		int8b;
typedef int		int4b;
typedef unsigned long	uint8b;
typedef unsigned int	uint4b;
typedef unsigned char	uint1b;

typedef long	LONGINT;
typedef long	POINTER;

#ifndef FOUND
#define FOUND		1
#endif
#ifndef NOTFOUND
#define NOTFOUND	0
#endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif


typedef struct __List__
{
	struct __List__ *pNext;
} LIST;


/*计算链表的个数*/
int CountList(LIST* ptHead);

/*其中operate_list是对单个节点的操作函数*/
/*该函数很有用可以实现打印释放内存等操作*/
int TravelList(LIST *ptHead,int (*operate_list)(LIST *));


/*直接将数据插入头节点*/
void InsertList(LIST **pptHead,LIST *p);
void InsertListTail(LIST **pptHead,LIST *p);
void AppendListTail(LIST **pptHead,LIST *p);


/*其中comp_list是对插入的具体操作,的排序条件,只做升*/
/*comp_list参数1被比数，参数2链表内的数据 	   */
/*对插入的节点的pNext没有要求(链表内部自动复位)    */
/*该函数的LIST *p 参数是外部分配的内存 			*/
void InsertListSort(LIST **pptHead,LIST *p,int (*comp_list)(LIST *,LIST *));


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
      void (*sum_list)(LIST *,LIST *));
int InsertListSortSum(LIST **pptHead,LIST *p,
      int (*comp_list)(LIST *,LIST *),
      void (*sum_list)(LIST *,LIST *));

/*其中 *pi是要查找的数据*search_list是查询匹配条件*/
/*search_list参数1是链表内的单个ITEM，参数2是外部输入的数据 */
int SearchList(LIST *ptHead,void *pi,LIST **p,int (*comp_list)(void *,LIST *));


/*其中 *p要删除节点的指针*/
/*返回数值：
	TRUE删除了 FALSE未删除*/
int DeleteList(LIST **pptHead,LIST *p);

void DestroyList(LIST *p);
void FreeList(LIST *p);
void ReverseList(LIST **pptHead);
void assign_insert_list(void **ppData,void *pData);

#ifndef HIGHER
#define HIGHER 1 
#endif
#ifndef LOWER
#define LOWER  -1
#endif
#ifndef EQUAL
#define EQUAL  0 
#endif

typedef struct __Bin_Tree__
{
	struct __Bin_Tree__ *pLeft;
	struct __Bin_Tree__ *pRight;
	void   *pData;
	int    diff;
}   BINTREE;
void LinkBin(BINTREE *ptHead,void (*link_node_v)(void **,void **));
void HandShakeBin(BINTREE *ptHead);
void AdjustBin(BINTREE **pptHead);
int4b InsertBin(BINTREE **pptHead, void *pValue, 
        int4b ( *pFunctionExt)(void *,void*), 
        void ( *pAssign)(void **,void *));
BINTREE * MallocBin();
int4b SearchBin(BINTREE *ptHead, void *pValue,
	int ( *pFunction)(void *,void *), void **pptCur);
int4b DestroyBin(BINTREE *ptHead);
int TravelBin(BINTREE *ptHead, int ( *pOperation)(void **));
int4b DeleteBinMin(BINTREE **pptHead, BINTREE **pptCur);
int4b DeleteBin(BINTREE **pptHead, void *p,
	int4b ( *pFunction)(void *,void *),BINTREE **pptCur);
void assign_insert_bintree(void **ppData,void *pData);


#endif

