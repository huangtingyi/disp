//////////////////////////////////////////////////////////////////////////
/// @brief    ����API
///           ʵ�ֲ�ͬ��ʽ�Ķ��ĺ�ȡ�����ļ����ղ�ѯ
//////////////////////////////////////////////////////////////////////////
#include "GTAQTSInterfaceBase.h"
#include "CallBackBase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/property_tree/json_parser.hpp>
#include "IoService.h"
#include "TinyThread.h"
#ifdef _WIN32
#include <windows.h>
#define  SLEEP(t)  Sleep((t)*1000)
#else
#include <unistd.h>
#define  SLEEP(t)  sleep(t)
#endif
//#include "../VecStockCode/VectorStockCodeSH.h"
//#include "../VecStockCode/VectorStockCodeSZ.h"

#include "callsupp.h"

#include "gta_supp.h"

int iDebugFlag=0,iWriteFlag=0;
char sCfgJsonName[1024],sDispName[1024],sPrivilegeName[1024],sWorkRoot[1024];

void signalProcess(int signal)
{
	if (signal == SIGUSR1){
		printf("signal SIGUSR1...\n");
		return;
	}
	exit(0);
}

int main(int argc, char *argv[])
{

	strcpy(sCfgJsonName,	"./gta_ints.json");
	strcpy(sDispName,	"./disp.json");
	strcpy(sPrivilegeName,	"./user_privilege.json");
	strcpy(sWorkRoot,	"/stock/work");

	for (int c; (c = getopt(argc, argv, "d:c:r:u:o:w:?:")) != EOF;){

		switch (c){
		case 'd':
			iDebugFlag = atoi(optarg);
			break;
		case 'c':
			strcpy(sCfgJsonName, optarg);
			break;
		case 'r':
			strcpy(sDispName, optarg);
			break;
		case 'u':
			strcpy(sPrivilegeName, optarg);
			break;
		case 'o':
			strcpy(sWorkRoot, optarg);
			break;
		case 'w':
			iWriteFlag=atoi(optarg);
			if(iWriteFlag!=1&&iWriteFlag!=2&&iWriteFlag!=3) iWriteFlag=0;
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-c cfg-name ]\n");
			printf("   [-r disp-name ]\n");
			printf("   [-u user-privilege-name ]\n");
			printf("   [-o work-root-name ]\n");
			printf("   [-d DebugFlag ]\n");
			printf("   [-w (1,writegta,2 writetdf,3,write gta&tdf, other nowrite) ]\n");
			exit(1);
			break;
		}
	}

	signal(SIGINT, signalProcess);

	//��ʼ��ˢ�����飬�Լ�ˢ���ļ�����ȫ�ֱ���
	InitUserArray(sDispName,&R);

	//ˢ��һ�²���������Ҫ��disp����������������������
	RefreshUserArray(sDispName,&R);

	uint16_t port;
	string host,id,passwd,strWork;
	boost::property_tree::ptree tRoot,t;

	strWork=	string(sWorkRoot);
	try{
		boost::property_tree::read_json(sCfgJsonName,tRoot);
		t 	= tRoot.get_child("gta_server");
		id 	= tRoot.get<string>("id");
		passwd 	= tRoot.get<string>("passwd");
	}
	catch (...) {
		printf("�ļ� %s �����ڻ��ʽ�Ƿ�.\n",sCfgJsonName);
		exit(1);
	}

	IoService	ios;
	//������Ϣ�ص���
	CallBackBase CallbackBase(iWriteFlag,(char*)"",strWork);
	CallbackBase.SetIoService(&ios);

	//�����������ݷ���
	ios.Start();

	//��������API����
	//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.2.1.1 ����ʵ��CreateInstance �½�
	IGTAQTSApiBase* pApiBase = IGTAQTSApiBase::CreateInstance(CallbackBase);
	// ע��FENS��ַ,�����ע��������ݸ߿��÷��棬�����ۿۡ�
	for (auto it = t.begin(); it != t.end(); ++it) {
		try {
			host = it->second.get<string>("host");
			port = it->second.get<uint16_t>("port");

			pApiBase->RegisterService(host.c_str(), port);
		}
		catch (...) {
			break;
		}
	}
	//���ó�ʱʱ��
	pApiBase->SetTimeout(30);

	do{
		//ͨ���û������������������½
		//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.2.1.5 �û���֤Login �½�
		RetCode  ret = pApiBase->LoginX(id.c_str(), passwd.c_str(), "NetType=0");
		if (Ret_Success != ret){
			printf("Login error:%d\n", ret);
			break;
		}

		//****************************** ��ȡ֤ȯ�����б���Ȩ���б�****************************************
		CDataBuffer<StockSymbol> StockList1;

		// ��ȡ�Ͻ�������������б�������SSE��ʾ�Ͻ�����SZSE��ʾ�����CFFEX��ʾ�н���
		//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.2.1.11 ��ȡ�����б�GetStockList �½�
		ret = pApiBase->GetStockList((char*)"sse,szse", StockList1);
		if (Ret_Success != ret){
			printf("GetStockList(sse,szse) error:%d\n", ret);
			break;
		}

		StockSymbol* pStock = StockList1;
		int sz = StockList1.Size();
		char sShStr[40960],sSzStr[40960];
		
		GetStockStrAll(pStock,sz,sShStr,sSzStr);


/*		VectorStockCodeSH vSH;
		VectorStockCodeSZ vSZ;

		for (int i = 0; i < sz; ++i) {
			if (!(vSH.push(pStock[i].Symbol))) {
				vSZ.push(pStock[i].Symbol);
			}
		}
		printf("\n");
		
*/
		CDataBuffer<MsgType> DataTypeList;
		// ��ȡȨ���б�
		//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.1.1.7 ��ȡȨ���б�GetDataTypeList �½�
		ret = pApiBase->GetDataTypeList(DataTypeList);
		if (Ret_Success != ret){
			printf("GetDataTypeList(sse) error:%d\n", ret);
			break;
		}

		MsgType* pMsg = DataTypeList;
		int Count = DataTypeList.Size();
		printf("MsgType Count = %d, List:", Count);
		for (int i = 0; i < Count; ++i)
		{
			printf("Ox%08x, ", pMsg[i]);
		}
		printf("\n");

		//************************************������������***********************************************

		// �����붩�����ʵʱ��������
		//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.1.1.8 ����ʵʱ����Subscribe �½�
//		string strCodesSH,strCodesSZ;
//		vSH.strForSub(strCodesSH);
//		vSZ.strForSub(strCodesSZ);
		
		//��ʼ����ͣ��ͣ����
		InitLimitArray();
		//��ȡ��̬�Ϻ�����,��ȡ��ͣ��ͣ��ֵ
		CDataBuffer<SSEL2_Static> Snap_Quotation;
		//ret = pApiBase->QuerySnap_SSEL2_Static((char*)(strCodesSH.c_str()), Snap_Quotation);
		ret = pApiBase->QuerySnap_SSEL2_Static(sShStr, Snap_Quotation);
		if (Ret_Success != ret) {
			return false;
		}
		// ��ȡȫ������
		sz = Snap_Quotation.Size();
		for (int i = 0; i < sz; ++i) {
			SSEL2_Static& SS = Snap_Quotation[i];
			int iStockCode=atoi(SS.Symbol);
			
			if(iStockCode>=0&&iStockCode<MAX_STOCK_CODE){
				LIMIT[iStockCode].WarrantDownLimit=	SS.PriceDownLimit;
				LIMIT[iStockCode].WarrantUpLimit=	SS.PriceUpLimit;
			}
			//g_LimitPriceMgr.update(SS.Symbol, SS.PriceUpLimit, SS.PriceDownLimit);
		}

		//�Ϻ�L2���Ͼ���
		//ret = pApiBase->Subscribe(Msg_SSEL2_Auction, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Auction, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Auction code=%d\n",ret);
			break;
		}

		//�Ϻ�L2ʵʱ����
		//ret = pApiBase->Subscribe(Msg_SSEL2_Quotation, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Quotation, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Quotation code=%d\n",ret);
			break;
		}
		//�Ϻ�L2ʵʱ����
		//ret = pApiBase->Subscribe(Msg_SSEL2_Transaction, (char*)(strCodesSH.c_str()));
		ret = pApiBase->Subscribe(Msg_SSEL2_Transaction, sShStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SSEL2_Transaction code=%d\n",ret);
			break;
		}

		//����L2ʵʱ����
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Quotation, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Quotation, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Quotation code=%d\n",ret);
			break;
		}
		//����L2ʵʱ����
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Transaction, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Transaction, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Transaction code=%d\n",ret);
			break;
		}
		//����L2���ί��
		//ret = pApiBase->Subscribe(Msg_SZSEL2_Order, (char*)(strCodesSZ.c_str()));
		ret = pApiBase->Subscribe(Msg_SZSEL2_Order, sSzStr);
		if (Ret_Success != ret) {
			printf("Subscribe Msg_SZSEL2_Order code=%d\n",ret);
			break;
		}

	}
	while (false);

	printf("-----------------------------1.\n");

	//ѭ������disp����仯������仯��֪ͨˢ��
	WatchFileCloseWriteAndLock(sDispName);

	printf("-----------------------------2.\n");

	//�������벻�����е�����
	while(1){//���̲߳����˳�
		sleep(1);
	}

	//�˳������߳�
	ios.Stop();

	// ������ʹ��APIʱ����Ҫ���ô˽ӿ��ͷ��ڲ���Դ������������ڴ�й©������Ԥ֪����
	//�������̩��ʵʱ����ϵͳV2.X �û��ֲᡷ4.2.1.2 �ͷ�ʵ��ReleaseInstance �½�
	IGTAQTSApiBase::ReleaseInstance(pApiBase);

	return 0;
}