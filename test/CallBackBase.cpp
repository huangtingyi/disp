#include "CallBackBase.h"

void Domain2Ip(char sDomain[],char sIp[])
{
	BIO::io_service 	ios;
	BIP::tcp::resolver 	r(ios);
	BIP::tcp::resolver::query q(string(sDomain),string("1000"));

	BIP::tcp::resolver::iterator it = r.resolve(q);

	BIP::tcp::endpoint ep = *it;
	
	ios.stop();
	
	strcpy(sIp,ep.address().to_string().c_str());

}
//将bizCode和pbmsg信息打包成为串 格式：2字节长度+1字节bizCode+pb->SerializeToString结果，长度为 1字节+序列化串
void addBizcode(string &output, const google::protobuf::Message &pbmsg, BizCode bizCode)
{
	string msg;

	pbmsg.SerializeToString(&msg);

	char strBizCode[2] = { 0 };

	strBizCode[0] = bizCode;
	output = string(strBizCode) + msg;

	uint16_t l = uint16_t(htons(output.size()));

	char sLen[3]={0};

	sLen[0] = ((char*)&l)[0];
	sLen[1] = ((char*)&l)[1];
	output.insert(0,sLen,2);
}
//将msgInput的第一个字段取出，作为bizCode后面的串作为pbmsg
void getBizcode(BizCode &bizCode, string &pbmsg, const string &msgInput)
{
	bizCode = BizCode((unsigned char) (msgInput[0]));
	pbmsg = msgInput.substr(1, string::npos);
}
void MainCliSocketRun(BIO::io_service *m_ios)
{

	BIO::io_service::work wk(*m_ios);
	
	m_ios->run();

	m_ios->stop();
}

CallBackBase::CallBackBase(char sIp[],int iPort,char sUserName[],char sPassword[],int iMaxCnt)
{
	char sRealIp[32];

	//如果是域名则变化一下IP
	if(sIp[0]<'0'||sIp[0]>'9')
		Domain2Ip(sIp,sRealIp);
	else	strcpy(sRealIp,sIp);

	m_ip=		string(sRealIp);
	m_port=		iPort;

	m_username=	string(sUserName);
	m_password=	string(sPassword);
	
	m_loginSuccess= false;
	m_exit=		false;
	m_connect=	false;
	
	m_readHeader=	true;
	m_lenBody=	0;
	m_secHeartbeat= 20;

	m_ios=		new BIO::io_service();
	m_socket=	new BIP::tcp::socket(*m_ios);
	m_timerHeartbeat= new BIO::deadline_timer(*m_ios, boost::posix_time::seconds(20));
	
	m_iMktCnt=	0;
	m_iTraCnt=	0;
	m_iQueCnt=	0;
	m_iOrdCnt=	0;
	m_iD31Cnt=	0;
	m_iTotalCnt=	0;
	m_maxcnt=	iMaxCnt;
}
void CallBackBase::toHeartbeat(const BSYS::error_code& errcode)
{
	if (!errcode){
		PbHeartbeat hb;

		hb.set_nothing(false);
		AsyncSend(hb,BizCode::HEART_BEAT);

		m_timerHeartbeat->expires_from_now(boost::posix_time::seconds(m_secHeartbeat));
		m_timerHeartbeat->async_wait(boost::bind(&CallBackBase::toHeartbeat, this,BIO::placeholders::error));
	}
}
void CallBackBase::beginHeartbeat()
{
	toHeartbeat(BSYS::error_code());
}

void CallBackBase::Recv(string &msg)
{
//	int iStockCode=0;

	string msgProtobuf; //剥离了bizcode序列化了的pb数据
	BizCode iBizCode;

	getBizcode(iBizCode, msgProtobuf, msg);

	switch (iBizCode)
	{
	case LOGIN_REP:
	{
		Reply rep;

		rep.ParseFromString(msgProtobuf);
				
		if(rep.err()!=SUCCESS){
			printf("login ERROR exist code=%d msg=%s.\n",
				rep.err(),rep.desc().c_str());
			exit(1);
		}

		m_loginSuccess=true;

		try{
			m_secHeartbeat = atoi(rep.desc().c_str());
		}
		catch (...){
			m_secHeartbeat = 20;
		}

		printf("login SUCCESS HB=%d,%d\n",m_secHeartbeat,atoi(rep.desc().c_str()));

		beginHeartbeat();
	}
		break;
	case MKT_DATA_LV2:
	{
		MktData 	t;
		t.ParseFromString(msgProtobuf);
//		iStockCode=	t.szcode();
//		SendMsg2Cli(iStockCode,'M',msgProtobuf);
		m_iMktCnt++;m_iTotalCnt++;
	}
	break;
	case MKT_DATA_DEAL:
	{
		Transaction 	t;
		t.ParseFromString(msgProtobuf);
//		iStockCode=	t.szcode();
//		SendMsg2Cli(iStockCode,'T',msgProtobuf);
		m_iTraCnt++;m_iTotalCnt++;
	}
	break;
	case SZ_ORDER:
	{
		Order 		t;
		t.ParseFromString(msgProtobuf);
//		iStockCode=	t.szcode();
//		SendMsg2Cli(iStockCode,'O',msgProtobuf);
		m_iOrdCnt++;m_iTotalCnt++;
	}
	break;
	case MKT_DATA_ORDERQUEUE:
	{
		Order_queue	t;
		t.ParseFromString(msgProtobuf);
//		iStockCode=	t.szcode();
//		SendMsg2Cli(iStockCode,'Q',msgProtobuf);
		m_iQueCnt++;m_iTotalCnt++;
	}
	break;
	case D31_ITEM:
	{
		D31Item         t;
		t.ParseFromString(msgProtobuf);
//		iStockCode=	t.nstockcode();
//		SendMsg2Cli(iStockCode,'D',msgProtobuf);
		m_iD31Cnt++;m_iTotalCnt++;
	}
	break;
	default:
	break;
	}
	
	if(m_iTotalCnt>=m_maxcnt)	m_exit=true;

	if((m_iTotalCnt%300)==0)	DisplayStat(iBizCode);	
	
}
void CallBackBase::DisplayStat(BizCode iBizCode)
{
	char sHostTime[15],sMSec[4];
	
	GetHostTimeX(sHostTime,sMSec);

	printf("%s:%s --mkt=%-6d\ttra=%-6d\tque=%-6d\tord=%-6d\td31=%-6d\tall=%-6d\tbiz=%-6d\n",
		sHostTime,sMSec,m_iMktCnt,m_iTraCnt,m_iQueCnt,m_iOrdCnt,m_iD31Cnt,m_iTotalCnt,iBizCode);
}
void CallBackBase::handle_read(const BSYS::error_code& err, size_t bytes_transferred)
{
	if (err){
		handle_error(err);
		return;
	}
	
	m_rbuf.commit(bytes_transferred);
	stringstream ss;

	if (m_readHeader){
	
		if (m_rbuf.size() >= 2){
	
			ss << &m_rbuf;
			std::string strHeader = ss.str();

			strHeader.resize(2);
			m_lenBody = ntohs(*(u_short*) (strHeader.data()));
			m_rbuf.consume(2);
			m_readHeader = false;

			async_read(*m_socket, m_rbuf, BIO::transfer_exactly(m_lenBody), 
				boost::bind(&CallBackBase::handle_read, this,
				BIO::placeholders::error, 
				BIO::placeholders::bytes_transferred));

		}
	}
	else{
		if (m_rbuf.size() >= m_lenBody){

			ss << &m_rbuf;
			string msg = ss.str();
			msg.resize(m_lenBody);
			
			Recv(msg);
			
			m_rbuf.consume(m_lenBody);
			m_readHeader = true;

			async_read(*m_socket, m_rbuf, BIO::transfer_exactly(2), 
				boost::bind(&CallBackBase::handle_read, this, 
				BIO::placeholders::error, 
				BIO::placeholders::bytes_transferred));
				
		}	
	}
}
void CallBackBase::handle_error(const BSYS::error_code& err)
{
	char sHostTime[15],sMSec[4];
	
	GetHostTimeX(sHostTime,sMSec);

	printf("ERROR  msg %s:%s --%s\n",sHostTime,sMSec,err.message().c_str());
}
void CallBackBase::handle_write(const BSYS::error_code& err, size_t byte_transferred)
{
	m_que_wbuf.pop();

	if (err){
		printf("transferred %ld bytes\n",byte_transferred);
		handle_error(err);
	}
}
void CallBackBase::handle_connect(bool c)
{
	m_connect=true;
}
void CallBackBase::Login()
{
	LoginRequest req;

	req.set_name(m_username);
	req.set_password(m_password);

	AsyncSend(req, BizCode::LOGIN_REQ);
	
	//如果没有成功返回登录成功则等待
	while(m_loginSuccess==false){
		usleep(100*000);
	}
}

void CallBackBase::AsyncSend(const PB::Message &msg, BizCode bizCode)
{
	string strTmp;

	addBizcode(strTmp, msg, bizCode);

	m_que_wbuf.push(strTmp);

	async_write(*m_socket, BIO::buffer(strTmp,strTmp.size()), 
		boost::bind(&CallBackBase::handle_write, this,
		BIO::placeholders::error, 
		BIO::placeholders::bytes_transferred));
}

int CallBackBase::Connect()
{

	m_pThread = new thread(MainCliSocketRun,std::ref(m_ios));

	BSYS::error_code errcode;
	BIP::tcp::endpoint endpoint(BIP::address::from_string(m_ip),m_port);

	m_socket->connect(endpoint,errcode);
	if (errcode){
		m_socket->close();
		return false;
	}

	m_readHeader = true;
	
	async_read(*m_socket,m_rbuf,BIO::transfer_exactly(2), 
		boost::bind(&CallBackBase::handle_read,this,
		BIO::placeholders::error,
		BIO::placeholders::bytes_transferred));

	return true;
}
void CallBackBase::SubscribeAll()
{
	SubscribeRequest req;

	req.add_sub(proto_sub_LV2);
	req.add_sub(proto_sub_DEAL);
	req.add_sub(proto_sub_ORDERQUEUE);
	req.add_sub(proto_sub_SZ_ORDER);
	req.add_sub(proto_sub_D31);

	AsyncSend(req, SUBSCRIBLE);
}

void CallBackBase::SubscribeNo()
{
	SubscribeRequest req;

	AsyncSend(req, SUBSCRIBLE);
}
