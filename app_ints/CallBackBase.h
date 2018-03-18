#ifndef _CALLBACK_BASE_H
#define _CALLBACK_BASE_H
#include "GTAQTSInterfaceBase.h"
#include "../Common/IoService.h"
#include "../Common/SubData.h"

typedef struct _filename_set_
{
	string gtaqhName;
	string gtaahName;
	string gtathName;
	string gtaqzName;
	string gtatzName;
	string gtaozName;
	
	string tdfmktName;
	string tdfqueName;
	string tdftraName;
	string tdfordName;
	

	FILE *fpGtaQh;
	FILE *fpGtaAh;
	FILE *fpGtaTh;
	FILE *fpGtaQz;
	FILE *fpGtaTz;
	FILE *fpGtaOz;
	
	FILE *fpTdfMkt;
	FILE *fpTdfQue;
	FILE *fpTdfTra;
	FILE *fpTdfOrd;
	

} FileNameSet;

//用户通过订阅接口订阅相应消息数据后，数据将通过本接口返回
//详见《国泰安实时行情系统V2.X 用户手册》4.1.2 回调接口类IGTAQTSCallbackBase 章节
class CallBackBase :public IGTAQTSCallbackBase
{
public:
	CallBackBase(int iWriteFlag,string &strWork);
	virtual ~CallBackBase();

	void SetIoService(IoService *ios);

public:
	virtual void OnLoginState(RetCode errCode) override;
    /// 连接状态返回，连接成功/失败
    /// @param  msgType     -- 消息类型
    /// @param  errCode     -- 失败原因，成功时返回0
    ///                     详见《国泰安实时行情系统V2.X 用户手册》5.5返回码含义列表RetCode 章节
	virtual void OnConnectionState(MsgType msgType, RetCode errCode);

    /// 上交所L2实时行情快照订阅数据实时回调接口
    /// @param  RealSSEL2Quotation  -- 实时数据
	virtual void OnSubscribe_SSEL2_Quotation(const SSEL2_Quotation& RealSSEL2Quotation);

    /// 上交所L2逐笔成交订阅数据实时回调接口
    /// @param  RealSSEL2Transaction    -- 实时数据
	virtual void OnSubscribe_SSEL2_Transaction(const SSEL2_Transaction& RealSSEL2Transaction);
    
    /// 上交所L2集合竞价订阅数据实时回调接口
	virtual void OnSubscribe_SSEL2_Auction(const SSEL2_Auction& RealSSEL2Auction);

    /// 深交所L2实时行情订阅数据实时回调接口
    /// @param  RealSZSEL2Quotation -- 实时数据
	virtual void OnSubscribe_SZSEL2_Quotation(const SZSEL2_Quotation& RealSZSEL2Quotation);

	/**
	    需要处理其它消息类型时，请通过头文件GTAQTSInterfaceBase.h内的IGTAQTSCallbackBase类，
	    拷贝对应的OnSubscribe_xxx接口到此处，参照上面的方法，实现该函数即可
	    详见《国泰安实时行情系统V2.X 用户手册》4.1.2.3 实时数据回调OnSubscribe 章节
	*/
	virtual void OnSubscribe_SZSEL2_Transaction(const SZSEL2_Transaction& RealSZSEL2Transaction) override;
	virtual void OnSubscribe_SZSEL2_Order(const SZSEL2_Order& RealSZSEL2Order) override;
	
	/// 处理订阅数据
	void Deal_Message_SSEL2_Quotation(SubData *subdata);
	void Deal_Message_SSEL2_Transaction(SubData *subdata);
	void Deal_Message_SSEL2_Auction(SubData *subdata);
	void Deal_Message_SZSEL2_Quotation(SubData *subdata);
	void Deal_Message_SZSEL2_Transaction(SubData *subdata);
	void Deal_Message_SZSEL2_Order(SubData *subdata);


private:
	IoService	*m_ios;
	FileNameSet m_fileSet;
	int	m_iWriteFlag;	
};

void LockWorkThread();
void UnLockWorkThread();
int IsWorkThreadLock();

void InitUserArray(char sDispName[]);
void FlashUserArray(char sDispName[]);

#endif  //_CALLBACK_BASE_H
