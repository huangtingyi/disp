#ifndef _CALLBACK_BASE_H
#define _CALLBACK_BASE_H
#include "TDFAPIStruct.h"
#include "IoService.h"
#include "SubData.h"

#include "callsupp.h"


typedef struct _filename_set_
{	
	string tdfmktName;
	string tdfqueName;
	string tdftraName;
	string tdfordName;
	
	FILE *fpTdfMkt;
	FILE *fpTdfQue;
	FILE *fpTdfTra;
	FILE *fpTdfOrd;

} FileNameSet;

typedef struct _my_sub_data_
{
        int nItemCnt;
        long long       cur_time;       ///< 接收到数据时间，精确至毫秒，格式为：DDHHMMSSmmm
        std::string     data;           ///< 数据内容
}MySubData;

class CallBackBase
{
public:
	CallBackBase(int iWriteFlag,string& strWork);
	virtual ~CallBackBase();

	void SetIoService(IoService *ios);
	
	int OpenFileSet(int iWriteFlag,string& strWork);
	void CloseFileSet();

	
	void OnSubscribe_TdfMkt(TDF_MARKET_DATA *p,int nItemCnt);
	void OnSubscribe_TdfTra(TDF_TRANSACTION *p,int nItemCnt);
	void OnSubscribe_TdfOrd(TDF_ORDER *p,int nItemCnt);
	void OnSubscribe_TdfQue(TDF_ORDER_QUEUE *p,int nItemCnt);
public:
	/// 处理订阅数据
	void Deal_Message_TdfMkt(MySubData *d);
	void Deal_Message_TdfTra(MySubData *d);
	void Deal_Message_TdfOrd(MySubData *d);
	void Deal_Message_TdfQue(MySubData *d);
	void Deal_Message_D31Item(MySubData *d);



	IoService	*m_ios;
private:
	FileNameSet m_fileSet;
	int	m_iWriteFlag;
};

#endif  //_CALLBACK_BASE_H
