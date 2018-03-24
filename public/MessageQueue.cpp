#include "MessageQueue.h"

#ifdef DEF_HP
#include <sys/mqueue.h>
#endif
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

/*
 ##	设计思路：
 ##	2，使用方式，
 ##				构造：MessageQueue *p = new MessageQueue("/home/bill");
 ##				判断：if (!p || !(*p)) throw 1;
 ##				使用...
 ##
 */

//##ModelId=4314101D004F
void MessageQueue::open(bool ifblock, bool ifcreate, int maxlen, int maxnum)
{
	m_iBlock = ifblock;
	m_iMaxNum = maxnum;
	m_iMaxLen = maxlen;
	/*
	 if (buflen==-1)
	 m_iBufLen = maxlen;
	 else
	 m_iBufLen = buflen;
	 */
	m_iBufLen = maxlen;
	if (m_iBufLen > maxlen * maxnum || m_iBufLen < maxlen)
		throw "error!";

	//##如果已经open了，不再调用
	if (m_bAttached)
		return;

	__OPEN:
	//##没有open，先看看能不能直接open
	if (_open())
		return;

	//##检查错误原因
	if (m_iErrorNo != E_NOENT)
		throw "error!";

	//##检查消息创建参数
	if (!ifcreate)
		throw "error!";

	//##试着创建
	if (_create())
		return;

	//##检查错误原因
	if (m_iErrorNo != E_EXIST)
		throw "error!";

	goto __OPEN;
}

//##ModelId=4314101D0059
void MessageQueue::remove()
{
	//##如果不存在
	if (!exist())
		return;

	//##这个消息队列存在，进行删除
#ifdef SYSTEM_V
	{
		//##SYSTEM_V下面的操作
		if (!_open())
		{
			//##异常处理
			if (m_iErrorNo == E_NOENT)
			goto _TRUE;
			else
			throw "error!";
		}

		if (msgctl (m_iSysMQID, IPC_RMID, NULL))
		{
			//##异常处理
			if (errno == EIDRM)
			goto _TRUE;
			else
			throw "error!";
		}

		goto _TRUE;
	}
#else
	{
		//##POSIX下面的操作
		close();

		if (mq_unlink(m_sMQName))
		{
			//##异常处理
			if (errno == ENOENT)
				goto _TRUE;
			else
				throw "error!";
		}
	}
#endif

	_TRUE: m_bAttached = false;
	return;
}

//##ModelId=4314101D005A
void MessageQueue::close()
{
	//##看看有没有打开
	if (!m_bAttached)
		return;

	//##断开和消息队列的连接（不删除消息队列）
#ifdef SYSTEM_V
	//##不需要关闭，不用就是了
#else
	{
		if (mq_close(m_iSysMQID))
		{
			//##异常处理
			m_iErrorNo = _getStandardError(errno);
			if (m_iErrorNo == E_NOENT)
			{
				m_bAttached = false;
				return;
			}

			throw "error!";
		}

		m_bAttached = false;
	}
#endif

	return;
}

//##ModelId=4314101D005B
bool MessageQueue::exist()
{
	//##如果已经open了，那一定存在了
	if (m_bAttached)
		return true;

	//##如果能直接打开，说明存在
	if (_open())
	{
		//##不管关闭是否成功
		close();
		return true;
	}
	else
	{
		if (m_iErrorNo == E_PERM)
			return true;

		return false;
	}
}

//##ModelId=4314101D0061
int MessageQueue::send(void *data, int len, long lMtype)
{
	//##这里的实现不考虑优先级
	int ret;

	if (!len)
		return 0;

#ifdef SYSTEM_V

	if (len>m_iBufLen)
	throw "error!";

	void *p = m_pBuff;

	if (lMtype==0)
	*((long *)p) = len; // modified by linjinxin
	else
	*((long *)p) = lMtype;

	memcpy((char *)p+sizeof(long), data, len);

	ret = msgsnd (m_iSysMQID, p, len, (m_iBlock ? 0 : IPC_NOWAIT));
#else
	ret = mq_send(m_iSysMQID, (char *) data, len, 1);
#endif

	if (ret == -1)
	{
		//##异常处理
		m_iErrorNo = _getStandardError(errno);

		if (m_iErrorNo == E_NOMSG)
			m_iErrorNo = E_FULL; //POSIX

		if (m_iErrorNo == E_FULL)
			return 0;

		if (m_iErrorNo == E_NOENT)
			m_bAttached = false;

		throw "error!";
//		return -1;
	}

	return len;
}
//##ModelId=4314101D0061
int MessageQueue::send(void *data, int len)
{
	return send(data, len, 0);
}
//##ModelId=4314101D0064
int MessageQueue::receive(char *buf, long lMtype)
{
	//##如果没有消息，返回空
	/*
	 2005-05-24
	 接收消息的时候，忽略信号引起的中断
	 */
	int ret;
	int len;

#ifdef SYSTEM_V
	memset ((char *)m_pBuff, 0, m_iBufLen+sizeof(long));

	__RECEIVE:

	//ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
	ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, lMtype, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));

	/*
	 If msgtyp is 0, then the first message in the queue is read.

	 If msgtyp is greater than 0, then the first message on the queue
	 of  type  msgtyp  is  read,  unless  MSG_EXCEPT  was asserted in
	 msgflg, in which case the first message on the queue of type not
	 equal to msgtyp will be read.

	 If  msgtyp  is  less than 0, then the first message on the queue
	 with the lowest type less than or equal to the absolute value of
	 msgtyp will be read.

	 */

	if (ret >=0)
	{
		memcpy (buf, (char *)m_pBuff+sizeof(long), ret);
	}
	else
	{
		m_iErrorNo = _getStandardError (errno);

		if (m_iErrorNo == E_NOMSG)
		return 0;

		if (m_iErrorNo == E_NOENT)
		m_bAttached = false;

		if (m_iErrorNo == E_INTR)
		{
			printf ( "接收消息过程中收到信号，忽略信号...");
			goto __RECEIVE;
		}

		throw "error!";
	}

	return ret;
#else
	/*struct mq_attr attr;
	 ret = mq_getattr (m_iSysMQID, &attr);
	 if (ret == -1)
	 {
	 m_iErrorNo = _getStandardError (errno);
	 return -1;
	 }
	 */

	__RECEIVE: ret = mq_receive(m_iSysMQID, buf, /*attr.mq_msgsize*/m_iMaxLen, NULL);

	if (ret == -1)
	{
		m_iErrorNo = _getStandardError(errno);
		if (m_iErrorNo == E_NOMSG)
			return 0;

		if (m_iErrorNo == E_INTR)
		{
			printf("接收消息过程中收到信号，忽略信号...");
			goto __RECEIVE;
		}

		throw "error!";
	}
	return ret;
#endif
}

//##ModelId=4314101D0064
int MessageQueue::receive(char *buf)
{
	return receive(buf, 0);
}

//##ModelId=4314101D0066
int MessageQueue::getMessageNumber()
{
	if (!m_bAttached)
	{
		m_iErrorNo = E_CLOSE;
		return 0;
	}

#ifdef SYSTEM_V
	struct msqid_ds attr;
	//int ret;

	//##获取消息队列相关信息
	if (msgctl (m_iSysMQID, IPC_STAT, &attr))
	{
		//##异常处理
		m_iErrorNo = _getStandardError (errno);
		if (m_iErrorNo == E_NOENT)
		m_bAttached = false;

		throw "error!";
	}

	return attr.msg_qnum;
#else
	struct mq_attr attr;

	//##获取消息队列相关信息
	if (mq_getattr(m_iSysMQID, &attr))
	{
		//##异常处理
		m_iErrorNo = _getStandardError(errno);
		//return -1;
		throw "error!";
	}

	return attr.mq_curmsgs;
#endif
}

//MessageQueue::MessageQueue(char *mqname)
//##ModelId=4314101D0067
MessageQueue::MessageQueue(int key)
{
	//增加原始的key信息，2018-03-24
	m_oriKey= key;

	m_iSysMQID = -1;
	m_iMaxNum = 0;
	m_iMaxLen = 0;
	m_iBlock = 0;
	m_bAttached = false;
	m_pBuff = 0;
	m_iBufLen = 0;

	//##十进制的数转成十六进制的数，1000->0X1000
	//##方便去找队列
	{
		int iTemp = 0;

		m_iKey = 0;
		while (key)
		{
			m_iKey |= ((key % 10) << iTemp);
			key = key / 10;
			iTemp += 4;
		}
	}
	/*

	 m_sMQName = new char[strlen(mqname)+1];
	 if (!m_sMQName)
	 return;

	 strcpy(m_sMQName, mqname);

	 */
	m_sMQName = new char[32];
	if (!m_sMQName)
		throw "error!";

	sprintf(m_sMQName, "/tmp/mq%d", key);
}

//##ModelId=4314101D0069
int MessageQueue::getSysID()
{
	return m_iSysMQID;
}

//##ModelId=4314101D006A
int MessageQueue::getOccurPercent()
{

	if (!m_bAttached)
	{
		m_iErrorNo = E_CLOSE;
		return 0;
	}

#ifdef SYSTEM_V
	struct msqid_ds attr;
	//int ret;

	//##获取消息队列相关信息
	if (msgctl (m_iSysMQID, IPC_STAT, &attr))
	{
		//##异常处理
		m_iErrorNo = _getStandardError (errno);
		if (m_iErrorNo == E_NOENT)
		m_bAttached = false;

		throw "error!";
	}

	if (!attr.msg_qbytes) return 0;

	return attr.msg_cbytes * 100 / attr.msg_qbytes;
#else
	int i;

	i = getMessageNumber();

	if (!m_iMaxNum)
		return 0;

	return i * 100 / m_iMaxNum;
#endif
}

//##ModelId=4314101D006B
MessageQueue::~MessageQueue()
{
	close();

	if (m_sMQName)
		delete[] m_sMQName;

	if (m_pBuff)
		delete[] m_pBuff;
}

//##ModelId=4314101D006D
bool MessageQueue::_open()
{
	if (m_bAttached)
		return true;

#ifdef SYSTEM_V
	{
		//##SYSTEM_V下的操作
//		struct msqid_ds attr;
//		int ret;

		//int key = ftok (m_sMQName, '0');

		m_iSysMQID = msgget (m_iKey, MQ_PERMISSION);
		if (m_iSysMQID == -1)
		{
			//##异常处理
			goto _FALSE;
		}

		//##获取消息队列相关信息

		/*
		 ret = msgctl (m_iSysMQID, IPC_STAT, &attr);
		 if (ret == -1)
		 {
		 //##异常处理
		 goto _FALSE;
		 }

		 m_iMaxLen = attr.__unused5;
		 m_iMaxNum = attr.__unused4;
		 */

		if (m_pBuff)
		delete [] m_pBuff;

		m_pBuff = new char[m_iBufLen+sizeof(long)];
		if (!m_pBuff)
		{
			//##异常处理
			m_iErrorNo = E_NOMEM;
			return false;
		}

	}
#else
	{
		//##POSIX下的操作
		struct mq_attr attr;
		int ret;

		attr.mq_flags = m_iBlock ? 0 : O_NONBLOCK;

		m_iSysMQID = mq_open(m_sMQName, O_RDWR, MQ_PERMISSION);
		if (m_iSysMQID == -1)
		{
			//##异常处理
			goto _FALSE;
		}

		ret = mq_setattr(m_iSysMQID, &attr, NULL);
		if (ret == -1)
		{
			//##异常处理
			m_iErrorNo = _getStandardError(errno);
			mq_close(m_iSysMQID);
			return false;
		}

		//##获取消息队列相关信息
		ret = mq_getattr(m_iSysMQID, &attr);
		if (ret == -1)
		{
			//##异常处理
			m_iErrorNo = _getStandardError(errno);
			mq_close(m_iSysMQID);
			return false;
		}

		m_iMaxLen = attr.mq_msgsize;
		m_iMaxNum = attr.mq_maxmsg;
	}
#endif

	m_bAttached = true;
	return true;

	_FALSE: m_iErrorNo = _getStandardError(errno);
	return false;
}

//##ModelId=4314101D006E
bool MessageQueue::_create()
{
	//##创建（设定消息队列大小、打开
#ifdef SYSTEM_V
	{
		//##SYSTEM_V下面的操作
		//int key = ftok (m_sMQName, '0');
		struct msqid_ds attr;
		int ret;

		m_iSysMQID = msgget (m_iKey, MQ_PERMISSION | IPC_CREAT | IPC_EXCL);
		if (m_iSysMQID == -1)
		{
			//##异常处理
			goto _FALSE;
		}
		//##设定大小
		ret = msgctl (m_iSysMQID, IPC_STAT, &attr);
		if (ret == -1)
		{
			//##异常处理
			goto _FALSE;
		}

		if ((int)attr.msg_qbytes != m_iMaxNum*m_iMaxLen)
		{
			attr.msg_qbytes = m_iMaxNum * m_iMaxLen;
			//##试验一下是否行得通
			//##试验证明，不行！
			/*{{{
			 attr.__unused4 = m_iMaxNum;
			 attr.__unused5 = m_iMaxLen;
			 }}}*/

			ret = msgctl (m_iSysMQID, IPC_SET, &attr);
			if (ret == -1)
			{
				//##异常处理
				int eb = errno;
				msgctl (m_iSysMQID, IPC_RMID, NULL);
				errno = eb;
				goto _FALSE;
			}
		}

		if (m_pBuff)
		delete [] m_pBuff;

		m_pBuff = new char[m_iBufLen+sizeof(long)];
		if (!m_pBuff)
		{
			m_bAttached = false;
			m_iErrorNo = E_NOMEM;
			return false;
		}

	}
#else
	{
		//##POSIX下面的操作
		struct mq_attr attr;
		int ret;

		attr.mq_flags = m_iBlock ? 0 : O_NONBLOCK;
		attr.mq_maxmsg = m_iMaxNum;
		attr.mq_msgsize = m_iMaxLen;

		m_iSysMQID = mq_open(m_sMQName, O_RDWR | O_CREAT | O_EXCL, MQ_PERMISSION, &attr);
		if (m_iSysMQID == -1)
		{
			//##异常处理
			goto _FALSE;
		}

		ret = mq_setattr(m_iSysMQID, &attr, NULL);
		if (ret == -1)
		{
			//##异常处理
			goto _FALSE;
		}
	}
#endif

	m_bAttached = true;
	return true;

	_FALSE: m_iErrorNo = _getStandardError(errno);
	return false;
}

//##ModelId=4314101D006F
int MessageQueue::_getStandardError(int err)
{
	/*
	 SYSTEM_V                                         POSIX

	 EACCES  NO PERMISSION
	 EFAULT  POINTER UNACCESSIBLE
	 EIDRM   MQ REMOVED
	 EINVAL  INVALID VALUE(mqid,parameter value)	EBADF,EINVAL
	 EPERM	NO PERMISSION
	 EEXIST	MQ ALREADY EXIST
	 ENOENT  MQ DOES NOT EXIST
	 ENOMEM  NOT ENOUGH MEMORY
	 ENOSPC  CAN NOT CREATE MORE MQ
	 E2BIG
	 EAGAIN
	 ENOMSG

	 NOT SUPPORTED BY THE IMPLEMENTATION	ENOSYS
	 TOO MANY

	 */
#ifdef SYSTEM_V
	switch (err)
	{
		case EFAULT:
		return E_INVAL;
		case EIDRM:
		return E_NOENT;
		case EINVAL:
		return E_INVAL;
		case EPERM:
		return E_PERM;
		case EEXIST:
		return E_EXIST;
		case ENOENT:
		return E_NOENT;
		case ENOMEM:
		return E_NOMEM;
		case ENOSPC:
		return E_SYS;
		case ENOMSG:
		return E_NOMSG;
		case EINTR:
		return E_INTR;
		case EAGAIN:
		return E_FULL;
		default:
		return E_UNKNW;
	}
#else
	switch (err)
	{
	case EBADF:
		return E_INVAL;
//		case ENOSYS:
//			return E_NOSYS;
	case EINVAL:
		return E_INVAL;
//		case EACCESS:
//			return E_PERM;
	case EEXIST:
		return E_EXIST;
	case EINTR:
		return E_INTR;
	case EMFILE:
		return E_SYS;
	case ENOENT:
		return E_NOENT;
	case ENOSPC:
		return E_SYS;
	case EAGAIN:
		return E_NOMSG;
	case EMSGSIZE:
		return E_SYS;
	default:
		return E_UNKNW;
	}
#endif
}

//##ModelId=4314101D006C
MessageQueue::operator bool()
{
	return m_sMQName;
	// return m_bAttached;
}
//##ModelId=432E88AD022F
int MessageQueue::send(string &str, long lMtype)
{
	//##这里的实现不考虑优先级
	string strtemp;
	int ret;
	int len = str.size();

	if (!len)
		return 0;

	if (len > m_iBufLen)
		throw "error!";
#ifdef CHECK_MSG_NUM
	else if (len < m_iBufLen && getMessageNumber()> = m_iMaxNum)
	{
		int iSleepStep = 50000;
		usleep(iSleepStep);
		while (getMessageNumber() >= m_iMaxNum)
		{
			iSleepStep += 50000;
			if (iSleepStep > 500000)
			iSleepStep = 50000;
			usleep(iSleepStep);
		}
	}
#endif

#ifdef SYSTEM_V

	strtemp.resize(sizeof(long),0);

	void *p = (void *)strtemp.data();

	if (lMtype==0)
	*((long *)p) = len; //modified by linjinxin
	else
	*((long *)p) = lMtype;

	strtemp+=str;

	ret = msgsnd (m_iSysMQID, strtemp.data(), len, (m_iBlock ? 0 : IPC_NOWAIT));
#else
	ret = mq_send(m_iSysMQID, str.data(), len, 1);
#endif

	if (ret == -1)
	{
		//##异常处理
		m_iErrorNo = _getStandardError(errno);

		if (m_iErrorNo == E_NOMSG)
			m_iErrorNo = E_FULL; //POSIX

		if (m_iErrorNo == E_FULL)
			return 0;

		if (m_iErrorNo == E_NOENT)
			m_bAttached = false;

		throw "error!";
//		return -1;
	}

	return len;
}

//##ModelId=432E88D90188
int MessageQueue::receive(string &str, long lMtype)
{
	//##如果没有消息，返回空
	/*
	 2005-05-24
	 接收消息的时候，忽略信号引起的中断
	 */
	int ret;

	string strtemp(m_iBufLen + sizeof(long), 0);

#ifdef SYSTEM_V
	//memset ((char *)m_pBuff, 0, m_iBufLen+sizeof(long));

	__RECEIVE2:

	//ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
	//ret = msgrcv (m_iSysMQID, (void *)strtemp.data(), m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
	ret = msgrcv (m_iSysMQID, (void *)strtemp.data(), m_iBufLen, lMtype, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
	/*
	 If msgtyp is 0, then the first message in the queue is read.

	 If msgtyp is greater than 0, then the first message on the queue
	 of  type  msgtyp  is  read,  unless  MSG_EXCEPT  was asserted in
	 msgflg, in which case the first message on the queue of type not
	 equal to msgtyp will be read.

	 If  msgtyp  is  less than 0, then the first message on the queue
	 with the lowest type less than or equal to the absolute value of
	 msgtyp will be read.


	 */
	if (ret >=0)
	{
		//memcpy (buf, (char *)m_pBuff+sizeof(long), ret);
		//str.assign(strtemp,sizeof(long),strtemp.size()-sizeof(long));
		str.assign(strtemp,sizeof(long),ret);
	}
	else
	{
		m_iErrorNo = _getStandardError (errno);

		if (m_iErrorNo == E_NOMSG)
		{
			str="";
			return 0;
		}
		if (m_iErrorNo == E_NOENT)
		m_bAttached = false;

		if (m_iErrorNo == E_INTR)
		{
			printf ( "接收消息过程中收到信号，忽略信号...");
			goto __RECEIVE2;
		}

		throw "error!";
	}

	return ret;
#else
	/*struct mq_attr attr;
	 ret = mq_getattr (m_iSysMQID, &attr);
	 if (ret == -1)
	 {
	 m_iErrorNo = _getStandardError (errno);
	 return -1;
	 }
	 */

	__RECEIVE2: ret = mq_receive(m_iSysMQID, (char *) strtemp.data(), /*attr.mq_msgsize*/m_iMaxLen, NULL);

	if (ret == -1)
	{
		m_iErrorNo = _getStandardError(errno);
		if (m_iErrorNo == E_NOMSG)
			return 0;

		if (m_iErrorNo == E_INTR)
		{
			printf("接收消息过程中收到信号，忽略信号...");
			goto __RECEIVE2;
		}

		throw "error!";
	}
	str = strtemp;
	return ret;
#endif
}

//##ModelId=4331083B0017
void MessageQueue::setBlockFlag(bool ifblock)
{
	m_iBlock = ifblock;
}

void MessageQueue::setMaxNum(int maxnum)
{
#ifdef CHECK_MSG_NUM
	if (!m_bAttached)
	return;
	if (maxnum > m_iMaxNum)
	m_iMaxNum = maxnum;
#endif
}

bool MessageQueue::getMessageInfo(int& iMsqNum,int& iMsqTotal,int& iMsqKB,int& iMsqTotalKB,int& iPercent)
{
	if (!m_bAttached)
	{
		m_iErrorNo = E_CLOSE;
		return false;
	}

#ifdef SYSTEM_V
		struct msqid_ds attr;
		//int ret;

		//##获取消息队列相关信息
		if (msgctl (m_iSysMQID, IPC_STAT, &attr))
		{
			//##异常处理
			m_iErrorNo = _getStandardError (errno);
			if (m_iErrorNo == E_NOENT)
				m_bAttached = false;

			throw
				"throw!";
		}


		iMsqNum = attr.msg_qnum;
		iMsqTotal = m_iMaxNum;
		iMsqKB = attr.msg_cbytes / 1024;
		iMsqTotalKB = attr.msg_qbytes/1024;
		if ( 0== attr.msg_qbytes)
			 iPercent=0;
		else
		     iPercent = attr.msg_cbytes * 100 / attr.msg_qbytes;

#else
		struct mq_attr attr;

		//##获取消息队列相关信息
		if (mq_getattr (m_iSysMQID, &attr))
		{
			//##异常处理
			m_iErrorNo = _getStandardError (errno);
			//return -1;
			throw
				"throw!";
		}

		//return attr.mq_curmsgs;
		iMsqNum = attr.mq_curmsgs;
		iMsqTotal = m_iMaxNum;
		iMsqKB = attr.mq_msgsize / 1024;
		iMsqTotalKB = attr.mq_maxmsg/1024;
		if ( 0== attr.mq_maxmsg)
			 iPercent=0;
		else
		     iPercent = attr.mq_msgsize * 100 / attr.mq_maxmsg;
#endif


    return true;
}
