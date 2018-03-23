#include <vector>
#include <map>
#include <iostream>
#include "MessageQueue.h"

class MqMonitorMgr
{
public:
	MqMonitorMgr(int iMqKey);
	~MqMonitorMgr();
	bool existMq();
	void startMqmonitor();
public:
	MessageQueue *m_pMq;
	int m_iMqKey;
	int m_iWarnLevel;
	int m_iWaitTime;//等待时间/ms
	int m_iLimtVal;//清理阀值/百分比
};

MqMonitorMgr::MqMonitorMgr(int iMqKey)
{
	m_iMqKey = iMqKey;
	m_iWarnLevel = 0;
	m_iWaitTime = 10000;
	m_iLimtVal = 80;

	if (m_iMqKey <=0 )
	{
		cout<<"消息监控队列Id["<<m_iMqKey<<"]有误."<<endl;
		exit(-1);
	}

	m_pMq = new MessageQueue(m_iMqKey);
	if (!m_pMq)
	{
		cout<<"mqkeyId["<<m_iMqKey<<"]申请系统资源失败."<<endl;
		exit(-1);
	}

	m_pMq->open(false, true, 1024, 5000);
}

MqMonitorMgr::~MqMonitorMgr()
{
	if (!m_pMq)
	{
		delete m_pMq;
		m_pMq = NULL;
	}
}

bool MqMonitorMgr::existMq()
{
	return m_pMq->exist();
}

void MqMonitorMgr::startMqmonitor()
{
	int iMode = 1;
	int iMsgNum = 0;
	char sBuf[5120] = { 0 };
	int iMsqTotal = 0;
	int iMsqKB = 0;
	int iMsqTotalKB = 0;
	int iPercent = 0;
	long gTotalClear = 0;
	while (true)
	{
		int iNorMqNum = 0; //用于控制sleep

		//time (&m_tCurTime);
		m_pMq->getMessageInfo(iMsgNum, iMsqTotal, iMsqKB, iMsqTotalKB, iPercent);
		if (iPercent < 10)
		{
			m_iWarnLevel = 0;
			iNorMqNum++;
			usleep(2000000);
			continue;
		}
		else if (iPercent < m_iLimtVal)
		{
			m_iWarnLevel = 1;
			usleep(1000000);
		}
		else
		{
			m_iWarnLevel = 2;
			if (m_pMq->receive(sBuf, 0) <= 0)
			{
				continue;
			}
			gTotalClear++;
			if (gTotalClear % 2000 == 0)
				cout << "MqId:[" << m_iMqKey << "]消息历史累计清理量[" << gTotalClear << "]." << endl;
		}
	}
	return ;
}
