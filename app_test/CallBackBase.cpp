#include "CallBackBase.h"
#include <stdio.h>
#include "../Common/public.h"

CallBackBase::CallBackBase(void)
{
    m_ios = 0;
}


CallBackBase::~CallBackBase(void)
{
}

void CallBackBase::SetIoService(IoService *ios)
{
    m_ios = ios;
}

void CallBackBase::OnLoginState(RetCode errCode) {
//	BOOST_LOG_SEV(g_lg, info) << __FUNCTION__ << ' ' << errCode;
}

/// 连接状态返回，连接成功/失败
/// @param  msgType     -- 消息类型
/// @param  errCode     -- 失败原因，成功时返回0 
///                     详见《国泰安实时行情系统V2.X 用户手册》5.5返回码含义列表RetCode 章节 
void CallBackBase::OnConnectionState(MsgType msgType, RetCode errCode)
{
    printf("MsgType:0x%x ConnectionState:%d\n", msgType, errCode);
//	BOOST_LOG_SEV(g_lg, info) << __FUNCTION__ << ' ' << msgType << ' ' << errCode;
}

void CallBackBase::OnSubscribe_SSEL2_Quotation(const SSEL2_Quotation& RealSSEL2Quotation)
{
    /**
    警告：请勿在回调函数接口内执行耗时操作，如：复杂运算，写文件等；否则会堵塞数据的接收。
    建议处理方式：把接收到的数据放至队列，再由工作线程处理接收到的数据内容；
    可参照本回调函数的处理方法
    */

    if ( 0 == m_ios )
    {
        printf("IoService object is null");
        return ;
    }

    UTIL_Time stTime;
    PUTIL_GetLocalTime(&stTime);

    //接收到数据后，先放入本地队列，等待数据处理接口处理
    SubData *subdata = new SubData;
    subdata->msgType = Msg_SSEL2_Quotation;
    subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
    subdata->data.assign((const char*)&RealSSEL2Quotation, sizeof(RealSSEL2Quotation));

    TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
    m_ios->Post(task);
}

/// 深交所L1实时行情订阅数据实时回调接口
/// @param  RealSZSEL1Quotation -- 实时数据
void CallBackBase::OnSubscribe_SZSEL1_Quotation(const SZSEL1_Quotation& RealSZSEL1Quotation)
{
    /**
    警告：请勿在回调函数接口内执行耗时操作，如：复杂运算，写文件等；否则会堵塞数据的接收。
    建议处理方式：把接收到的数据放至队列，再由工作线程处理接收到的数据内容；
    可参照本回调函数的处理方法
    */

    if ( 0 == m_ios )
    {
        printf("IoService object is null");
        return ;
    }

    UTIL_Time stTime;
    PUTIL_GetLocalTime(&stTime);

    //接收到数据后，先放入本地队列，等待数据处理接口处理
    SubData *subdata = new SubData;
    subdata->msgType = Msg_SZSEL1_Quotation;
    subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
    subdata->data.assign((const char*)&RealSZSEL1Quotation, sizeof(RealSZSEL1Quotation));

    TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
    m_ios->Post(task);
}

/// 上交所L2逐笔成交订阅数据实时回调接口
/// @param  RealSSEL2Transaction    -- 实时数据
void CallBackBase::OnSubscribe_SSEL2_Transaction(const SSEL2_Transaction& RealSSEL2Transaction)
{
    /**
    警告：请勿在回调函数接口内执行耗时操作，如：复杂运算，写文件等；否则会堵塞数据的接收。
    建议处理方式：把接收到的数据放至队列，再由工作线程处理接收到的数据内容；
    可参照本回调函数的处理方法
    */

    if ( 0 == m_ios )
    {
        printf("IoService object is null");
        return ;
    }

    UTIL_Time stTime;
    PUTIL_GetLocalTime(&stTime);

    //接收到数据后，先放入本地队列，等待数据处理接口处理
    SubData *subdata = new SubData;
    subdata->msgType = Msg_SSEL2_Transaction;
    subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
    subdata->data.assign((const char*)&RealSSEL2Transaction, sizeof(RealSSEL2Transaction));

    TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
    m_ios->Post(task);
}

/// 深交所L2实时行情订阅数据实时回调接口
/// @param  RealSZSEL2Quotation -- 实时数据
void CallBackBase::OnSubscribe_SZSEL2_Quotation(const SZSEL2_Quotation& RealSZSEL2Quotation)
{
    /**
    警告：请勿在回调函数接口内执行耗时操作，如：复杂运算，写文件等；否则会堵塞数据的接收。
    建议处理方式：把接收到的数据放至队列，再由工作线程处理接收到的数据内容；
    可参照本回调函数的处理方法
    */

    if ( 0 == m_ios )
    {
        printf("IoService object is null");
        return ;
    }

    UTIL_Time stTime;
    PUTIL_GetLocalTime(&stTime);

    //接收到数据后，先放入本地队列，等待数据处理接口处理
    SubData *subdata = new SubData;
    subdata->msgType = Msg_SZSEL2_Quotation;
    subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
    subdata->data.assign((const char*)&RealSZSEL2Quotation, sizeof(RealSZSEL2Quotation));

    TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
    m_ios->Post(task);
}

/// 处理上交所L2实时行情快照订阅数据
void CallBackBase::Deal_Message(SubData *subdata)
{
    static int count = 0;// 防止输出太多内容

    //智能指针，会自动释放指针指向的对象，释放内存空间
    std::unique_ptr<SubData> subdataptr(subdata);

    switch ( subdataptr->msgType )
    {
    case Msg_SSEL2_Quotation:
        {
            SSEL2_Quotation *RealSSEL2Quotation = (SSEL2_Quotation*)&subdata->data[0];
		m_gta2csvMgr.writeCsv(*RealSSEL2Quotation);
#if 0
            if( count < 10 )// 防止输出太多内容
            {
                count++;
                printf("Subscribe Data: PacketTimeStamp = %lld, Symbol = %s, OpenPrice = %f, HighPrice = %f, LowPrice = %f, LastPrice = %f, ClosePrice = %f\n",
                    RealSSEL2Quotation->PacketTimeStamp, RealSSEL2Quotation->Symbol, RealSSEL2Quotation->OpenPrice, RealSSEL2Quotation->HighPrice,
                    RealSSEL2Quotation->LowPrice, RealSSEL2Quotation->LastPrice, RealSSEL2Quotation->ClosePrice);
            }
#endif
        }
        break;
    case Msg_SSEL2_Transaction:
        {
            SSEL2_Transaction *RealSSEL2Transaction = (SSEL2_Transaction*)&subdata->data[0];
			m_gta2csvMgr.writeCsv(*RealSSEL2Transaction);
#if 0
            if( count < 10 )// 防止输出太多内容
            {
                count++;
                printf("Subscribe Data: PacketTimeStamp = %lld, Symbol = %s, TradeTime = %d, TradePrice = %f, TradeVolume = %u, TradeAmount = %f\n",
                    RealSSEL2Transaction->PacketTimeStamp, RealSSEL2Transaction->Symbol, RealSSEL2Transaction->TradeTime, RealSSEL2Transaction->TradePrice,
                    RealSSEL2Transaction->TradeVolume, RealSSEL2Transaction->TradeAmount);
            }
#endif
        }
        break;
    case Msg_SZSEL1_Quotation:
        {
            SZSEL1_Quotation *RealSZSEL1Quotation = (SZSEL1_Quotation*)&subdata->data[0];

            if( count < 10 )// 防止输出太多内容
            {
                count++;
                printf("Subscribe Data: Time = %lld, Symbol = %s, OpenPrice = %f, HighPrice = %f, LowPrice = %f, LastPrice = %f\n",
                    RealSZSEL1Quotation->Time, RealSZSEL1Quotation->Symbol, RealSZSEL1Quotation->OpenPrice, RealSZSEL1Quotation->HighPrice,
                    RealSZSEL1Quotation->LowPrice, RealSZSEL1Quotation->LastPrice);
            }
        }
        break;
    case Msg_SZSEL2_Quotation:
        {
            SZSEL2_Quotation *RealSZSEL2Quotation = (SZSEL2_Quotation*)&subdata->data[0];
			m_gta2csvMgr.writeCsv(*RealSZSEL2Quotation);
#if 0
            if( count < 10 )// 防止输出太多内容
            {
                count++;
                printf("Subscribe Data: LocalTimeStamp = %d, Symbol = %s, OpenPrice = %f, HighPrice = %f, LowPrice = %f, LastPrice = %f\n",
                    RealSSEL2Quotation->LocalTimeStamp, RealSSEL2Quotation->Symbol, RealSSEL2Quotation->OpenPrice, RealSSEL2Quotation->HighPrice,
                    RealSSEL2Quotation->LowPrice, RealSSEL2Quotation->LastPrice);
            }
#endif
        }
        break;
	case Msg_SZSEL2_Transaction:
	{
		SZSEL2_Transaction *RealSZSEL2Transaction = (SZSEL2_Transaction*)&subdata->data[0];
		m_gta2csvMgr.writeCsv(*RealSZSEL2Transaction);
	}
	break;
	case Msg_SZSEL2_Order:
	{
		SZSEL2_Order *RealSZSEL2Order = (SZSEL2_Order*)&subdata->data[0];
		m_gta2csvMgr.writeCsv(*RealSZSEL2Order);
	}
	break;
    default:
        printf("unknown message type:0x%x\n", subdataptr->msgType);
        break;
    }
}

/*virtual*/void CallBackBase::OnSubscribe_SZSEL2_Order(const SZSEL2_Order& RealSZSEL2Order)/*override*/ {
	if (0 == m_ios) {
		printf("IoService object is null");
		return;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SZSEL2_Transaction;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSZSEL2Order, sizeof(RealSZSEL2Order));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
	m_ios->Post(task);
}

void CallBackBase::OnSubscribe_SZSEL2_Transaction(const SZSEL2_Transaction& RealSZSEL2Transaction) {
	if (0 == m_ios)	{
		printf("IoService object is null");
		return;
	}

	UTIL_Time stTime;
	PUTIL_GetLocalTime(&stTime);

	//接收到数据后，先放入本地队列，等待数据处理接口处理
	SubData *subdata = new SubData;
	subdata->msgType = Msg_SZSEL2_Transaction;
	subdata->cur_time = PUTIL_SystemTimeToDateTime(&stTime);
	subdata->data.assign((const char*)&RealSZSEL2Transaction, sizeof(RealSZSEL2Transaction));

	TaskPtr task(new Task(std::bind(&CallBackBase::Deal_Message, this, subdata)));
	m_ios->Post(task);
}
