//////////////////////////////////////////////////////////////////////////////
/// @file       QTSDataType.h
/// @brief      QTS数据格式定义
/// @copyright  Copyright (C), 2008-2014, GTA Information Tech. Co., Ltd.
/// @version    2.0
/// @date       2014.7.18
//////////////////////////////////////////////////////////////////////////////
#ifndef GTA_QTS_DATA_TYPE_H_
#define GTA_QTS_DATA_TYPE_H_

// 导出类宏定义
#ifdef _WIN32
#define _CDECL         __cdecl
#ifdef GTA_QTS_API_EXPORTS
#define GTA_API_EXPORT __declspec(dllexport)
#else
#define GTA_API_EXPORT __declspec(dllimport)
#endif
#else
#define _CDECL
#define GTA_API_EXPORT
#ifndef __stdcall
#define __stdcall
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

/// 消息类型定义，值为unsigned int值
enum MsgType
{
    Msg_Unknown                 = 0x00000000,       ///< 错误的消息类型
    Msg_SSEL1_Static            = 0x00101000,       ///< 上交所L1静态数据
    Msg_SSEL1_Quotation         = 0x00101001,       ///< 上交所L1实时行情
    Msg_SSE_IndexPress          = 0x0010100D,       ///< 上交所指数通行情
    Msg_SSEL2_Static            = 0x00102000,       ///< 上交所L2静态数据 
    Msg_SSEL2_Quotation         = 0x00102001,       ///< 上交所L2实时行情 UA3202
    Msg_SSEL2_Transaction       = 0x00102002,       ///< 上交所L2逐笔成交 UA3201
    Msg_SSEL2_Index             = 0x00102003,       ///< 上交所L2指数行情 UA3113
    Msg_SSEL2_Auction           = 0x00102004,       ///< 上交所L2虚拟集合竞价 UA3107
    Msg_SSEL2_Overview          = 0x00102005,       ///< 上交所L2市场总览 UA3115
    Msg_SSEIOL1_Static          = 0x00103000,       ///< 上交所个股期权静态数据
    Msg_SSEIOL1_Quotation       = 0x00103001,       ///< 上交所个股期权实时行情 UA9002
    Msg_SZSEL1_Static           = 0x00201000,       ///< 深交所L1静态数据
    Msg_SZSEL1_Quotation        = 0x00201001,       ///< 深交所L1实时行情
    Msg_SZSEL1_Bulletin         = 0x0020100C,       ///< 深交所L1公告
    Msg_SZSEL2_Static           = 0x00202000,       ///< 深交所L2静态数据 UA101
    Msg_SZSEL2_Quotation        = 0x00202001,       ///< 深交所L2实时行情 UA103
    Msg_SZSEL2_Transaction      = 0x00202002,       ///< 深交所L2逐笔成交 UA202
    Msg_SZSEL2_Index            = 0x00202003,       ///< 深交所L2指数行情 UA104
    Msg_SZSEL2_Order            = 0x00202006,       ///< 深交所L2逐笔委托 UA201
    Msg_SZSEL2_Status           = 0x00202007,       ///< 深交所L2证券状态 UA102
    Msg_CFFEXL2_Static          = 0x00302000,       ///< 中金所L2静态数据
    Msg_CFFEXL2_Quotation       = 0x00302001,       ///< 中金所L2实时行情
    Msg_SHFEL1_Static           = 0x00401000,       ///< 上期所静态数据
    Msg_SHFEL1_Quotation        = 0x00401001,       ///< 上期所实时行情
    Msg_CZCEL1_Static           = 0x00501000,       ///< 郑商所静态数据
    Msg_CZCEL1_Quotation        = 0x00501001,       ///< 郑商所实时行情
    Msg_ESUNNY_Index            = 0x00501003,       ///< 易盛指数行情
    Msg_DCEL1_Static            = 0x00601000,       ///< 大商所L1静态数据
    Msg_DCEL1_Quotation         = 0x00601001,       ///< 大商所L1深度行情
    Msg_DCEL1_ArbiQuotation     = 0x00601008,       ///< 大商所L1套利深度行情
    Msg_DCEL2_Static            = 0x00602000,       ///< 大商所L2静态数据
    Msg_DCEL2_Quotation         = 0x00602001,       ///< 大商所L2最优深度行情
    Msg_DCEL2_ArbiQuotation     = 0x00602008,       ///< 大商所L2最优套利深度行情
    Msg_DCEL2_RealTimePrice     = 0x00602009,       ///< 大商所L2实时结算价
    Msg_DCEL2_OrderStatistic    = 0x0060200A,       ///< 大商所L2委托统计行情
    Msg_DCEL2_MarchPriceQty     = 0x0060200B,       ///< 大商所L2分价成交量行情
    Msg_HKEXL2_Static           = 0x00702000,       ///< 港交所L2静态数据
    Msg_HKEXL2_Quotation        = 0x00702001,       ///< 港交所L2实时行情
    Msg_HKEXL2_Index            = 0x00702003,       ///< 港交所L2指数行情
    Msg_HKEXL2_Overview         = 0x00702005,       ///< 港交所L2市场总览
    Msg_HKEXL2_BrokerQueue      = 0x0070200E,       ///< 港交所L2经纪人队列
};
/// 数据分类，多个市场汇总后的分类
enum DataCategory
{
    Category_Unknown            =   -1,             ///< 错误消息分类
    Category_Static             = 0x00,             ///< 静态数据
    Category_Quotation          = 0x01,             ///< 实时行情
    Category_Transaction        = 0x02,             ///< 逐笔成交
    Category_Index              = 0x03,             ///< 指数行情
    Category_Auction            = 0x04,             ///< 虚拟集合竞价
    Category_Overview           = 0x05,             ///< 市场总览
    Category_Order              = 0x06,             ///< 逐笔委托
    Category_Status             = 0x07,             ///< 证券状态
    Category_ArbiQuotation      = 0x08,             ///< 套利深度行情
    Category_RealTimePrice      = 0x09,             ///< 实时结算价
    Category_OrderStatistic     = 0x0A,             ///< 委托统计行情
    Category_MarchPriceQty      = 0x0B,             ///< 分价成交量行情
};
/// 市场类型定义
enum MarketTag
{
    Market_Unknown         = 0,     ///< 错误市场
    Market_SSE             = 1,     ///< 上交所
    Market_SZSE            = 2,     ///< 深交所
    Market_CFFEX           = 3,     ///< 中金所
    Market_SHFE            = 4,     ///< 上期所
    Market_CZCE            = 5,     ///< 郑商所
    Market_DCE             = 6,     ///< 大商所
    Market_HKEX            = 7,     ///< 港交所
};
/// 网络状态定义.
enum ConnectState
{
    Connecting                  = 0,    ///< 正在连接
    Connected                   = 1,    ///< 已连接
    ConnectClosing              = 2,    ///< 正在关闭
    ConnectClosed               = 3,    ///< 已关闭
};
/// 返回值含义列表
enum RetCode
{
    Ret_Error                   = -1,   ///< 失败
    Ret_Success                 = 0,    ///< 成功
    Ret_NoAddress               = 1,    ///< 请先设置行情服务器地址
    Ret_NoPermission            = 2,    ///< 没有权限，请联系客服
    Ret_ParamInvalid            = 3,    ///< 参数无效
    Ret_AccountError            = 4,    ///< 帐号或密码错误
    Ret_AccountOutDate          = 5,    ///< 帐号不在有效期内
    Ret_ConnectFail             = 6,    ///< 连接失败
    Ret_LoginRepeat             = 7,    ///< 帐号重复登录
    Ret_OutTime                 = 8,    ///< 超时
    Ret_CloseConnect            = 9,    ///< 连接断开
    Ret_OutLimit                = 10,   ///< 代码订阅数超出上限
    Ret_LowVersion              = 11,   ///< 版本过低
};

/// 字段ID定义
enum FieldID
{
    FID_ERROR                   = 0,            ///< 错误字段定义，字段已到结尾
    FID_AuctionOpenPrice        = 1007,         ///< 虚拟开盘参考价. 
    FID_AuctionVolume           = 1009,         ///< 虚拟匹配量. 
    FID_BondWtAvgBuyPrice       = 1013,         ///< 债券加权平均委买价. （元）
    FID_BondWtAvgSellPrice      = 1014,         ///< 债券加权平均委卖价. 
    FID_BuyLevelNo              = 1015,         ///< 买盘价位数. "沪深L2实时行情：10档行情，不足时补空；上交所个股期权实时行情：五档:总是5，空档位用0填充。"
    FID_BuyLevelQueue           = 1016,         ///< 买1档队列. 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
    FID_BuyOrderID              = 1017,         ///< 买方委托索引. 从 1 开始计数，0 表示无对应委托
    FID_BuyOrderNo              = 1018,         ///< 买方委托价位数. 
    FID_BuyPrice01              = 1019,         ///< 申买价. 3 位小数
    FID_BuyPrice02              = 1020,         ///< 申买价. 3 位小数
    FID_BuyPrice03              = 1021,         ///< 申买价. 3 位小数
    FID_BuyPrice04              = 1022,         ///< 申买价. 3 位小数
    FID_BuyPrice05              = 1023,         ///< 申买价. 3 位小数
    FID_BuyPrice06              = 1024,         ///< 申买价. 3 位小数
    FID_BuyPrice07              = 1025,         ///< 申买价. 3 位小数
    FID_BuyPrice08              = 1026,         ///< 申买价. 3 位小数
    FID_BuyPrice09              = 1027,         ///< 申买价. 3 位小数
    FID_BuyPrice10              = 1028,         ///< 申买价. 3 位小数
    FID_BuyRecID                = 1029,         ///< 买方订单号. 
    FID_BuySellFlag             = 1030,         ///< 内外盘标志. B – 外盘,主动买;S – 内盘,主动卖;N – 未知
    FID_BuyVolume01             = 1031,         ///< 申买量. 
    FID_BuyVolume02             = 1032,         ///< 申买量. 
    FID_BuyVolume03             = 1033,         ///< 申买量. 
    FID_BuyVolume04             = 1034,         ///< 申买量. 
    FID_BuyVolume05             = 1035,         ///< 申买量. 
    FID_BuyVolume06             = 1036,         ///< 申买量. 
    FID_BuyVolume07             = 1037,         ///< 申买量. 
    FID_BuyVolume08             = 1038,         ///< 申买量. 
    FID_BuyVolume09             = 1039,         ///< 申买量. 
    FID_BuyVolume10             = 1040,         ///< 申买量. 
    FID_BuyVolumeUnit           = 1041,         ///< 买数量单位. 买订单的申报数量必须是该字段的整数倍。
    FID_CallOrPut               = 1042,         ///< 认购认沽. C = Call，P = Put
    FID_CFICode                 = 1043,         ///< 证券类别. "上交所：’E'表示股票；‘EU’表示基金；‘D’表示债券；‘RWS’表示权证；‘FF’表示期货。
                                                ///< 深交所：CFICode和SecuritySubType两个字段确定了不同的证券类别，CFICode的取值为MRI，EU，ES，D，DC。"
    FID_OutstandingShare        = 1044,         ///< 流通股数. 
    FID_ClosePrice              = 1046,         ///< 今收盘价. 
    FID_ContractID              = 1047,         ///< 合约交易代码 . 合约交易代码17位，按以下顺序编写：1、第1至第6位为数字，取标的证券代码，如工商银行601398，取“601398”；2、第7位为C(Call)或者P(Put)，分别表示认购期权或者认沽期权；3、第8、9位表示到期年份；4、第10、11位表示到期月份；5、第12位期初设为“M”，表示月份合约。当合约首次调整后，“M”修改为 “A”，以表示该合约被调整过一次，如发生第二次调整，则“A”修改为“B”、“M”修改为 “A”，以此类推；6、第13至17位表示期权行权价格.留两位备用
    FID_ContractMultiplier      = 1048,         ///< 债券折合回购标准券比例. 2 位小数
    FID_ContractMultiplierUnit  = 1049,         ///< 合约单位 . 经过除权除息调整后的合约单位, 一定是整数 
    FID_ConversionBeginDate     = 1051,         ///< 行权开始日. 
    FID_ConversionEndDate       = 1052,         ///< 行权结束日. 
    FID_ConversionPrice         = 1053,         ///< 行权价格. 3 位小数
    FID_ConversionRatio         = 1054,         ///< 行权比例. 4 位小数
    FID_CouponRate              = 1055,         ///< 票面利率. 6 位小数
    FID_CrdBuyStatus            = 1056,         ///< 当前融资开放状态. Y = 开放,N = 暂停
    FID_CrdBuyUnderlying        = 1057,         ///< 融资标的标志. "上交所：’T‘表示是融资标的证券；’F‘表示不是融资标的证券。
                                                ///< 深交所：Y= 是，N= 否"
    FID_CrdPriceCheckType       = 1058,         ///< 提价检查方式. 0=不检查，1=不低于最近成交价，2=不低于昨收价，3=不低于最高叫买，4=不低于最低叫卖
    FID_CrdSellStatus           = 1059,         ///< 当前融券开放状态. Y = 开放,N = 暂停
    FID_CrdSellUnderlying       = 1060,         ///< 融券标的标志. "上交所：’T‘表示是融券标的证券；’F‘表示不是融券标的证券。
                                                ///< 深交所：Y= 是，N= 否"
    FID_Currency                = 1061,         ///< 币种. HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币 空值：配合“成交金额”使用。币
    FID_DeliveryDate            = 1062,         ///< 行权交割日. 行权交割日，默认为行权日的下一个交易日，YYYYMMDD
    FID_EndDate                 = 1063,         ///< 最后交易日 . 期权最后交易日/行权日，YYYYMMDD 
    FID_ETFBuyAmount            = 1064,         ///< ETF 申购金额. 
    FID_ETFBuyNo                = 1065,         ///< ETF 申购笔数. 
    FID_ETFBuyVolume            = 1066,         ///< ETF 申购数量. 
    FID_ETFSellAmount           = 1067,         ///< ETF 赎回金额. 
    FID_ETFSellNo               = 1068,         ///< ETF 赎回笔数. 
    FID_ETFSellVolume           = 1069,         ///< ETF 赎回数量. 
    FID_ExerciseDate            = 1070,         ///< 期权行权日 . 期权行权日，YYYYMMDD 
    FID_ExercisePrice           = 1071,         ///< 期权行权价 . 经过除权除息调整后的期权行权价，右对齐，精确到厘;行权价格调整采取四舍五入方式，股票保留两位小数，ETF保留三位小数。
    FID_ExpireDate              = 1072,         ///< 期权到期日 . 期权到期日，YYYYMMDD 
    FID_GageRate                = 1073,         ///< 担保品折算率. 2 位小数
    FID_HighPrice               = 1074,         ///< 最高价. 4位小数
    FID_IndustryType            = 1075,         ///< 行业种类. 
    FID_InterestAccrualDate     = 1076,         ///< 起息日. 
    FID_InterruptAuctionPrice   = 1077,         ///< 波动性中断参考价. 4 decimal places
    FID_InterruptAuctionVolume  = 1078,         ///< 波动性中断虚拟匹配量. 虚拟集合竞价过程中，若期权期货的价格波动性太大，则需要中断一段时间，中断结束后，继续使用这一数据作为虚拟匹配量
    FID_IOPV                    = 1079,         ///< ETF 净值估值. 
    FID_ISINCode                = 1080,         ///< ISIN代码. 
    FID_IssuedVolume            = 1082,         ///< 总发行量. 
    FID_LastPrice               = 1083,         ///< 现价. 4位小数
    FID_LeaveVolume             = 1084,         ///< 虚拟未匹配量. 
    FID_ListingDate             = 1085,         ///< 上市日期. 在上交所首日交易日期，YYYYMMDD
    FID_LmtOrdCeiling           = 1086,         ///< 限价申报上限 . 单笔限价申报的申报张数上限。 
    FID_LmtOrdFloor             = 1087,         ///< 限价申报下限 . 单笔限价申报的申报张数下限。 
    FID_LowPrice                = 1088,         ///< 最低价. 4位小数
    FID_MarginRatioParam1       = 1089,         ///< 保证金计算参数一 . 保证金计算参数，单位：% 
    FID_MarginRatioParam2       = 1090,         ///< 保证金计算参数二 . 保证金计算参数，单位：% 
    FID_MarginUnit              = 1091,         ///< 单位保证金 . 当日持有一张合约所需要的保证金数 量，精确到分 
    FID_MarketMakerFlag         = 1095,         ///< 做市商标志. Y = 有做市商，N = 无做市商
    FID_MarketTime              = 1099,         ///< 市场时间. 百分之一秒
    FID_MarketType              = 1100,         ///< 市场种类. ‘ASHR’表示A股市场；‘BSHR’表示B股市场；‘CSHR‘表示国际版市场。
    FID_MaturityDate            = 1101,         ///< 到期交割日. 
    FID_MaxBuyDuration          = 1102,         ///< 买入成交最大等待时间. 
    FID_DeclareVolumeCeiling    = 1103,         ///< 申报量上限. 申报数量上限
    FID_MaxSellDuration         = 1104,         ///< 卖出成交最大等待时间. 
    FID_MessageID               = 1105,         ///< 消息序号. 
    FID_DeclareVolumeFloor      = 1106,         ///< 申报量下限. 申报数量下限
    FID_MktOrdCeiling           = 1111,         ///< 市价申报上限 . 单笔市价申报的申报张数上限。 
    FID_MktOrdFloor             = 1112,         ///< 市价申报下限 . 单笔市价申报的申报张数下限。 
    FID_NameUnderlying          = 1113,         ///< 基础证券名称 . 
    FID_NAV                     = 1114,         ///< 基金T-1日累计净值. 4 位小数
    FID_NonTradableNo           = 1116,         ///< 非流通股数. 预留
    FID_OfferingFlag            = 1117,         ///< 股票：要约收购;债券、优先股：转股回售;Y=是,N=否,空值表示无意义
    FID_OpenPrice               = 1118,         ///< 开盘价. 4位小数
    FID_OptionType              = 1119,         ///< 行权类型. A= 美式，E = 欧式，B = 百慕大式
    FID_LocalTimeStamp          = 1120,         ///< 采集时间. 国泰安内部加的采集时间，用于测延时，精确到毫秒
    FID_OrderCode               = 1121,         ///< 委托买卖方向。买卖方向 ：1=买 2=卖 G=借入 F=出借 
    FID_OrderPrice              = 1122,         ///< 委托价格. 3位小数
    FID_OrderType               = 1123,         ///< 订单类别.1=市价 2=限价 U=本方最优
    FID_OrderVolume             = 1124,         ///< 委托数量. 
    FID_PacketTimeStamp         = 1125,         ///< 包头时间. 国泰安内部加的包头时间，用于测延时，精确到毫秒
    FID_ParValue                = 1128,         ///< 每股面值. 3 位小数
    FID_PERatio1                = 1129,         ///< 市盈率 1.
    FID_PERatio2                = 1130,         ///< 市盈率 2.
    FID_PreClosePrice           = 1131,         ///< 昨收价.
    FID_PreClosePriceUnderlying = 1132,         ///< 标的昨收价 . 期权标的证券除权除息调整后的前收盘价格，右对齐，单位：元 
    FID_PreSettlePrice          = 1133,         ///< 昨结算. 昨日结算价，右对齐，单位：元 ；4 decimal places
    FID_PreYearEPS              = 1134,         ///< 上年每股利润. 4 位小数
    FID_PriceDownLimit          = 1135,         ///< 跌幅价格. 3 位小数
    FID_TickSize                = 1136,         ///< 最小报价单位. "申报价格的最小变动单位
                                                ///< 上交所个股期权：单位元，精确到0.1厘"
    FID_UpDownLimitType         = 1138,         ///< 涨跌幅限制类型. ‘N’表示交易规则3.4.13规定的有涨跌幅限制类型或者权证管理办法第22条规定；‘R'表示交易规则3.4.15和3.4.16规定的无涨跌幅限制类型；’S‘表示回购涨跌幅控制类型。
    FID_PriceUpLimit            = 1139,         ///< 涨幅价格. 对于N类型涨跌幅限制的产品，该字段当日不会更改，基于前收盘价（已首日上市交易产品为发行价）计算；对于R类型五涨跌幅限制的产品，该字段取开盘时基于参考价格计算的上限价格。3 位小数
    FID_RecID                   = 1140,         ///< 消息记录号.从 1 开始计数，同一频道连续
    FID_RedemptionStatus        = 1141,         ///< 赎回标志.适用于 ETF,LOF 等开放式基金，对于黄金 ETF 是指现金赎回开关。Y= 是,N= 否，空格无意义
    FID_RoundLot                = 1142,         ///< 整手数 . 交易单位（一手等于几张合约）
    FID_SecurityEN              = 1143,         ///< 英文证券名称. 
    FID_SecurityName            = 1145,         ///< 证券名称. 
    FID_SecurityPhaseTag        = 1147,         ///< 交易状态.
    FID_SecurityStatus          = 1148,         ///< 产品状态标识. 第0位对应：‘N’表示首日上市；第1位对应：‘D’表示除权；第2位对应：‘R'表示除息；第3位对应：‘D‘表示国内主板正常交易产品，‘S’表示风险警示产品，‘P’表示退市整理产品。
    FID_SecurityStatusFlag      = 1149,         ///< 期权合约状态. 该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。第1位：‘0’表示可开仓，‘1’表示限制卖出开仓（包括备兑开仓）和买入开仓。第2位：‘0’表示未连续停牌，‘1’表示连续停牌。第3位：‘0’表示未临近到期日，‘1’表示距离到期日不足10个交易日。第4位：‘0’表示近期未做调整，‘1’表示最近10个交易日内合约发生过调整。第5位：‘A’表示当日新挂牌的合约，‘E’表示存续的合约，‘D’表示当日摘牌的合约。
    FID_SecuritySubType         = 1150,         ///< 证券子类别. "上交所：自定义详细证券类别
                                                ///< 深交所：SecuritySubType和CFICode确定了不同的证券类别，取值范围为：N/A（空值发布），A，B,AR，BR，G，O，R，AN，AS。"
    FID_SellLevelNo             = 1153,         ///< 卖盘价位数. "上交所个股期权实时行情：五档:总是5，空档位用0填充；
                                                ///< 深交所L2实时行情、上交所L2实时行情：10档行情，不足时补空"
    FID_SellLevelQueue          = 1154,         ///< 卖1档队列. 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
    FID_SellOrderID             = 1155,         ///< 卖方委托索引. 从 1 开始计数，0 表示无对应委托
    FID_SellOrderNo             = 1156,         ///< 卖方委托价位数. 
    FID_SellPrice01             = 1157,         ///< 申卖价. 3 位小数
    FID_SellPrice02             = 1158,         ///< 申卖价. 3 位小数
    FID_SellPrice03             = 1159,         ///< 申卖价. 3 位小数
    FID_SellPrice04             = 1160,         ///< 申卖价. 3 位小数
    FID_SellPrice05             = 1161,         ///< 申卖价. 3 位小数
    FID_SellPrice06             = 1162,         ///< 申卖价. 3 位小数
    FID_SellPrice07             = 1163,         ///< 申卖价. 3 位小数
    FID_SellPrice08             = 1164,         ///< 申卖价. 3 位小数
    FID_SellPrice09             = 1165,         ///< 申卖价. 3 位小数
    FID_SellPrice10             = 1166,         ///< 申卖价. 3 位小数
    FID_SellRecID               = 1167,         ///< 卖方订单号. 
    FID_SellVolume01            = 1168,         ///< 申卖量. 
    FID_SellVolume02            = 1169,         ///< 申卖量. 
    FID_SellVolume03            = 1170,         ///< 申卖量. 
    FID_SellVolume04            = 1171,         ///< 申卖量. 
    FID_SellVolume05            = 1172,         ///< 申卖量. 
    FID_SellVolume06            = 1173,         ///< 申卖量. 
    FID_SellVolume07            = 1174,         ///< 申卖量. 
    FID_SellVolume08            = 1175,         ///< 申卖量. 
    FID_SellVolume09            = 1176,         ///< 申卖量. 
    FID_SellVolume10            = 1177,         ///< 申卖量. 
    FID_SellVolumeUnit          = 1178,         ///< 卖数量单位. 卖数量单位
    FID_SetID                   = 1179,         ///< 频道代码. 
    FID_SetNo                   = 1180,         ///< 产品集编号. 取值范围从1到999,。用来表明产品的一种分组方式，用于在多主机间记性负载均衡分配。该值在一个交易日内不会变化。
    FID_SettlePrice             = 1181,         ///< 结算价. 4 decimal places
    FID_Side                    = 1183,         ///< 买卖方向. 0=无未匹配量，买卖两边的未匹配量都为 0;1=买方有未匹配量，卖方未匹配量=0;2=卖方有未匹配量，买方未匹配量=0
    FID_StartDate               = 1184,         ///< 首个交易日 . 期权首个交易日,YYYYMMDD (年*10000+月*100+日)
    FID_StatusID                = 1185,          ///< 证券交易状态，0：此字段无意义，忽略此值  2：交易停牌  3：复牌  此字段盘中有可能会下发。
    FID_SubscribeStatus         = 1186,         ///< 认购标志.适用于网上发行认购。Y= 是,N= 否，空格无意义
    FID_Symbol                  = 1187,         ///< 证券代码. 
    FID_SymbolSource            = 1188,         ///< 证券代码源. 102=深圳证券交易所，103=香港交易所
    FID_SymbolUnderlying        = 1189,         ///< 基础证券代码. 
    FID_TradeAmount             = 1190,         ///< 成交金额. 成交金额
    FID_Time                    = 1191,         ///< 数据生成时间(毫秒)，不适用于TradeCancelFlag=Y
    FID_TotalAmount             = 1192,         ///< 成交总额. 元
    FID_TotalBuyNo              = 1193,         ///< 买入总笔数. 
    FID_TotalBuyOrderVolume     = 1195,         ///< 委托买入总量. 股票：股;权证：份;债券：手
    FID_TotalLongPosition       = 1196,         ///< 合约未平仓数 . 单位是 （张） 
    FID_TotalNo                 = 1197,         ///< 成交笔数. 
    FID_TotalPosition           = 1198,         ///< 持仓量. 
    FID_TotalSellNo             = 1199,         ///< 卖出总笔数. 
    FID_TotalSellOrderVolume    = 1201,         ///< 委托卖出总量. 
    FID_TotalVolume             = 1202,         ///< 成交总量. "上交所L2实时行情：股票：股;权证：份;债券：手；
                                                ///< 上交所个股期权：Trade volume of this security.如果期权合约的产品代码为“00000000”，则表示行情日期，格式为YYYYMMDD"
    FID_TotalWarrantExecVol     = 1203,         ///< 权证执行总量. 
    FID_TradableMarketNo        = 1204,         ///< 可流通已上市数量. 预留
    FID_TradableNo              = 1205,         ///< 可流通未上市数量. 预留
    FID_TradeChannel            = 1206,         ///< 成交通道. 
    FID_TradeCode               = 1207,         ///< 成交代码. 
    FID_TradeDate               = 1208,         ///< 市场日期. 
    FID_TradePrice              = 1210,         ///< 成交价格. 3 位小数
                                                ///< 0 = 开市前,
                                                ///< 1 = 开盘集合竞价,
                                                ///< 2 = 连续竞价阶段,
                                                ///< 3 = 盘中临时停市,
                                                ///< 4 = 收盘集合竞价,
                                                ///< 5 = 集中竞价闭市,
                                                ///< 6 = 协议转让结束,
                                                ///< 7 = 闭市
    FID_TradeStatus             = 1212,         ///< 当前品种交易状态. 
    FID_TradeTime               = 1213,         ///< 成交时间. 143025006 表示14:30:25.006
    FID_TradeType               = 1214,         ///< 成交类型.4=撤销，主动或自动撤单执行报告；F=成交，成交执行报告. 
                                                ///< 下面针对港股：公共成交类型
                                                ///< 0：自动对盘的非双边客成交 (AMS<space>)
                                                ///< 4：开市前成交(Off-exchange previous day) (AMS “P”)
                                                ///< 22：非自动对盘或者特别买卖单位的非双边客成交(AMS “M”)
                                                ///< 100：自动对盘的双边客成交(AMS“Y”)
                                                ///< 101：非自动对盘或者特别买卖单位的双边客成交(AMS “X”)
                                                ///< 102：碎股成交(AMS “D”)
                                                ///< 103：竞价成交 (AMS “U”)
                                                ///< 104：海外交易
                                                ///< 当TradeCancelFlag = Y时不适用
    FID_TradeVolume             = 1215,         ///< 成交数量. 股票：股;权证：份;债券：张，如果TradeCancelFlag=Y，表示未成交的剩余量
    FID_UnderlyingType          = 1217,         ///< 标的证券类型 . EBS – ETF， ASH – A股 
    FID_Version                 = 1218,         ///< 合约版本号 . 期权合约的版本号。新挂合约是’1’ 
    FID_WarrantClearingType     = 1219,         ///< 权证结算方式. S = 证券结算，C = 现金结算
    FID_WarrantDownLimit        = 1221,         ///< 权证跌停价. （元）
    FID_WarrantUpLimit          = 1223,         ///< 权证涨停价. （元）
    FID_WithdrawBuyAmount       = 1224,         ///< 买入撤单额. 
    FID_WithdrawBuyNo           = 1225,         ///< 买入撤单笔数. 
    FID_WithdrawBuyVolume       = 1226,         ///< 买入撤单量. 
    FID_WithdrawSellAmount      = 1227,         ///< 卖出撤单额. 
    FID_WithdrawSellNo          = 1228,         ///< 卖出撤单笔数. 
    FID_WithdrawSellVolume      = 1229,         ///< 卖出撤单量. 
    FID_WtAvgBuyPrice           = 1230,         ///< 加权平均委买价. 3 位小数（元）
    FID_WtAvgSellPrice          = 1231,         ///< 加权平均委卖价. 3 位小数（元）
    FID_XDAmount                = 1232,         ///< 除息金额. 每股分红金额
    FID_XRRatio                 = 1233,         ///< 除权比例. 每股送股比例；对于国债预发行产品，为保证金比例。
    FID_YearEPS                 = 1235,         ///< 本年每股利润. 4 位小数
    FID_YTM                     = 1236,         ///< 债券到期收益率. 
    FID_TotalBuyOrderNo01       = 1251,         ///< 买一档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo02       = 1252,         ///< 买二档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo03       = 1253,         ///< 买三档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo04       = 1254,         ///< 买四档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo05       = 1255,         ///< 买五档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo06       = 1256,         ///< 买六档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo07       = 1257,         ///< 买七档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo08       = 1258,         ///< 买八档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo09       = 1259,         ///< 买九档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalBuyOrderNo10       = 1260,         ///< 买十档总委托笔数. 深交所L2行情快照：为 0 表示不揭示
    FID_TotalSellOrderNo01      = 1261,         ///< 卖一档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo02      = 1262,         ///< 卖二档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo03      = 1263,         ///< 卖三档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo04      = 1264,         ///< 卖四档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo05      = 1265,         ///< 卖五档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo06      = 1266,         ///< 卖六档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo07      = 1267,         ///< 卖七档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo08      = 1268,         ///< 卖八档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo09      = 1269,         ///< 卖九档总委托笔数. 为 0 表示不揭示
    FID_TotalSellOrderNo10      = 1270,         ///< 卖十档总委托笔数. 为 0 表示不揭示
    FID_SellLevelQueueNo01      = 1271,         ///< 卖一档揭示委托笔数. 为 0 表示不揭示
    FID_BuyLevelQueueNo01       = 1272,         ///< 买一档揭示委托笔数. 为 0 表示不揭示
    FID_PreTotalPosition        = 1092,         ///< 昨持仓量
    FID_PreDelta                = 1093,         ///< 昨虚实度
    FID_Delta                   = 1094,         ///< 今虚实度
    FID_SettleGroupID           = 1096,         ///< 结算组代码
    FID_SettleID                = 1097,         ///< 结算编号
    /// QTS2.2增加部分，沪深L1部分字段
    FID_PriceUpdown1            = 1282,         ///< 涨跌1. LastPrice-PreClosePrice
    FID_PriceUpdown2            = 1200,         ///< 涨跌2. LastPrice-上一笔LastPrice,对于当天的第一笔成交，该字段=LastPrice-PreClosePrice
    /// QTS2.2增加部分，上海L2静态数据部分字段
    FID_SampleNo                = 1273,         ///< 样本数量.UINT32.指数当前的样本数量
    FID_SampleAvgPrice          = 1274,         ///< 样本均价.int64.指数当前样本的均价,市价总值/发行股本,若该指标不统计则输出 N/A
    FID_AvgCapital              = 1275,         ///< 平均股本.UINT64.指数当前样本的平均股本,（算术平均，单位：亿股）,若该指标不统计则输出 N/A
    FID_TotalMarketValue        = 1276,         ///< 总市值.UINT64.指数当前样本的总市值汇总 （算术和， 单位： 万亿元）,若该指标不统计则输出 N/A
    FID_MarketValueRatio        = 1277,         ///< 占比%.int64.指数当前样本的总市值占上证综指全样本的总市值,百分比,若该指标不统计则输出 N/A
    FID_StaticPERatio           = 1278,         ///< 静态市盈率.UINT64.指数当前样本的静态市盈率。公式：合计（人民币收盘价*发行量） /合计（每股收益*发行量）,若该指标未统计则输出 N/A
    FID_IndexLevelStatus        = 1279,         ///< 指数级别标识.string(20).前 5 位为指数排序数值，最后一位即指数级别信息：,1 为重点指数;2 为全貌指数;其他可根据需要扩展。
    FID_QuotationFlag           = 1280,         ///< 行情源标志。该字段为4位字符串，每位表示特定的含义，无定义则填空格。 第1位对应：’1’表示国上海机房行情源，’2’表示深圳机房行情源
    FID_AutoSplitDate           = 1283,         ///< 垂直价差组合策略到期解除日期.该日期表示垂直价差组合策略到期解除的日期，YYYYMMDD；   对于E-2后波动加挂的合约，此字段仍为E-2日。到期日调整时，此字段信息自动同步调整。
    FID_ParseFileFlag           = 1284,         ///< 内部使用。上交所L2静态数据文件解析标志，详见SSEL2StaticFilter定义
    /// QTS2.3增加部分，深交所L2六个消息类型部分字段
    FID_ExerciseStatus          = 1343,         ///< 权证标志。适用于处于行权期的权证。1= 是,0=否，空格无意义
    FID_FinancialStatus         = 1345,         ///< 证券状态信息。A=上市公司早间披露提示，B=上市公司午间披露提示。每个字节揭示一种状态，最多可同时揭示八种状态
    FID_PurchasingStatus        = 1348,         ///< 认购标志。适用于网上发行认购代。1= 是,0= 否，空格无意义
    FID_StockDiviStatus         = 1349,         ///< 转股标志。适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    FID_PutableStatus           = 1350,         ///< 回售标志。适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    FID_AcceptedStatus          = 1351,         ///< 预受要约标志。适用于处于要约收购期的股票。1= 是,0= 否，空格无意义
    FID_ReleaseStatus           = 1352,         ///< 解除要约标志 。适用于处于要约收购期的股票。1= 是,0= 否，空格无意义
    FID_CancStockDiviStatus     = 1353,         ///< 转股撤单标志。适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    FID_CancPutableStatus       = 1354,         ///< 回售撤单标志 。适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    FID_PledgeStatus            = 1355,         ///< 质押标志。适用于质押式回购可质押入库证券。1= 是,0= 否，空格无意义
    FID_RemovePledge            = 1356,         ///< 解押标志。适用于质押式回购可质押入库证券。1= 是,0= 否，空格无意义
    FID_VoteStatus              = 1357,         ///< 表决权标志。适用于优先股。1= 是,0= 否，空格无意义
    FID_StockPledgeRepo         = 1358,         ///< 股票质押式回购标志。适用于可开展股票质押式回购业务的证券。1= 是,0= 否，空格无意义
    FID_DivideStatus            = 1359,         ///< 实时分拆标志。适用于分级基金。1= 是,0= 否，空格无意义
    FID_MergerStatus            = 1360,         ///< 实时合并标志。适用于分级基金。1= 是,0= 否，空格无意义
    FID_UnderlyingSecurityIDSource = 1361,      ///< 基础证券代码源，102 = 深圳证券交易所
    FID_SecurityType            = 1362,         ///< 证券类别代码。1主板 A股； 2中小板股票；3创业板股票；4主板 B股；5国债 （含地方债）；6企业债；7公司债；8可转债；9私募债；10可交换私募债；11证券公司次级债；12质押式回购；13资产支持证券；14本市场股票ETF；15跨市场ETF；16跨境ETF；17本市场实物债券ETF；18现金债券ETF；19黄金ETF；20货币ETF；21（预留）杠杆ETF;22(预留)商品期货ETF；23标准LOF;24分级子基金；25封闭式基金；26仅申赎基金；28权证；29个股期权；30ETF期权；33优先股；34证券公司短期债；35可交换公司债
    FID_DayTrading              = 1363,         ///< 回转交易标志。是否支持当日回转交易：Y=支持，N=不支持
    FID_SecurityStatusTag       = 1364,         ///< 证券状态标识。该字段为14位字符串，起每位表示特定的含义，无定义则填空格。第1位对应：“1”表示停牌；第2位对应：“1”表示除权；第3位对应：“1”表示除息；第4位对应：“1”表示ST;第5位对应：“1”表示*ST;第6位对应：“1”表示上市首日；第7位对应：“1”表示公司再融资；第8位对应：“1”表示恢复上市首日；第9位对应：“1”表示网络投票；第10位对应：“1”表示退市整理期；第12位对应：“1”表示增发股份上市；第13位对应：“1”表示合约调整；第14位对应：“1”表示暂停上市后协议转让。          
    FID_GageFlag                = 1365,         ///< 保证金证券标志。是否可作为融资融券可充抵保证金证券：Y=是，N=否
    FID_PledgeFlag              = 1366,         ///< 质押入库标志。是否可质押入库:Y=是，N=否
    FID_RegularShare            = 1367,         ///< 对应回购标准券。
    FID_QualificationFlag       = 1368,         ///< 投资者适当性管理标志：是否需要对该证券作投资者适当性管理：Y=是，N=否
    FID_IssuePrice              = 1369,         ///< 贴现发行价。，4位小数
    FID_Interest                = 1370,         ///< 每百元应计利息，8位小数，优先股：0.0000 表示浮动股息率
    FID_ExpirationDays          = 1371,         ///< 购回期限。
    FID_LimitUpRateO            = 1372,         ///< 开盘集合竞价上涨幅度，3位小数
    FID_LimitDownRateO          = 1373,         ///< 开盘集合竞价下跌幅度，3位小数
    FID_LimitUpAbsoluteO        = 1374,         ///< 开盘集合竞价上涨限价，4位小数
    FID_LimitDownAbsoluteO      = 1375,         ///< 开盘集合竞价下跌限价，4位小数
    FID_AuctionUpDownRateO      = 1376,         ///< 开盘集合竞价有效范围涨跌幅度，3位小数
    FID_AuctionUpDownAbsoluteO  = 1377,         ///< 开盘集合竞价有效范围涨跌价格，4位小数
    FID_LimitUpRateT            = 1378,         ///< 连续竞价上涨幅度，3位小数
    FID_LimitDownRateT          = 1379,         ///< 连续竞价下跌幅度，3位小数
    FID_LimitUpAbsoluteT        = 1394,         ///< 连续竞价上涨限价，4位小数
    FID_LimitDownAbsoluteT      = 1395,         ///< 连续竞价下跌限价，4位小数
    FID_AuctionUpDownRateT      = 1380,         ///< 连续竞价有效范围涨跌幅度，3位小数
    FID_AuctionUpDownAbsoluteT  = 1381,         ///< 连续竞价有效范围涨跌价格，4位小数
    FID_LimitUpRateC            = 1382,         ///< 收盘集合竞价上涨幅度，3位小数
    FID_LimitDownRateC          = 1383,         ///< 收盘集合竞价下跌幅度，3位小数
    FID_LimitUpAbsoluteC        = 1384,         ///< 收盘集合竞价上涨限价，4位小数
    FID_LimitDownAbsoluteC      = 1385,         ///< 收盘集合竞价下跌限价，4位小数
    FID_AuctionUpDownRateC      = 1386,         ///< 收盘集合竞价有效范围涨跌幅度，3位小数
    FID_AuctionUpDownAbsoluteC  = 1387,         ///< 收盘集合竞价有效范围涨跌价格，4位小数
    FID_BuyQtyUpperLimit        = 1388,         ///< 买数量上限。买委托数量的上限，2位小数
    FID_SellQtyUpperLimit       = 1389,         ///< 卖数量上限。卖委托数量的上限，2位小数
    FID_PremiumRate             = 1391,         ///< 权证溢价率。6位小数
    FID_GoldPurchase            = 1392,         ///< 黄金ETF实物申购标志。适用于黄金ETF实物申购。Y= 是,N= 否，空格无意义
    FID_GoldRedemption          = 1393,         ///< 黄金ETF实物赎回标志。适用于黄金ETF实物赎回。Y= 是,N= 否，空格无意义
    FID_ClearingPrice           = 1396,         ///< 结算价格。适用于权证，4位小数
    /// QTS2.5增加部分,上交所指数通
    FID_ActionDay               = 1285,         ///< 业务发生日,行情文件所代表的自然日期（北京时间），内容为被用于计算的那天的自然日期（北京时间）
    FID_UpDownRate              = 1286,         ///< 涨跌幅
    FID_ExchangeRate            = 1329,         ///< 汇率，该汇率在盘中时为0.00000000，收盘后，该汇率值为该指数收盘时计算指数所使用的汇率
    FID_DisplayNum              = 1344,         ///< 指数展示序号,展示指数的顺序按指数展示序号进行排列。
    FID_ClosePrice2             = 1346,         ///< 当日收盘值2，若该指数为全球指数，该收盘值为当日亚太区收盘值。初始值为0.0000。当值不为0.0000时，说明指数亚太区已收盘
    FID_ClosePrice3             = 1347,         ///< 当日收盘值3，若该指数为全球指数，该收盘值为当日欧洲区收盘值。初始值为0.0000。当值不为0.0000时，说明指数欧洲区已收盘

    /// QTS2.6增加部分,大商所、郑商所、易盛指数
    FID_InterestChg             = 1287,         ///< 持仓量变化
    FID_LifeHigh                = 1288,         ///< 历史最高成交价格
    FID_LifeLow                 = 1289,         ///< 历史最低成交价格
    FID_ClearPrice              = 1290,         ///< 当日交割结算价
    FID_AveragePrice            = 1291,         ///< 均价
    FID_Gamma                   = 1293,         ///< Gamma
    FID_Rho                     = 1294,         ///< Rho
    FID_Theta                   = 1295,         ///< Theta
    FID_Vega                    = 1296,         ///< Vega
    FID_DeriveBidPrice          = 1297,         ///< 组合买入价
    FID_DeriveAskPrice          = 1298,         ///< 组合卖出价
    FID_DeriveBidLot            = 1299,         ///< 组合买入数量
    FID_DeriveAskLot            = 1300,         ///< 组合卖出数量
    FID_LastLot                 = 1398,         ///< 最后一笔成交手数
                                  
    FID_MBLQuotBuyNum           = 1301,         ///< 深度买行情数量
    FID_MBLQuotSellNum          = 1302,         ///< 深度卖行情数量
    FID_Price01                 = 1303,         ///< 价格1
    FID_PriceBOQty01            = 1304,         ///< 买开数量01
    FID_PriceBEQty01            = 1305,         ///< 买平数量01
    FID_PriceSOQty01            = 1306,         ///< 卖开数量01
    FID_PriceSEQty01            = 1307,         ///< 卖平数量01
    FID_Price02                 = 1308,         ///< 价格2
    FID_PriceBOQty02            = 1309,         ///< 买开数量02
    FID_PriceBEQty02            = 1310,         ///< 买平数量02
    FID_PriceSOQty02            = 1311,         ///< 卖开数量02
    FID_PriceSEQty02            = 1312,         ///< 卖平数量02
    FID_Price03                 = 1313,         ///< 价格3
    FID_PriceBOQty03            = 1314,         ///< 买开数量03
    FID_PriceBEQty03            = 1315,         ///< 买平数量03
    FID_PriceSOQty03            = 1316,         ///< 卖开数量03
    FID_PriceSEQty03            = 1317,         ///< 卖平数量03
    FID_Price04                 = 1318,         ///< 价格4
    FID_PriceBOQty04            = 1319,         ///< 买开数量04
    FID_PriceBEQty04            = 1320,         ///< 买平数量04
    FID_PriceSOQty04            = 1321,         ///< 卖开数量04
    FID_PriceSEQty04            = 1322,         ///< 卖平数量04
    FID_Price05                 = 1323,         ///< 价格5
    FID_PriceBOQty05            = 1324,         ///< 买开数量05
    FID_PriceBEQty05            = 1325,         ///< 买平数量05
    FID_PriceSOQty05            = 1326,         ///< 卖开数量05
    FID_PriceSEQty05            = 1327,         ///< 卖平数量05
    FID_RoutineNo               = 1328,         ///< 事务编号
                                  
    FID_BidImplyQty01           = 1330,         ///< 申买推导量一
    FID_BidImplyQty02           = 1331,         ///< 申买推导量二
    FID_BidImplyQty03           = 1332,         ///< 申买推导量三
    FID_BidImplyQty04           = 1333,         ///< 申买推导量四
    FID_BidImplyQty05           = 1334,         ///< 申买推导量五
    FID_AskImplyQty01           = 1335,         ///< 申卖推导量一
    FID_AskImplyQty02           = 1336,         ///< 申卖推导量二
    FID_AskImplyQty03           = 1337,         ///< 申卖推导量三
    FID_AskImplyQty04           = 1338,         ///< 申卖推导量四
    FID_AskImplyQty05           = 1339,         ///< 申卖推导量五
                                  
    FID_LastMatchQty            = 1341,         ///< 最新成交量
                                  
    // 2.6.4 增加三个字段         
    FID_WtAvgRate               = 1399,         ///< 实时加权平均利率
    FID_WtAvgRateUpdown         = 1400,         ///< 加权平均利率涨跌BP
    FID_PreWtAvgRate            = 1401,         ///< 昨收盘加权平均利率
    FID_QualificationClass      = 1402,         ///< 投资者适当性管理分类

    // 2.7 港股需要添加的字段
    FID_ChannelID               = 1453,         ///< 信道ID
    FID_SpreadTableCode         = 1413,         ///< 证券价差表,'01'：Part A  '02'：Part B
    FID_SecurityNameGCCS        = 1414,         ///< 证券名称的繁体中文编码，UTF8编码
    FID_SecurityNameGB          = 1415,         ///< 证券名称的简体中文编码，UTF8编码
    FID_VCMFlag                 = 1416,         ///< VCM标的证券标志，Y：适用于VCM  N：不适用于VCM
    FID_ShortSellFlag           = 1417,         ///< 卖空授权标志，Y：允许卖空  N：不允许卖空
    FID_CASFlag                 = 1418,         ///< CAS标的证券，Y：适用于CAS  N：不适用于CAS
    FID_CCASSFlag               = 1419,         ///< 中央结算证券，Y：中央结算证券  N：非中央结算证券
    FID_DummySecurityFlag       = 1420,         ///< 虚拟证券标志，Y：虚拟证券  N：正常证券
    FID_TestSecurityFlag        = 1421,         ///< 测试证券标志，Y：测试证券  N：正常证券
    FID_StampDutyFlag           = 1422,         ///< 印花税标志，Y：需要印花税  N：不需要印花税
    FID_FreeText                = 1423,         ///< 自由文本，固定长度的数组. 当没有自由文档时由空格代替
    FID_EFNFlag                 = 1424,         ///< 外汇基金债券标志，Y：是外汇基金债券  N：不是外汇基金债券
    FID_AccruedInterest         = 1425,         ///< 应计利息，3位小数
    FID_StrikePrice             = 1426,         ///< 执行价格，3位小数
    FID_BasicCurrency           = 1431,         ///< HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币
    //FID_CallOrPut               = 1042,         ///< 认购认沽标志，对于衍生权证以及一篮子权证：C 认购、P认沽、O其他；对于股票挂钩票据、牛熊证：C 看涨；P看跌 
    FID_NoUnderlyingSecurities  = 1428,         ///< 标的证券数目，0-20：一揽子认股权证；0-1：是权证和结构性产品
    FID_UnderlyingSecurityCode01 = 1541,        ///< 标的证券代码
    FID_UnderlyingSecurityCode02 = 1542,        ///< 标的证券代码
    FID_UnderlyingSecurityCode03 = 1543,        ///< 标的证券代码
    FID_UnderlyingSecurityCode04 = 1544,        ///< 标的证券代码
    FID_UnderlyingSecurityCode05 = 1545,        ///< 标的证券代码
    FID_UnderlyingSecurityCode06 = 1546,        ///< 标的证券代码
    FID_UnderlyingSecurityCode07 = 1547,        ///< 标的证券代码
    FID_UnderlyingSecurityCode08 = 1548,        ///< 标的证券代码
    FID_UnderlyingSecurityCode09 = 1549,        ///< 标的证券代码
    FID_UnderlyingSecurityCode10 = 1550,        ///< 标的证券代码
    FID_UnderlyingSecurityCode11 = 1551,        ///< 标的证券代码
    FID_UnderlyingSecurityCode12 = 1552,        ///< 标的证券代码
    FID_UnderlyingSecurityCode13 = 1553,        ///< 标的证券代码
    FID_UnderlyingSecurityCode14 = 1554,        ///< 标的证券代码
    FID_UnderlyingSecurityCode15 = 1555,        ///< 标的证券代码
    FID_UnderlyingSecurityCode16 = 1556,        ///< 标的证券代码
    FID_UnderlyingSecurityCode17 = 1557,        ///< 标的证券代码
    FID_UnderlyingSecurityCode18 = 1558,        ///< 标的证券代码
    FID_UnderlyingSecurityCode19 = 1559,        ///< 标的证券代码
    FID_UnderlyingSecurityCode20 = 1560,        ///< 标的证券代码
    FID_UnderlyingSecurityWeight01 = 1561,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight02 = 1562,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight03 = 1563,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight04 = 1564,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight05 = 1565,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight06 = 1566,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight07 = 1567,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight08 = 1568,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight09 = 1569,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight10 = 1570,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight11 = 1571,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight12 = 1572,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight13 = 1573,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight14 = 1574,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight15 = 1575,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight16 = 1576,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight17 = 1577,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight18 = 1578,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight19 = 1579,        ///< 标的证券代码权重
    FID_UnderlyingSecurityWeight20 = 1580,        ///< 标的证券代码权重
    FID_NoLiquidityProviders    = 1430,         ///< 做市商数目，1-50
    FID_LPBrokerNumber01        = 1581,         ///< 做市商经纪人数目
    FID_LPBrokerNumber02        = 1582,         ///< 做市商经纪人数目
    FID_LPBrokerNumber03        = 1583,         ///< 做市商经纪人数目
    FID_LPBrokerNumber04        = 1584,         ///< 做市商经纪人数目
    FID_LPBrokerNumber05        = 1585,         ///< 做市商经纪人数目
    FID_LPBrokerNumber06        = 1586,         ///< 做市商经纪人数目
    FID_LPBrokerNumber07        = 1587,         ///< 做市商经纪人数目
    FID_LPBrokerNumber08        = 1588,         ///< 做市商经纪人数目
    FID_LPBrokerNumber09        = 1589,         ///< 做市商经纪人数目
    FID_LPBrokerNumber10        = 1590,         ///< 做市商经纪人数目
    FID_LPBrokerNumber11        = 1591,         ///< 做市商经纪人数目
    FID_LPBrokerNumber12        = 1592,         ///< 做市商经纪人数目
    FID_LPBrokerNumber13        = 1593,         ///< 做市商经纪人数目
    FID_LPBrokerNumber14        = 1594,         ///< 做市商经纪人数目
    FID_LPBrokerNumber15        = 1595,         ///< 做市商经纪人数目
    FID_LPBrokerNumber16        = 1596,         ///< 做市商经纪人数目
    FID_LPBrokerNumber17        = 1597,         ///< 做市商经纪人数目
    FID_LPBrokerNumber18        = 1598,         ///< 做市商经纪人数目
    FID_LPBrokerNumber19        = 1599,         ///< 做市商经纪人数目
    FID_LPBrokerNumber20        = 1600,         ///< 做市商经纪人数目
    FID_LPBrokerNumber21        = 1601,         ///< 做市商经纪人数目
    FID_LPBrokerNumber22        = 1602,         ///< 做市商经纪人数目
    FID_LPBrokerNumber23        = 1603,         ///< 做市商经纪人数目
    FID_LPBrokerNumber24        = 1604,         ///< 做市商经纪人数目
    FID_LPBrokerNumber25        = 1605,         ///< 做市商经纪人数目
    FID_LPBrokerNumber26        = 1606,         ///< 做市商经纪人数目
    FID_LPBrokerNumber27        = 1607,         ///< 做市商经纪人数目
    FID_LPBrokerNumber28        = 1608,         ///< 做市商经纪人数目
    FID_LPBrokerNumber29        = 1609,         ///< 做市商经纪人数目
    FID_LPBrokerNumber30        = 1610,         ///< 做市商经纪人数目
    FID_LPBrokerNumber31        = 1611,         ///< 做市商经纪人数目
    FID_LPBrokerNumber32        = 1612,         ///< 做市商经纪人数目
    FID_LPBrokerNumber33        = 1613,         ///< 做市商经纪人数目
    FID_LPBrokerNumber34        = 1614,         ///< 做市商经纪人数目
    FID_LPBrokerNumber35        = 1615,         ///< 做市商经纪人数目
    FID_LPBrokerNumber36        = 1616,         ///< 做市商经纪人数目
    FID_LPBrokerNumber37        = 1617,         ///< 做市商经纪人数目
    FID_LPBrokerNumber38        = 1618,         ///< 做市商经纪人数目
    FID_LPBrokerNumber39        = 1619,         ///< 做市商经纪人数目
    FID_LPBrokerNumber40        = 1620,         ///< 做市商经纪人数目
    FID_LPBrokerNumber41        = 1621,         ///< 做市商经纪人数目
    FID_LPBrokerNumber42        = 1622,         ///< 做市商经纪人数目
    FID_LPBrokerNumber43        = 1623,         ///< 做市商经纪人数目
    FID_LPBrokerNumber44        = 1624,         ///< 做市商经纪人数目
    FID_LPBrokerNumber45        = 1625,         ///< 做市商经纪人数目
    FID_LPBrokerNumber46        = 1626,         ///< 做市商经纪人数目
    FID_LPBrokerNumber47        = 1627,         ///< 做市商经纪人数目
    FID_LPBrokerNumber48        = 1628,         ///< 做市商经纪人数目
    FID_LPBrokerNumber49        = 1629,         ///< 做市商经纪人数目
    FID_LPBrokerNumber50        = 1630,         ///< 做市商经纪人数目
    FID_Yield                   = 1432,         ///< 债券当期收益率：基于息票率和名义价格，3位小数
    FID_NominalPrice            = 1434,         ///< 按盘价，3位小数
    FID_VWAP                    = 1435,         ///< 成交量加权平均价格，3位小数
    FID_ShortSellSharesTraded   = 1436,         ///< 卖空成交总量
    FID_ShortSellTurnover       = 1437,         ///< 卖空成交金额，3位小数
    FID_TradeCancelFlag         = 1433,         ///< 交易是否被取消标志。Y：取消  N：没有取消  指示：原来交易收录器中的交易已经被取消了
    FID_AggregatePrice          = 1438,         ///< 参考平衡价格，3位小数，IEP不适用时取值为0
    FID_AggregateQuantity       = 1439,         ///< 参考平衡成交量
    FID_CASReferencePrice       = 1440,         ///< 委托订单参考价格（CAS阶段），3位小数，如果参考价不适用，取值为0
    FID_CASLowerPrice           = 1441,         ///< 委托订单参考价格（CAS阶段），3位小数，取值0表示N/A
    FID_CASUpperPrice           = 1442,         ///< 委托订单上限价格（CAS阶段），3位小数，取值0表示N/A
    FID_OrderImbalanceDirection = 1443,         ///< 买卖数量不平衡方向，N：买卖相等 B：买方剩余 S：卖方剩余 空格：不适用，当参考平衡价不适用
    FID_OrderImbalanceQuantity  = 1444,         ///< 买卖不平衡数量，当不平衡方向OrderImbalanceDirection为空格，此值无意义
    FID_CoolingOffStartTime     = 1445,         ///< 冷静期开始时间，交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
    FID_CoolingOffEndTime       = 1446,         ///< 冷静期结束时间，交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
    FID_VCMReferencePrice       = 1447,         ///< 冷静期参考价，3位小数
    FID_VCMLowerPrice           = 1448,         ///< 冷静期可交易价格下限，3位小数
    FID_VCMUpperPrice           = 1449,         ///< 冷静期可交易价格上限，3位小数
    FID_BuyBQMoreFlag           = 1452,         ///< 买方经纪人队列标志，Y：队列中还有经纪人 N：队列中已没有经纪人
    FID_BuyBQItemCount          = 1450,         ///< 买方经纪人队列数0-40
    FID_BuyBrokerID01           = 1454,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID02           = 1455,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID03           = 1456,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID04           = 1457,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID05           = 1458,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID06           = 1459,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID07           = 1460,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID08           = 1461,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID09           = 1462,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID10           = 1463,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID11           = 1464,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID12           = 1465,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID13           = 1466,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID14           = 1467,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID15           = 1468,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID16           = 1469,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID17           = 1470,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID18           = 1471,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID19           = 1472,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID20           = 1473,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID21           = 1474,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID22           = 1475,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID23           = 1476,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID24           = 1477,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID25           = 1478,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID26           = 1479,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID27           = 1480,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID28           = 1481,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID29           = 1482,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID30           = 1483,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID31           = 1484,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID32           = 1485,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID33           = 1486,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID34           = 1487,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID35           = 1488,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID36           = 1489,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID37           = 1490,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID38           = 1491,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID39           = 1492,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBrokerID40           = 1493,         ///< 买方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_BuyBQPriceLevel01       = 1494,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel02       = 1495,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel03       = 1496,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel04       = 1497,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel05       = 1498,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel06       = 1499,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel07       = 1500,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel08       = 1501,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel09       = 1502,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel10       = 1503,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel11       = 1504,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel12       = 1505,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel13       = 1506,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel14       = 1507,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel15       = 1508,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel16       = 1509,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel17       = 1510,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel18       = 1511,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel19       = 1512,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel20       = 1513,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel21       = 1514,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel22       = 1515,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel23       = 1516,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel24       = 1517,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel25       = 1518,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel26       = 1519,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel27       = 1520,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel28       = 1521,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel29       = 1522,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel30       = 1523,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel31       = 1524,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel32       = 1525,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel33       = 1526,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel34       = 1527,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel35       = 1528,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel36       = 1529,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel37       = 1530,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel38       = 1531,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel39       = 1532,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_BuyBQPriceLevel40       = 1533,         ///< 该标志表明买方经纪人ID所对应的价格档位
    FID_MarketName              = 1534,         ///< 市场名称
    FID_PreviousSesClose        = 1404,         ///< 前交易阶段收盘值，4位有效小数（CSI、S&P为前一个交易日收盘价，HIS、TR为上一交易阶段的收盘值）
    FID_EASValue                = 1405,         ///< 清算价平均估算值，2位有效小数
    FID_Exception               = 1406,         ///< 异常指示器
    FID_CurrencyFactor          = 1403,         ///< 货币转换因子,非零值 n 意味着这个代码的所有价格字段应该如此表示：价值= 该价格 * 10^n
    FID_TradingSessionID        = 1409,         ///< 1：日中
    FID_TradingSessionSubID     = 1410,         ///< 交易阶段子标识符
                                                ///< 0：日关闭(DC)
                                                ///< 1：开盘前 (Order Input OI)
                                                ///< 2：开盘或竞价开盘(Matching MA)
                                                ///< 3：连续交易(Continuous CT)   
                                                ///< 4：收市或收市竞价阶段（Matching (MA)）
                                                ///< 5：交易后[CAS]订单输入（OI）
                                                ///< 7：暂停 (Blocking BL)
                                                ///< 100: 尚未开市 (NO)
                                                ///< 101：不能取消/修改 (NC)
                                                ///< 102：交易所干预(EI)
                                                ///< 103：闭市 (CL)
                                                ///< 104：取消委托(OC)
                                                ///< 105：参考价定价[CAS]（RP）
                                                ///< 106：不可取消[CAS]（NW）
                                                ///< 107：随机收市[CAS] (RC)
    FID_TradingSesStatus        = 1411,         ///< 当前交易阶段状态
                                                ///< 0：未知（为NO）
                                                ///< 1：停牌（对于BL，EI）
                                                ///< 2：开盘（对于[POS] OI，[POS] NC，[POS] MA，CT，OC）
                                                ///< 3：关闭（针对CL）
                                                ///< 5：预关闭（对于[CAS] RP，[CAS] NW，[CAS] RC，[CAS] MA，[CAS] OI）
                                                ///< 100：日关闭(对于DC)
    FID_TradingSesControlFlag   = 1412,         ///< 不同时段切换方式标志，‘0’：自动的 (默认)；‘1’：手动的 (这天的正常调度无效)
    FID_StartDateTime           = 1407,         ///< 交易状态开始时间，交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
    FID_EndDateTime             = 1408,         ///< 交易状态结束时间，交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss


    FID_SellBQMoreFlag          = 1451,         ///< 卖方经纪人队列标志，Y：队列中还有经纪人 N：队列中已没有经纪人
    FID_SellBQItemCount         = 1535,         ///< 卖方经纪人队列数0-40
    FID_SellBrokerID01          = 1631,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID02          = 1632,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID03          = 1633,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID04          = 1634,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID05          = 1635,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID06          = 1636,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID07          = 1637,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID08          = 1638,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID09          = 1639,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID10          = 1640,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID11          = 1641,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID12          = 1642,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID13          = 1643,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID14          = 1644,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID15          = 1645,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID16          = 1646,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID17          = 1647,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID18          = 1648,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID19          = 1649,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID20          = 1650,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID21          = 1651,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID22          = 1652,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID23          = 1653,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID24          = 1654,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID25          = 1655,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID26          = 1656,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID27          = 1657,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID28          = 1658,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID29          = 1659,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID30          = 1660,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID31          = 1661,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID32          = 1662,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID33          = 1663,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID34          = 1664,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID35          = 1665,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID36          = 1666,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID37          = 1667,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID38          = 1668,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID39          = 1669,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBrokerID40          = 1670,         ///< 卖方经纪人队列中经纪人ID的值或显示距离最有价格的价差
    FID_SellBQPriceLevel01      = 1671,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel02      = 1672,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel03      = 1673,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel04      = 1674,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel05      = 1675,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel06      = 1676,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel07      = 1677,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel08      = 1678,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel09      = 1679,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel10      = 1680,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel11      = 1681,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel12      = 1682,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel13      = 1683,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel14      = 1684,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel15      = 1685,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel16      = 1686,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel17      = 1687,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel18      = 1688,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel19      = 1689,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel20      = 1690,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel21      = 1691,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel22      = 1692,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel23      = 1693,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel24      = 1694,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel25      = 1695,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel26      = 1696,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel27      = 1697,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel28      = 1698,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel29      = 1699,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel30      = 1700,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel31      = 1701,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel32      = 1702,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel33      = 1703,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel34      = 1704,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel35      = 1705,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel36      = 1706,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel37      = 1707,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel38      = 1708,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel39      = 1709,         ///< 该标志表明卖方经纪人ID所对应的价格档位
    FID_SellBQPriceLevel40      = 1710,         ///< 该标志表明卖方经纪人ID所对应的价格档位
};

///消息结构体的字段类型
enum FIELD_TYPE{    
   Type_ERROR       = 0,           ///< 未知类型
   Type_Char        = 1,           ///< char
   Type_Short       = 2,           ///< short
   Type_Int         = 3,           ///< int
   Type_UInt        = 4,           ///< unsigned int
   Type_Longlong    = 5,           ///< long long 
   Type_ULonglong   = 6,           ///< unsigned long long 
   Type_String      = 7,           ///< string 字符串
   Type_Double      = 8,           ///< double
   Type_Sequence    = 9,           ///< queue 队列
};

/// 数据结构字段长度定义
#define SYMBOL_LEN          40      ///< 代码长度
#define SECURITY_NAME_LEN   20      ///< 证券名称长度
#define SECURITY_EN_LEN     24      ///< 英文证券简称
#define MARKET_TAG_LEN      10      ///< 市场标志长度
#define TIMEOUT_DEFAULT     3       ///< 默认设置超时时长
#define QUOTATION_FLAG_LEN  4       ///< 行情标志长度
#define SYMBOLSOURCE_LEN    5       ///< 代码源长度
#define PHRASE_TAG_LEN      8       ///< 当前品种交易状态长度
#define SETTLE_GROUP_ID_LEN 10      ///< 结算组代码长度
#define USER_NAME_LEN       50      ///< 用户名长度
#define USER_PWD_LEN        16      ///< 密码长度
#define BROKERQUEUE_LEN     40      ///< 队列数组长度
#define UNDERLYINGSECURITYES_LEN  20  ///< 标的证券数目队列数
#define LPBROKERNUMBER_LEN  50      ///< 做市商经纪人ID队列数

/// 网络类型
enum NetType{
    Net_Type_A   = 0,   ///< 表示外网，默认值
    Net_Type_B   = 1,   ///< 表示特殊网络，如内网、专线等
};

#endif // GTA_QTS_DATA_TYPE_H_
