#include <string>
#include <iostream>
#include <time.h>
#include <errno.h>

#include "dat2cli_t_old.h"

using namespace std;

extern int errno;
static TcpSocket *g_pTcpSocket = NULL; //Tcp传输器

int iMyPid=0;
char sLogTime[15],sMSec[4];

#include "dat2cli_supp.h"

void startIosThread(boost::asio::io_service *pio)
{
	pio->run();
}
void signalProcess(int isignal)
{
	if (isignal == SIGUSR1){
		Dat2Client::m_bReadSysMq = true;
		return;
	}

	if (isignal == SIGINT||isignal==SIGTERM){
		Dat2Client::m_bExitFlag = true;
		printf("%s.%s pid=%d(%d) CATCH SIG=%d.\n",
			sLogTime,sMSec,getpid(),iMyPid,isignal);
		
	//只有是子进程的情况下才退出，如果是首要进程，则不直接下去exit
		if(getpid()!=iMyPid) return;
	}
	exit(0);
}
void SetProcessSignal()
{
	// 设置信号处理
	signal(SIGQUIT, signalProcess);
	signal(SIGTERM, signalProcess);
	signal(SIGINT, signalProcess);
	signal(SIGSEGV, signalProcess);
	signal(SIGILL, signalProcess);
	signal(SIGABRT, signalProcess);
	signal(SIGFPE, signalProcess);
	signal(SIGPIPE, signalProcess);

	signal(SIGUSR1, signalProcess);
}

bool Dat2Client::m_bReadSysMq = true;
bool Dat2Client::m_bExitFlag = false;


Dat2Client::Dat2Client()
{
	memset(m_sUserPrivJsonPath, 0, sizeof(m_sUserPrivJsonPath));
	memset(m_sOutDispJsonPath, 0, sizeof(m_sOutDispJsonPath));
	memset(m_sCfgJsonPath, 0, sizeof(m_sCfgJsonPath));
	
	memset(m_sWorkRootPath, 0, sizeof(m_sWorkRootPath));
	memset(m_sWriteUser, 0, sizeof(m_sWriteUser));


	strcpy(m_sWorkRootPath,"/stock/work");

	bzero((void*)&m_Info,sizeof(ServerInfo_t));

	m_iCheckIp = 0;
	m_iSetSocketFlag = 0;
	m_iPid = 0;

	m_poSysMQ = NULL;
	m_poDataMQ = NULL;

	m_pTimerController = NULL;
	m_pThread = NULL;

	m_mapUserAuth.clear();

	m_Conn.setAllSubs.clear();
	m_Conn.setSubscribed.clear();
	m_Conn.setSubsCode.clear();

	m_Conn.strUserName = "";
	m_Conn.iMqId = 0;

	m_Conn.lRecvCnt=0;
	m_Conn.lSendCnt=0;
	m_Conn.mapSubsStat.clear();
}

Dat2Client::~Dat2Client()
{
	m_io.stop();

	m_mapUserAuth.clear();
	if (m_poSysMQ != NULL){
		delete m_poSysMQ;
		m_poSysMQ = NULL;
	}
	if (m_pTimerController != NULL){
		delete m_pTimerController;
		m_pTimerController = NULL;
	}
	if (m_pThread != NULL){
		delete m_pThread;
		m_pThread = NULL;
	}
	if (m_iPid != 0)
		kill(m_iPid, SIGUSR1);
}

bool Dat2Client::isValidHost(const char *sIp)
{
	//sIp验证
	for (auto it = m_mapUserAuth.begin(); it != m_mapUserAuth.end(); ++it){
		if (!strcmp(it->second.strIp.c_str(), sIp))
			return true;
	}
	return false;
}

int Dat2Client::Init()
{

	if(ReadServerInfo(m_sCfgJsonPath,m_Info)<0) return -1;


	m_poSysMQ = new MessageQueue(m_Info.iSysMqKey);
	if (!m_poSysMQ){
		string msg="error create mq id="+to_string(m_Info.iSysMqKey);
		throw msg;
	}

	m_poSysMQ->open(false, true, m_Info.iSysMqMaxLen, m_Info.iSysMqMaxNum);

	char sSemName[32];

	sprintf(sSemName, "%d", m_Info.iSemLockKey);
	m_semLock.getSem(sSemName, 1, 1);
	
	if(ReadUserAuth2Map(m_sUserPrivJsonPath,m_mapUserAuth)<0) return -1;

	return 0;
}

int Dat2Client::run(int argc, char *argv[])
{
	SetProcessSignal();
	for (int c; (c = getopt(argc, argv, "c:p:k:r:u:o:w:?:")) != EOF;){
		switch (c)
		{
		case 'c':
			m_iCheckIp = atoi(optarg);
			break;
		case 'k':
			m_iSetSocketFlag = atoi(optarg);
			break;
		case 'p':
			strcpy(m_sCfgJsonPath, optarg);
			break;
		case 'r':
			strcpy(m_sOutDispJsonPath, optarg);
			sprintf(m_sOutDispLogPath,"%s.log",m_sOutDispJsonPath);
			break;
		case 'u':
			strcpy(m_sUserPrivJsonPath, optarg);
			break;
		case 'o':
			strcpy(m_sWorkRootPath, optarg);
			break;
		case 'w':
			strcpy(m_sWriteUser, optarg);
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-p cfg-path ]\n");
			printf("   [-r disp-path ]\n");
			printf("   [-u user-privlegepath ]\n");
			printf("   [-o work-root-path ]\n");
			printf("   [-w write-user ]\n");
			printf("   [-d DebugFlag ]\n");
			printf("   [-c 1-Check IP;0-UnCheck IP ]\n");
			printf("   [-k 1:set socket buffer len 0: do not set socket buffer len]\n");
			exit(1);
			break;
		}
	}

	Init();

	int ret,iPort;
	long iDat2CliPid, iCliReqProcPid;
	char sDestIp[31] = { 0 };


	if (signal(SIGCHLD, SIG_DFL) != SIG_ERR)
		signal(SIGCHLD, SIG_IGN);

	g_pTcpSocket = new TcpSocket;
	struct sockaddr_in remoteaddr;

	while (true){

		ret = g_pTcpSocket->listen(m_Info.iPort, remoteaddr);
		if (ret < 0 && ret != -2) return -1;

		sleep(1);
		g_pTcpSocket->CloseListenSocket();

		//校验接入IP是否合法
		iPort=ntohs(remoteaddr.sin_port);
		strcpy(sDestIp, inet_ntoa(remoteaddr.sin_addr));

		GetHostTimeX(sLogTime,sMSec);

		if (m_iCheckIp)	{
			if (!isValidHost(sDestIp)){

				printf("%s.%s 未知IP:%s;拒绝接入.\n",sLogTime,sMSec,sDestIp);

				g_pTcpSocket->Close();
				continue;
			}
		}
		printf("%s.%s IP:%s;接入成功.\n",sLogTime,sMSec,sDestIp);

		if (m_iSetSocketFlag){
			if (m_Info.iSocketSendLen > 0)
				g_pTcpSocket->setSocketBufferLen(true, (int&)m_Info.iSocketSendLen);

			if (m_Info.iSocketRecvLen > 0)
				g_pTcpSocket->setSocketBufferLen(false,(int&) m_Info.iSocketRecvLen);
		}

		//int m_iPid; //收发进程记录与它配对应的PID 主进程记录为0
		switch (fork()){
		case -1://fork失败情况
			exit(1);
			break;
		case 0:
			iDat2CliPid = getpid();
//			cout << "["<<sSysDate<<"]:"<<"连接成功%s:"<<inet_ntoa(remoteaddr.sin_addr)<<":"<<ntohs(remoteaddr.sin_port)<<"connected"<<endl;

			GetHostTimeX(sLogTime,sMSec);
			printf("%s.%s IP:连接成功%s:%dconnected.\n",sLogTime,sMSec,sDestIp,iPort);

			iCliReqProcPid = fork();
			switch (iCliReqProcPid)
			{
			case -1:
				exit(1);
				break;
			case 0:
				//处理客户端发送的订阅请求进程
				m_iPid = iDat2CliPid;
				ret=RecvCliSockRequestAndProcess(sDestIp,iPort);
				g_pTcpSocket->Close();
				kill(iDat2CliPid, SIGINT);
				logout();
				
				GetHostTimeX(sLogTime,sMSec);
				printf("%s.%s CLICMD EXIT user=%s(%s:%d-p-%d) code=%d.\n",
					sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),ret);
					
				break;
			default://转发MQ数据到客户端SOCK连接进程

				m_iPid = iCliReqProcPid;

				ret=RecvMqAndDisp2Cli(sDestIp,iPort);
				g_pTcpSocket->Close();
				kill(iCliReqProcPid, SIGINT);

				GetHostTimeX(sLogTime,sMSec);
				printf("%s.%s RECVMQ EXIT user=%s(%s:%d-p-%d) code=%d.\n",
					sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),ret);
				break;
			}

			return 0;
			break;
		default:
			g_pTcpSocket->Close();
			break;
		}
	}

	g_pTcpSocket->CloseListenSocket();
	g_pTcpSocket->Close();

	delete g_pTcpSocket;

	return 0;
}

int Dat2Client::RecvCliSockRequestAndProcess(char sDestIp[],int iPort)
{
	string msg;
	int recv_len, msg_len, ret;
	unsigned char *data = (unsigned char*)m_sRecvBuffer;


	GetHostTimeX(sLogTime,sMSec);
	printf("%s.%s CLICMD process start user=%s(%s:%d-p-%d).\n",
		sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());

	memset(m_sRecvBuffer, 0, sizeof(m_sRecvBuffer));

	while(!Dat2Client::m_bExitFlag){

		msg_len = 2;
		recv_len = errno = 0;

		//做一个循环，从客户端socket中读取消息长度data中
		while(recv_len < msg_len){
			if ((ret = g_pTcpSocket->read((data + recv_len), msg_len - recv_len)) <= 0){
				printf("%s.%s user=%s(%s:%d-p-%d) READ ERROR code=%d.\n",
					sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),ret);

				return ERROR_TRANSPORT;
			}
			recv_len += ret;
		}

		msg_len = data[0] * 256 + data[1];

		//校验消息长度的合法性，如果非法则断开
		if (msg_len <= 0||msg_len > (SOCKET_DATALEN - 2)){ //消息长度有误
			printf("%s.%s 消息长度有误，msg_len：%d!\n",sLogTime,sMSec,msg_len);
			return ERROR_TRANSPORT;
		}


#ifdef DEBUG_ONE
		GetHostTimeX(sLogTime,sMSec);
		printf("%s.%s user=%s(%s:%d-p-%d) Recv BizCode[%d]Msg_Len[%d].\n",
			sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),(int)data[3],msg_len);
#endif

		//得到有效的消息长度后，则读取整个消息
		while(recv_len - 2 < msg_len){
			if ((ret = g_pTcpSocket->read((unsigned char *) (data + recv_len), msg_len)) <= 0){
				
				GetHostTimeX(sLogTime,sMSec);
				
				printf("%s.%s g_pTcpSocket读取失败.\n",sLogTime,sMSec);
				
				return ERROR_TRANSPORT;
			}
			recv_len += ret;
		}

		msg.assign((char*)data+2, msg_len);

		if(dealCommand(msg,sDestIp,iPort)==false){
			printf("%s.%s user=%s(%s:%d-p-%d) PROCESS CMD ERROR.\n",
				sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());

			return ERROR_TRANSPORT;
		}
	}
	return SUCC;
}
int Dat2Client::RecvMqAndDisp2Cli(char sDestIp[],int iPort)
{
	int iRet = 0,iDataMqID = 0,iCount=0;
	string strRecv;

	GetHostTimeX(sLogTime,sMSec);
	printf("%s.%s RECVMQ process user=%s(%s:%d-p-%d).\n",
		sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());

	while (!Dat2Client::m_bExitFlag){

		if (m_bReadSysMq){ //总控队列用于接收MQID，通过信号触发

			//接收登录的MQ信号，得到MQID
			if((iRet = m_poSysMQ->receive(strRecv, m_iPid))!=0){
				m_bReadSysMq = false;
				iDataMqID = atoi(strRecv.c_str());
				m_poDataMQ = new MessageQueue(iDataMqID);

				if (!m_poDataMQ)
					throw "error";

				m_poDataMQ->open(false, true, m_Info.iSysMqMaxLen, m_Info.iSysMqMaxNum);

				//从系统消息队列中拿到用户名
				char *pUserName=strchr((char*)strRecv.c_str(),',');
				if(pUserName==NULL) return ERROR_TRANSPORT;

				m_Conn.strUserName=string(pUserName+1);

				if(UserInWriteUser((char*)m_Conn.strUserName.c_str(),m_sWriteUser))
					Write2CliFile=MyWrite2CliFile;
				else	Write2CliFile=NULL;
				continue;
			}

			//如果没收到队列的MQ信号，就休眠一下等待
			usleep(10000);
			continue;
		}

		//这里poDataMQ已经准备好了
		//如果接收到消息为空，没有消息，则休眠0.1ms继续接收
		if((iRet = m_poDataMQ->receive(strRecv, 0))<=0){
			usleep(100);
			continue;
		}

		//做一个统计信息输出到终端
		
		m_Conn.lRecvCnt++;
		
		int iSubs=(int)((unsigned char) (strRecv[2]));
		
		m_Conn.mapSubsStat[iSubs]++;

		if((m_Conn.lRecvCnt)%3000==0){

			int l=0;
			char sTmp[256];
				
			strcpy(sTmp,"");

			GetHostTimeX(sLogTime,sMSec);

			for(auto it=m_Conn.mapSubsStat.begin(); it!=m_Conn.mapSubsStat.end(); it++){
				if(l==0)
					l+=sprintf(sTmp,"[%d:%ld]",	it->first,it->second);
				else	l+=sprintf(sTmp+l,"\t[%d:%ld]",	it->first,it->second);
			}

			printf("%s.%s pid=%d user=%s(%s:%d-p-%d)sendcnt=%ld{%s}\n",
				sLogTime,sMSec,getpid(),m_Conn.strUserName.c_str(),
				sDestIp,iPort,getpid(),m_Conn.lRecvCnt,sTmp);
		}

		unsigned char *data = (unsigned char*)m_sSendBuffer;
		int msg_len = strRecv.size(),num = 0;

		memcpy(m_sSendBuffer, strRecv.c_str(), strRecv.size());

		while(num < msg_len){
			if((iRet = g_pTcpSocket->write((data + num), msg_len - num))<0){
				printf("write socket error count=%d.\n",iCount);
				return ERROR_TRANSPORT;
			}
			num += iRet;
		}

		if(Write2CliFile!=NULL){
			if(Write2CliFile(m_sWorkRootPath,(char*)m_Conn.strUserName.c_str(),strRecv)<0)
				return ERROR_TRANSPORT;
		}
		if(((++iCount)%30000)==0)
			printf("pid=%d,the mqid=%d\tprocessed count=%d.\n",getpid(),m_poDataMQ->m_oriKey,iCount);
	}
	return SUCC;
}


bool Dat2Client::dealCommand(string &msg,char sDestIp[],int iPort)
{
	bool bret = true;
	BizCode iBizCode;
	string msgProtobuf;

	getBizcode(iBizCode, msgProtobuf, msg);

	switch (iBizCode){

	case LOGIN_REQ:
	{
		LoginRequest req;
		req.ParseFromString(msgProtobuf);

		Reply rep;
		ErrCode errcode = ErrCode::OTHER;

		string msgRep,msgBody;

		const string pswdInput = req.password();
		const auto itAuth = m_mapUserAuth.find(req.name());

		m_Conn.strUserName = req.name();

		GetHostTimeX(sLogTime,sMSec);
		printf("%s.%s Recv LOGIN_REQ user=%s(%s:%d-p-%d) passwd=%s.\n",
			sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),pswdInput.c_str());
		PrintHexBuf((char*)(msg.c_str()),msg.size());

		m_Conn.iMqId = itAuth->second.iMqId;

		rep.set_err(errcode);
		//先查询用户名是否有效,无效则返回客户端
		if (itAuth == m_mapUserAuth.cend()){
			rep.set_desc("user_invalid");
			addBizcode(msgBody, rep, BizCode::LOGIN_REP);
			g_pTcpSocket->send((unsigned char *) msgBody.data(), msgBody.size());
			bret = false;
			printf("%s.%s LOGIN_REP invalid user=%s(%s:%d-p-%d).\n",
				sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());

			break;
		}
		//如果已经登录则，回复已经登录信息
		if(isLogined(m_Conn.strUserName)){
			rep.set_desc("user_have_logined");
			addBizcode(msgBody, rep, BizCode::LOGIN_REP);
			g_pTcpSocket->send((unsigned char *) msgBody.data(), msgBody.size());
			bret = false;
			printf("%s.%s LOGIN_REP user logined user=%s(%s:%d-p-%d).\n",
				sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
			break;
		}
		//密码错误，则将密码错误信息返回客户端
		if(itAuth->second.strPassword != req.password()){
			rep.set_desc("pswd_error");
			addBizcode(msgBody, rep, BizCode::LOGIN_REP);
			g_pTcpSocket->send((unsigned char *) msgBody.data(), msgBody.size());
			bret = false;

			printf("%s.%s LOGIN_REP passwd error user=%s(%s:%d-p-%d).\n",
				sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
			break;
		}

		//用户，密码验证通过了再发送mqid给接收进程
		char sMsg[128];
		sprintf(sMsg, "%d,%s", m_Conn.iMqId,m_Conn.strUserName.c_str());
		string tmpStr=sMsg;
		m_poSysMQ->send(tmpStr, getpid());

		//如果密码验证也通过，则将心跳代码加入
		if(!m_pTimerController){
			//启动定时器
			m_pTimerController  = new TimerController(m_io,boost::bind(&Dat2Client::handleTimeOut,this),m_Info.iHeartBeat*2);
			m_pThread = new std::thread(startIosThread,&m_io);
			m_pThread->detach();
		}
		else{
			m_pTimerController->resetTimer(m_Info.iHeartBeat*2);
		}

		//如果密码验证也通过，并在返回成功后，再返回客户端allsubs信息【pb格式】
		rep.set_err(ErrCode::SUCCESS);
		char tmp[6];
		sprintf(tmp, "%u", m_Info.iHeartBeat);
		msgRep = tmp;

		rep.set_desc(msgRep);
		addBizcode(msgBody, rep, BizCode::LOGIN_REP);

		g_pTcpSocket->send((unsigned char *) msgBody.data(), msgBody.size());

		PbCodesBroadcast pbCodesBroadcast;
		for (const auto code : m_Conn.setAllSubs){
			pbCodesBroadcast.add_codes(code);
		}

		string pbMsg2;

		addBizcode(pbMsg2, pbCodesBroadcast, BizCode::CODES_BROADCAST);
		g_pTcpSocket->send((unsigned char *) pbMsg2.data(), pbMsg2.size());
		
		printf("%s.%s LOGIN_REP login SUCCESS user=%s(%s:%d-p-%d).\n",
			sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
	}
	break;
	case SUBSCRIBLE:
	{
		char sInfo[256],sMsg[256],sErrMsg[256];
	
		SubscribeRequest req;
		req.ParseFromString(msgProtobuf);
		bret = setSubscrible(req);

		GetHostTimeX(sLogTime,sMSec);

		sprintf(sInfo,"Recv SUBSCRIBLE user=%s(%s:%d-p-%d) Msg:Sub_Size=%d",
			m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),req.sub_size());

		printf("%s.%s %s.\n",sLogTime,sMSec,sInfo);

		PrintHexBuf((char*)(msg.c_str()),msg.size());
		
		//这里将改变之后的文件，全量写入到 disp.json.log文件中
		MyBin2HexStr((char*)(msg.c_str()),msg.size(),sMsg);
		if(LogDispJson(sInfo,sMsg,m_sOutDispJsonPath,m_sOutDispLogPath,sErrMsg)==false){
			GetHostTimeX(sLogTime,sMSec);
			printf("%s.%s SUBSCRIBLE %s user=%s(%s:%d-p-%d).\n",
				sLogTime,sMSec,sErrMsg,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
			bret = false;
		}
	}
		break;
	case HEART_BEAT:
	{
#ifdef DEBUG_ONE

		GetHostTimeX(sLogTime,sMSec);
		printf("%s.%s Recv HEART_BEAT MSG user=%s(%s:%d-p-%d).\n",
			sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());

		PrintHexBuf((char*)(msg.c_str()),msg.size());
#endif
		m_pTimerController->resetTimer(m_Info.iHeartBeat*2);
	}
		break;
	case CODES_SUB:
	{
		
		char sInfo[256],sMsg[256],sErrMsg[256];

		PbCodesSub req;

		req.ParseFromString(msgProtobuf);
		const int sz = req.codes_size();

		vector<uint32_t> codes(sz);
		const auto reqCodes = req.codes();
		for (int i = 0; i < sz; ++i)
		{
			codes[i] = reqCodes.operator[](i);
		}
		bret = addReduceCodes(codes, req.add_red());

		GetHostTimeX(sLogTime,sMSec);
		sprintf(sInfo,"Recv CODES_SUB user=%s(%s:%d-p-%d) Msg:codes_size=%d",
			m_Conn.strUserName.c_str(),sDestIp,iPort,getpid(),sz);

		printf("%s.%s %s.\n",sLogTime,sMSec,sInfo);

		PrintHexBuf((char*)(msg.c_str()),msg.size());
		
		//这里将改变之后的文件，全量写入到 disp.json.log文件中
		MyBin2HexStr((char*)(msg.c_str()),msg.size(),sMsg);
		if(LogDispJson(sInfo,sMsg,m_sOutDispJsonPath,m_sOutDispLogPath,sErrMsg)==false){
			GetHostTimeX(sLogTime,sMSec);
			printf("%s.%s CODES_SUB %s user=%s(%s:%d-p-%d).\n",
				sLogTime,sMSec,sErrMsg,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
			bret = false;
		}
	}
		break;
	default:
	{
		bret = false;
		
		GetHostTimeX(sLogTime,sMSec);
		printf("%s.%s Unknow Msg user=%s(%s:%d-p-%d).\n",
			sLogTime,sMSec,m_Conn.strUserName.c_str(),sDestIp,iPort,getpid());
		PrintHexBuf((char*)(msg.c_str()),msg.size());

		break;
	}
	}
	return bret;
}

bool Dat2Client::setSubscrible(const SubscribeRequest &req)
{
	if(m_Conn.strUserName.size() == 0)
		return false;

	if (!req.replay()){

		const auto pri = m_mapUserAuth.find(m_Conn.strUserName);
		const int numSub = req.sub_size();
		BizCode bizCodeTmp;

		m_Conn.setSubscribed.clear();

		for (int i = 0; i < numSub; ++i){

			auto t=pri->second.setAuth;

			bizCodeTmp = BizCode(req.sub(i));
			
			//如果权限找到,对于D31的订购，分别找各个子项目
			if(bizCodeTmp==D31_ITEM){
				if(t.find(180)!=t.cend())	m_Conn.setSubscribed.insert(180);
				if(t.find(183)!=t.cend())	m_Conn.setSubscribed.insert(183);
				if(t.find(185)!=t.cend())	m_Conn.setSubscribed.insert(185);
				if(t.find(18)!=t.cend())	m_Conn.setSubscribed.insert(18);
			}
			else{
				if(t.find(bizCodeTmp)!=t.cend())m_Conn.setSubscribed.insert((int)bizCodeTmp);
			}
		}

		writeDispJson();
	}
	return true;
}

bool Dat2Client::addReduceCodes(const vector<uint32_t> &codes, const bool addFlag)
{
	if(m_Conn.strUserName.size() == 0)
		return false;

	for (const auto code : codes){
		if(addFlag)
			m_Conn.setSubsCode.insert(code);
		else	m_Conn.setSubsCode.erase(code);
	}

	writeDispJson();
	m_semLock.V();
	return true;
}

bool Dat2Client::writeDispJson()
{
	string str = "";
	char sTmp[56] = { 0 };
	bool bfound = false;

	m_semLock.P();

	ptree root, readPt, items, userparam, itemsub, itemsubcodes, root_1;
	read_json(m_sOutDispJsonPath, readPt);

	if (readPt.count("users"))
	{
		ptree ptChildRead = readPt.get_child("users");
		for (auto pos : ptChildRead) //遍历数组
		{
			ptree p = pos.second;
			string user = p.get < string > ("user");
			if (user != m_Conn.strUserName)
			{
				root_1.push_back(pos);
			}
			else
			{
				userparam.put("user", m_Conn.strUserName);
				userparam.put<int>("mqid", m_Conn.iMqId);
				userparam.put<int>("pid", getpid());

				for (auto it = m_Conn.setSubscribed.begin(); it != m_Conn.setSubscribed.end(); ++it)
				{
					sprintf(sTmp, "%u", *it);
					str = sTmp;
					itemsub.push_back(std::make_pair("", ptree(str)));
				}
				userparam.put_child("subscribed", itemsub);

				for (auto it = m_Conn.setSubsCode.begin(); it != m_Conn.setSubsCode.end(); ++it)
				{
					sprintf(sTmp, "%u", *it);
					str = sTmp;
					itemsubcodes.push_back(std::make_pair("", ptree(str)));
				}
				userparam.put_child("subcodes", itemsubcodes);
				root_1.push_back(std::make_pair("", userparam));
				bfound = true;
			}
		}
		if (!bfound)
		{
			userparam.put("user", m_Conn.strUserName);
			userparam.put<int>("mqid", m_Conn.iMqId);
			userparam.put<int>("pid", getpid());

			for (auto iter = m_Conn.setSubscribed.begin(); iter != m_Conn.setSubscribed.end(); ++iter)
			{
				sprintf(sTmp, "%u", *iter);
				str = sTmp;
				itemsub.push_back(std::make_pair("", ptree(str)));
			}
			userparam.put_child("subscribed", itemsub);

			for (auto iter = m_Conn.setSubsCode.begin(); iter != m_Conn.setSubsCode.end(); ++iter)
			{
				sprintf(sTmp, "%u", *iter);
				str = sTmp;
				itemsubcodes.push_back(std::make_pair("", ptree(str)));
			}
			userparam.put_child("subcodes", itemsubcodes);
			root_1.push_back(std::make_pair("", userparam));

		}
		root.put_child("users", root_1);
		write_json(m_sOutDispJsonPath, root);
	}
	else
	{
		userparam.put("user", m_Conn.strUserName);
		userparam.put<int>("mqid", m_Conn.iMqId);
		userparam.put<int>("pid", getpid());

		for (auto iter = m_Conn.setSubscribed.begin(); iter != m_Conn.setSubscribed.end(); ++iter)
		{
			sprintf(sTmp, "%u", *iter);
			str = sTmp;
			itemsub.push_back(std::make_pair("", ptree(str)));
		}
		userparam.put_child("subscribed", itemsub);

		for (auto iter = m_Conn.setSubsCode.begin(); iter != m_Conn.setSubsCode.end(); ++iter)
		{
			sprintf(sTmp, "%u", *iter);
			str = sTmp;
			itemsubcodes.push_back(std::make_pair("", ptree(str)));
		}
		userparam.put_child("subcodes", itemsubcodes);
		root_1.push_back(std::make_pair("", userparam));
		root.put_child("users", root_1);
		write_json(m_sOutDispJsonPath, root);
	}

	m_semLock.V();
	return true;

}
bool Dat2Client::isLogined(string sUserName)
{
	string str = "";
	bool bfound = false;

	m_semLock.P();

	ptree root, readPt, items, userparam, itemsub, itemsubcodes, root_1;
	read_json(m_sOutDispJsonPath, readPt);
	if (readPt.count("users"))
	{
		ptree ptChildRead = readPt.get_child("users");
		for (auto pos : ptChildRead) //遍历数组
		{
			ptree p = pos.second;
			string user = p.get < string > ("user");
			if (user != sUserName)
			{
				root_1.push_back(pos);
			}
			else
			{
				int iPid = p.get<int>("pid",0);
				if (iPid == 0 || 0 != kill(iPid, 0)) //不存在
				{
					userparam.put("user", m_Conn.strUserName);
					userparam.put("pid", getpid());
					root_1.push_back(std::make_pair("", userparam));
					bfound = true;
				}
				else
				{
					m_semLock.V();
					return true;
				}
			}
		}
		if (!bfound)
		{
			userparam.put("user", m_Conn.strUserName);
			userparam.put<int>("pid", getpid());
			root_1.push_back(std::make_pair("", userparam));

		}
		root.put_child("users", root_1);
		write_json(m_sOutDispJsonPath, root);
	}
	else
	{
		userparam.put("user", m_Conn.strUserName);
		userparam.put<int>("pid", getpid());

		root_1.push_back(std::make_pair("", userparam));
		root.put_child("users", root_1);
		write_json(m_sOutDispJsonPath, root);
	}

	m_semLock.V();
	return false;

}
void Dat2Client::handleTimeOut()
{
	GetHostTimeX(sLogTime,sMSec);
	
	printf("%s.%s Client TimeOut! Pid[%d] will exit!",sLogTime,sMSec,getpid());

	kill(m_iPid, SIGINT);
	logout();
	exit(0);
}

void Dat2Client::logout()
{
	string str = "";

	m_semLock.P();

	ptree root, readPt, items, userparam, itemsub, itemsubcodes, root_1;
	read_json(m_sOutDispJsonPath, readPt);
	if (readPt.count("users") && m_Conn.strUserName.size()>0 )
	{
		ptree ptChildRead = readPt.get_child("users");
		for (auto pos : ptChildRead) //遍历数组
		{
			ptree p = pos.second;
			string user = p.get < string > ("user");
			if (user != m_Conn.strUserName)
			{
				root_1.push_back(pos);
			}
			else
			{
				int iPid = p.get<int>("pid",0);
				if (iPid != 0 && iPid!=getpid() && 0 == kill(iPid, 0)) //不存在
				{
					m_semLock.V();
					return ;
				}
			}
		}
		root.put_child("users", root_1);
		write_json(m_sOutDispJsonPath, root);
	}


	//LOGOUT时，将文件备份一下
	char sInfo[256],sErrMsg[256];

	GetHostTimeX(sLogTime,sMSec);

	sprintf(sInfo,"Recv LOGOUT user=%s(p-%d)",m_Conn.strUserName.c_str(),getpid());

	printf("%s.%s %s.\n",sLogTime,sMSec,sInfo);

	if(LogDispJson(sInfo,(char*)"NULL",m_sOutDispJsonPath,m_sOutDispLogPath,sErrMsg)==false){
		GetHostTimeX(sLogTime,sMSec);
		printf("%s.%s LOGOUT %s user=%s(p-%d).\n",
		sLogTime,sMSec,sErrMsg,m_Conn.strUserName.c_str(),getpid());
		exit(1);
	}

	m_semLock.V();
}

int main11(int argc, char *argv[])
{
	iMyPid=getpid();

	Dat2Client oTransMgr;
	oTransMgr.run(argc, argv);
	
	return 0;
}

//将当前CONN结构的信息，写到disp.json文件的对应用户上
#ifdef ___WANT_OLD_FUNCTION_HAHA__
bool writeDispJson11()
{
	string str = "";
	char sTmp[56] = { 0 };
	bool bfound = false;

	DATALOCK.P();

	ptree root, readPt, items, userparam, itemsub, itemsubcodes, root_1;
	read_json(sDispPath, readPt);

	if (readPt.count("users"))
	{
		ptree ptChildRead = readPt.get_child("users");
		for (auto pos : ptChildRead) //遍历数组
		{
			ptree p = pos.second;
			string user = p.get < string > ("user");
			if (user != CONN.strUserName)
			{
				root_1.push_back(pos);
			}
			else
			{
				userparam.put("user", CONN.strUserName);
				userparam.put<int>("mqid", CONN.iMqId);
				userparam.put<int>("pid", CONN.iPid);

				for (auto it = CONN.setSubscribed.begin(); it != CONN.setSubscribed.end(); ++it)
				{
					sprintf(sTmp, "%u", *it);
					str = sTmp;
					itemsub.push_back(std::make_pair("", ptree(str)));
				}
				userparam.put_child("subscribed", itemsub);

				for (auto it = CONN.setSubsCode.begin(); it != CONN.setSubsCode.end(); ++it)
				{
					sprintf(sTmp, "%u", *it);
					str = sTmp;
					itemsubcodes.push_back(std::make_pair("", ptree(str)));
				}
				userparam.put_child("subcodes", itemsubcodes);
				root_1.push_back(std::make_pair("", userparam));
				bfound = true;
			}
		}
		if (!bfound)
		{
			userparam.put("user", CONN.strUserName);
			userparam.put<int>("mqid", CONN.iMqId);
			userparam.put<int>("pid", CONN.iPid);

			for (auto iter = CONN.setSubscribed.begin(); iter != CONN.setSubscribed.end(); ++iter)
			{
				sprintf(sTmp, "%u", *iter);
				str = sTmp;
				itemsub.push_back(std::make_pair("", ptree(str)));
			}
			userparam.put_child("subscribed", itemsub);

			for (auto iter = CONN.setSubsCode.begin(); iter != CONN.setSubsCode.end(); ++iter)
			{
				sprintf(sTmp, "%u", *iter);
				str = sTmp;
				itemsubcodes.push_back(std::make_pair("", ptree(str)));
			}
			userparam.put_child("subcodes", itemsubcodes);
			root_1.push_back(std::make_pair("", userparam));

		}
		root.put_child("users", root_1);
		write_json(sDispPath, root);
	}
	else
	{
		userparam.put("user", CONN.strUserName);
		userparam.put<int>("mqid", CONN.iMqId);
		userparam.put<int>("pid", CONN.iPid);

		for (auto iter = CONN.setSubscribed.begin(); iter != CONN.setSubscribed.end(); ++iter)
		{
			sprintf(sTmp, "%u", *iter);
			str = sTmp;
			itemsub.push_back(std::make_pair("", ptree(str)));
		}
		userparam.put_child("subscribed", itemsub);

		for (auto iter = CONN.setSubsCode.begin(); iter != CONN.setSubsCode.end(); ++iter)
		{
			sprintf(sTmp, "%u", *iter);
			str = sTmp;
			itemsubcodes.push_back(std::make_pair("", ptree(str)));
		}
		userparam.put_child("subcodes", itemsubcodes);
		root_1.push_back(std::make_pair("", userparam));
		root.put_child("users", root_1);
		write_json(sDispPath, root);
	}

	DATALOCK.V();
	return true;

}

bool IsLogined1(string sUserName)
{
	string str = "";
	bool bfound = false;

	DATALOCK.P();

	ptree root, readPt, items, userparam, itemsub, itemsubcodes, root_1;
	read_json(sDispPath, readPt);
	if (readPt.count("users"))
	{
		ptree ptChildRead = readPt.get_child("users");
		for (auto pos : ptChildRead) //遍历数组
		{
			ptree p = pos.second;
			string user = p.get < string > ("user");
			if (user != sUserName)
			{
				root_1.push_back(pos);
			}
			else
			{
				int iPid = p.get<int>("pid",0);
				if (iPid == 0 || 0 != kill(iPid, 0)) //不存在
				{
					userparam.put("user", CONN.strUserName);
					userparam.put("pid", CONN.iPid);
					root_1.push_back(std::make_pair("", userparam));
					bfound = true;
				}
				else
				{
					DATALOCK.V();
					return true;
				}
			}
		}
		if (!bfound)
		{
			userparam.put("user", CONN.strUserName);
			userparam.put<int>("pid", CONN.iPid);
			root_1.push_back(std::make_pair("", userparam));

		}
		root.put_child("users", root_1);
		write_json(sDispPath, root);
	}
	else
	{
		userparam.put("user", CONN.strUserName);
		userparam.put<int>("pid", CONN.iPid);

		root_1.push_back(std::make_pair("", userparam));
		root.put_child("users", root_1);
		write_json(sDispPath, root);
	}

	DATALOCK.V();
	return false;

}
#endif
