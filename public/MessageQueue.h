#ifndef _MESSAGEQUEUE_H_
#define _MESSAGEQUEUE_H_
//#include <curses.h>
#include <string>
#include <mqueue.h>
using namespace std;
//##ModelId=4314101B0389
//##Documentation
//## 消息队列封装类，
//## 支持POSIX和SYSTEM V两种标准
//## 当SYSTEM V标准的时候，要求#define SYSTEM_V

/******************************************************************************
 在SYSTEM_V情况下和POSIX情况下，MessageQueue使用方法的区别：

 操作	SYSTEM_V					POSIX
 ----------------------------------------------------------------
 创建	要求有创建者有超级用户		不需要
 权限，因为默认只有16k
 大小的控件，通常需要扩
 展空间

 打开	要求所以使用该消息队列		打开的时候无需制定，自动获取
 的程序在打开或创建的时		创建时用的参数值。
 候使用的maxnum，maxlen
 相同。

 关闭	其实没有关闭				关闭了

 其他一样

 参考资料：SYSTEM_V if cmd is IPC_SET,an attempt is being made
 to increase to the value of msg_qbytes,and the calling process does not
 have the SUPER-USER PRIVILEGE.

 当系统POSIX的时候，建议使用POSIX的消息队列。
 ******************************************************************************/

class MessageQueue
{
public:
	//##ModelId=4314101D004F
	//##Documentation
	//## 打开消息队列，制定方式：
	//## 1，是否阻塞
	//## 2，如果不存在是否创建。
	void open(bool ifblock = true, bool ifcreate = true, int maxlen = 2048, int maxnum = 1000);

	//##ModelId=4314101D0059
	//##Documentation
	//## 删除一个已经存在的队列
	void remove();

	//##ModelId=4314101D005A
	//##Documentation
	//## 断开到某队列的连接
	void close();

	//##ModelId=4314101D005B
	//##Documentation
	//## 返回该队列是否存在
	bool exist();

	//##ModelId=4314101D0061
	//##Documentation
	//## 发送一段数据
	int send(void *data, int len, long lMtype);
	int send(void *data, int len);

	//##ModelId=4314101D0064
	//##Documentation
	//## 从消息队列接收一段数据
	int receive(char *buf, long lMtype);
	int receive(char *buf);
	//##ModelId=4314101D0066
	//##Documentation
	//## 队列内的消息条数
	int getMessageNumber();

	//##ModelId=4314101D0067
	//##Documentation
	//## 构造函数
	/*MessageQueue(
	 //##Documentation
	 //## 消息队列的名字，要求：
	 //## 是一个文件或者目录的完整绝对路径，如：
	 //## /home/bill/mqfile/process1000.input
	 //## （注意，路径中不能包括"."，".."，如：/home/bill/../bill/mqfile/process1000.input）
	 char *mqname);*/
	MessageQueue(int key);

	//##ModelId=4314101D0069
	//##Documentation
	//## 返回消息队列的IPCID
	int getSysID();

	//##ModelId=4314101D006A
	//##Documentation
	//## 返回使用百分比，精确到整数
	int getOccurPercent();

	//##ModelId=4314101D006B
	~MessageQueue();
	//##ModelId=4314101D006C
	operator bool();
	//##ModelId=432E88AD022F
	int send(string &str, long lMtype = 0);

	//##ModelId=432E88D90188
	int receive(string &str, long lMtype = 0);
	//##ModelId=4331083B0017
	void setBlockFlag(bool ifblock = true);

	bool getBlockFlag()
	{
		return m_iBlock;
	}
	;
	bool getMessageInfo(int& iMsqNum,int& iMsqTotal,int& iMsqKB,int& iMsqTotalKB,int& iPercent);
	void setMaxNum(int maxnum);

	//##ModelId=4314101B0393
	enum ERRTYPE
	{
		E_PERM = 1, E_NOENT, E_INVAL, E_EXIST, E_NOMEM, E_SYS, E_NOMSG, E_INTR, E_CLOSE, E_FULL, E_UNKNW
	};
	//原始十进制KEY，2018-03-24
	int m_oriKey;
private:
	//##ModelId=4314101D006D
	bool _open();

	//##ModelId=4314101D006E
	bool _create();
	//##ModelId=4314101D006F
	int _getStandardError(int err);

	//##ModelId=4314101D0032
	int m_iKey;

	//##ModelId=4314101D0039
	int m_iSysMQID;

	//##ModelId=4314101D003A
	char *m_sMQName;

	//##ModelId=4314101D0043
	//##Documentation
	//## 容纳的最大消息数
	int m_iMaxNum;

	//##ModelId=4314101D0044
	//##Documentation
	//## 每条消息的最大长度
	int m_iMaxLen;

	//##ModelId=4314101D0045
	int m_iBlock;

	//##ModelId=4314101D0046
	bool m_bAttached;
	//##ModelId=4314101D004D
	int m_iErrorNo;

	//##ModelId=432E78810204
	char *m_pBuff;
	//##ModelId=432E789F01D5
	int m_iBufLen;

};

#define SYSTEM_V

#ifdef SYSTEM_V
#define MQ_PERMISSION	0666
#else
#define MQ_PERMISSION	0660
#endif

#endif /* MESSAGEQUEUE_H_HEADER_INCLUDED_BDFE1869 */
