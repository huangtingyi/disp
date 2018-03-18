//////////////////////////////////////////////////////////////////////////////
/// @file       QTSStruct.h 
/// @brief      数据消息结构体定义文件
/// @copyright  Copyright (C), 2008-2014, GTA Information Tech. Co., Ltd.
/// @version    2.0
/// @date       2014.7.18
//////////////////////////////////////////////////////////////////////////////
#ifndef GTA_QTS_STRUCT_H_
#define GTA_QTS_STRUCT_H_
#include "QTSDataType.h"

#pragma pack(8)


/// 买卖档位信息，含价、量、笔数3个信息
struct  BuySellLevelInfo3
{
    double              Price;                  ///< 价格,3位小数
    unsigned long long  Volume;                 ///< 量
    unsigned int        TotalOrderNo;           ///< 委托笔数
};
/// 深交所 买卖档位信息，含价、量、笔数3个信息
struct SZSE_BuySellLevelInfo3
{
    double              Price;                  ///< 价格,6位小数
    double              Volume;                 ///< 量，2位小数
    unsigned long long  TotalOrderNo;           ///< 委托笔数
};
/// 买卖档位信息，含价、量信息
struct  BuySellLevelInfo
{
    double              Price;                  ///< 价格,3位小数
    unsigned long long  Volume;                 ///< 量
};
/// 大商所 买卖档位信息，含价、量、推导量3个信息
struct  DCE_BuySellLevelInfo3
{
    double              Price;                  ///< 价格,3位小数
    unsigned long long  Volume;                 ///< 量
    unsigned long long  ImplyQty;               ///< 推导量
};
/// 大商所 买卖委托档位信息
struct  DCE_BuySellOrdeLevelInfo
{
    unsigned int        TotalOrderNo;           ///< 委托笔数
};
/// 大商所 分价成交量行情信息
struct  DCE_PriceLevelInfo
{
    double              Price;               ///< 价格
    unsigned long long  PriceBOQty;          ///< 买开数量
    unsigned long long  PriceBEQty;          ///< 买平数量
    unsigned long long  PriceSOQty;          ///< 卖开数量
    unsigned long long  PriceSEQty;          ///< 卖平数量
};
/// 深交所买卖档位信息
struct SZSE_BuySellLevelInfo
{
    double              Price;                  ///< 价格
    double              Volume;                 ///< 量
};

#define LEVEL_ONE           1       ///< 一档行情
#define LEVEL_FIVE          5       ///< 五档行情
#define LEVEL_TEN           10      ///< 十档行情
#define SETTLE_LEN          10      ///< 结算组代码长度
#define ORDER_LEVEL_FIFTY   50      ///< 50笔委托
#define OTC_ELEMENT_LEN     3       ///< 开盘、交易、收盘竞价参数个数

/// 上交所L1静态数据
struct SSEL1_Static
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 Time;                               ///< 数据生成时间，标识接口中本记录更新时间HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码, 
    char                ISINCode[12];                       ///< ISIN代码, 
    char                SecurityName[SECURITY_NAME_LEN];    ///< 证券名称, 
    char                SecurityEN[SECURITY_EN_LEN];        ///< 英文证券名称, 
    char                SymbolUnderlying[SYMBOL_LEN];       ///< 基础证券代码, 
    char                MarketType[6];                      ///< 市场种类, ‘ASHR’表示A股市场；‘BSHR’表示B股市场；‘CSHR‘表示国际版市场。
    char                CFICode[6];                         ///< 证券类别, ’E'表示股票；‘EU’表示基金；‘D’表示债券；‘RWS’表示权证；‘FF’表示期货。
    char                SecuritySubType[6];                 ///< 证券子类别, 自定义详细证券类别
    char                Currency[5];                        ///< 币种, CNY = 人民币，HKD = 港币
    double              ParValue;                           ///< 面值.债券当前面值，单位元，其他产品取0.000
    long long           TradableNo;                         ///< 可流通未上市数量, 预留
    int                 EndDate;                            ///< 最后交易日期. 对于国债预发行产品，为最后交易日期，格式为YYYYMMDD
    int                 ListingDate;                        ///< 上市日期, 在上交所首日交易日期，YYYYMMDD
    unsigned int        SetNo;                              ///< 产品集编号, 取值范围从1到999,。用来表明产品的一种分组方式，用于在多主机间记性负载均衡分配。该值在一个交易日内不会变化。
    unsigned int        BuyVolumeUnit;                      ///< 买数量单位, 买订单的申报数量必须是该字段的整数倍。
    unsigned int        SellVolumeUnit;                     ///< 卖数量单位, 卖订单的申报数量必须是该字段的整数倍。
    unsigned int        DeclareVolumeFloor;                 ///< 申报量下限, 申报数量下限
    unsigned int        DeclareVolumeCeiling;               ///< 申报量上限, 申报数量上限
    double              PreClosePrice;                      ///< 昨收价, 3位小数；前收盘价格（如有除权除息，为除权除息后的收盘价）；对于货币市场基金实时申赎，取值为0.010
    double              TickSize;                           ///< 最小报价单位, 申报价格的最小变动单位
    char                UpDownLimitType;                    ///< 涨跌幅限制类型, ‘N’表示交易规则3.4.13规定的有涨跌幅限制类型或者权证管理办法第22条规定；‘R'表示交易规则3.4.15和3.4.16规定的无涨跌幅限制类型；’S‘表示回购涨跌幅控制类型。
    double              PriceUpLimit;                       ///< 涨幅价格, 对于N类型涨跌幅限制的产品，该字段当日不会更改，基于前收盘价（已首日上市交易产品为发行价）计算；对于R类型五涨跌幅限制的产品，该字段取开盘时基于参考价格计算的上限价格。
    double              PriceDownLimit;                     ///< 跌幅价格, 计算方式参考涨幅上限价格
    double              XRRatio;                            ///< 除权比例, 每股送股比例；对于国债预发行产品，为保证金比例。
    double              XDAmount;                           ///< 除息金额, 每股分红金额
    char                CrdBuyUnderlying;                   ///< 融资标的标志, ’T‘表示是融资标的证券；’F‘表示不是融资标的证券。
    char                CrdSellUnderlying;                  ///< 融券标的标志, ’T‘表示是融拳标的证券；’F‘表示不是融券标的证券。
    char                SecurityStatus[20];                 ///< 该字段为20位字符串，每位表示允许对应的业务，无定义则填空格。
                                                            ///< 第0位对应：‘N’表示首日上市。
                                                            ///< 第1位对应：‘D’表示除权。
                                                            ///< 第2位对应：‘R’表示除息。
                                                            ///< 第3位对应：’D’表示国内主板正常交易产品，’S’表示股票风险警示产品，’P’表示退市整理产品，’T’表示退市转让产品，’U’表示优先股产品。
                                                            ///< 第4位对应：’S’表示债券风险警示类。第4位未来改为不启用，此改动暂未实施，具体以交易所通知为准。
                                                            ///< 第5位对应：’L’表示债券投资者适当性要求类。
    int                 TradeDate;                          ///< 市场日期,交易日

};

/// 上交所L1实时行情
struct SSEL1_Quotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志,1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间,交易所发包时间，格式为：日期时间YYYYMMDDHHMMSSMMM
    int                 Time;                               ///< 数据生成时间,时间戳143025表示 14:30:25
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码,期权合约的产品代码;唯一标示
    char                SecurityName[SECURITY_NAME_LEN];    ///< 证券简称,UTF-8编码
    double              OpenPrice;                          ///< 开盘价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LastPrice;                          ///< 现价
    unsigned long long  TotalNo;                            ///< 成交笔数
    unsigned long long  TotalVolume;                        ///< 成交总量,股票：股;权证：份;债券：手
    double              TotalAmount;                        ///< 成交总额,（元）
    double              PreClosePrice;                      ///< 昨收价
    double              ClosePrice;                         ///< 今收盘价
    union
    {
        BuySellLevelInfo    SellLevel[LEVEL_FIVE];      ///< 五档卖行情
        struct  
        {
            double              SellPrice01;            ///< 申卖价1
            unsigned long long  SellVolume01;           ///< 申卖量1
            double              SellPrice02;            ///< 申卖价2
            unsigned long long  SellVolume02;           ///< 申卖量2
            double              SellPrice03;            ///< 申卖价3
            unsigned long long  SellVolume03;           ///< 申卖量3
            double              SellPrice04;            ///< 申卖价4
            unsigned long long  SellVolume04;           ///< 申卖量4
            double              SellPrice05;            ///< 申卖价5
            unsigned long long  SellVolume05;           ///< 申卖量5
        };
    };
    union
    {
        BuySellLevelInfo    BuyLevel[LEVEL_FIVE];        ///< 五档买行情
        struct  
        {
            double              BuyPrice01;             ///< 申买价1
            unsigned long long  BuyVolume01;            ///< 申买量1
            double              BuyPrice02;             ///< 申买价2
            unsigned long long  BuyVolume02;            ///< 申买量2
            double              BuyPrice03;             ///< 申买价3
            unsigned long long  BuyVolume03;            ///< 申买量3
            double              BuyPrice04;             ///< 申买价4
            unsigned long long  BuyVolume04;            ///< 申买量4
            double              BuyPrice05;             ///< 申买价5
            unsigned long long  BuyVolume05;            ///< 申买量5
        };
    };
    double              NAV;                                ///< 基金T-1日累计净值,四位小数
    double              IOPV;                               ///< ETF净值估值
    char                SecurityPhaseTag[PHRASE_TAG_LEN];   ///< 该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。
                                                            ///< 第1位：‘S’表示启动（开市前）时段，‘C’表示集合竞价时段，‘T’表示连续交易时段，‘B’表示休市时段，‘E’表示闭市时段，‘P’表示产品停牌，‘M’表示可恢复交易的熔断时段（盘中集合竞价），‘N’表示不可恢复交易的熔断时段（暂停交易至闭市），‘D’表示集合竞价阶段结束到连续竞价阶段开始之前的时段（如有）。
                                                            ///< 第2位：‘0’表示此产品不可正常交易，‘1’表示此产品可正常交易，无意义填空格。
                                                            ///< 第3位：‘0’表示未上市，‘1’表示已上市。
                                                            ///< 第4位：‘0’表示此产品在当前时段不接受进行新订单申报，‘1’ 表示此产品在当前时段可接受进行新订单申报。无意义填空格。
    int                 SecurityType;                       ///< 证券类别代码，1表示指数行情数据； 2表示股票（ A、 B 股）行情数据；3表示债券行情数据；4表示基金行情数据；
};

/// 上交所指数通数据
struct SSE_IndexPress
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    int                 TradeDate;                          ///< 交易日,日期的格式为“YYYYMMDD”
    int                 ActionDay;                          ///< 业务发生日,行情文件所代表的自然日期（北京时间），内容为被用于计算的那天的自然日期（北京时间） 日期的格式为“ YYYYMMDD”
    int                 Time;                               ///< 数据生成时间，行情文件的更新时间戳（北京时间），格式为“HHMMSS”
    char                Symbol[SYMBOL_LEN];                 ///< 指数代码
    char                SecurityName[40];                   ///< 指数简称
    char                MarketType[6];                      ///< 市场代码 0：全球；1：上证所； 2：深交所； 3：沪深；4：香港； 5：亚太；
    double              LastPrice;                          ///< 最新价,当日开盘值，当前交易日开盘指数值。 初始值为 0.0000。 当值为 0.0000时，说明指数未开盘
    double              OpenPrice;                          ///< 开盘价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              ClosePrice;                         ///< 今收盘价,当前交易日收盘值。初始值为 0.0000。当值不为 0.0000时，说明指数已收盘
    double              PreClosePrice;                      ///< 昨收价
    double              PriceUpdown1;                       ///< 涨跌 LastPrice-PreClosePrice
    double              UpDownRate;                         ///< 涨跌幅
    unsigned long long  TotalVolume;                        ///< 成交总量,单位：股
    double              TotalAmount;                        ///< 成交金额,（万元）
    double              ExchangeRate;                       ///< 汇率，该汇率在盘中时为0.00000000，收盘后，该汇率值为该指数收盘时计算指数所使用的汇率
    char                Currency[5];                        ///< 币种, 0：人民币； 1：港币； 2：美元； 3：台币； 4：日元
    int                 DisplayNum;                         ///< 指数展示序号,展示指数的顺序按指数展示序号进行排列
    double              ClosePrice2;                        ///< 当日收盘值2，若该指数为全球指数，该收盘值为当日亚太区收盘值。初始值为0.0000。当值不为0.0000时，说明指数亚太区已收盘
    double              ClosePrice3;                        ///< 当日收盘值3，若该指数为全球指数，该收盘值为当日欧洲区收盘值。初始值为0.0000。当值不为0.0000时，说明指数欧洲区已收盘
};

/// 上交所L2静态数据
struct SSEL2_Static
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                               ///< 数据生成时间，标识接口中本记录更新时间HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码, 
    char                ISINCode[12];                       ///< ISIN代码, 
    char                SecurityName[SECURITY_NAME_LEN];    ///< 证券名称, 
    char                SecurityEN[SECURITY_EN_LEN];        ///< 英文证券名称, 
    char                SymbolUnderlying[SYMBOL_LEN];       ///< 基础证券代码, 
    char                MarketType[6];                      ///< 市场种类, ‘ASHR’表示A股市场；‘BSHR’表示B股市场；‘CSHR‘表示国际版市场。
    char                CFICode[6];                         ///< 证券类别, ’E'表示股票；‘EU’表示基金；‘D’表示债券；‘RWS’表示权证；‘FF’表示期货。
    char                SecuritySubType[6];                 ///< 证券子类别, 自定义详细证券类别
    char                Currency[5];                        ///< 币种, CNY = 人民币，HKD = 港币
    double              ParValue;                           ///< 面值,债券当前面值，单位元，其他产品取0.000
    long long           TradableNo;                         ///< 可流通未上市数量, 预留
    int                 EndDate;                            ///< 最后交易日期,对于国债预发行产品，为最后交易日期，格式为YYYYMMDD
    int                 ListingDate;                        ///< 上市日期, 在上交所首日交易日期，YYYYMMDD
    unsigned int        SetNo;                              ///< 产品集编号, 取值范围从1到999,。用来表明产品的一种分组方式，用于在多主机间记性负载均衡分配。该值在一个交易日内不会变化。
    unsigned int        BuyVolumeUnit;                      ///< 买数量单位, 买订单的申报数量必须是该字段的整数倍。
    unsigned int        SellVolumeUnit;                     ///< 卖数量单位, 卖订单的申报数量必须是该字段的整数倍。
    unsigned int        DeclareVolumeFloor;                 ///< 申报量下限, 申报数量下限
    unsigned int        DeclareVolumeCeiling;               ///< 申报量上限, 申报数量上限
    double              PreClosePrice;                      ///< 昨收价, 3位小数；前收盘价格（如有除权除息，为除权除息后的收盘价）；对于货币市场基金实时申赎，取值为0.010
    double              TickSize;                           ///< 最小报价单位, 申报价格的最小变动单位
    char                UpDownLimitType;                    ///< 涨跌幅限制类型, ‘N’表示交易规则3.4.13规定的有涨跌幅限制类型或者权证管理办法第22条规定；‘R'表示交易规则3.4.15和3.4.16规定的无涨跌幅限制类型；’S‘表示回购涨跌幅控制类型。
    double              PriceUpLimit;                       ///< 涨幅价格, 对于N类型涨跌幅限制的产品，该字段当日不会更改，基于前收盘价（已首日上市交易产品为发行价）计算；对于R类型五涨跌幅限制的产品，该字段取开盘时基于参考价格计算的上限价格。
    double              PriceDownLimit;                     ///< 跌幅价格, 计算方式参考涨幅上限价格
    double              XRRatio;                            ///< 除权比例, 每股送股比例；对于国债预发行产品，为保证金比例。
    double              XDAmount;                           ///< 除息金额, 每股分红金额
    char                CrdBuyUnderlying;                   ///< 融资标的标志, ’T‘表示是融资标的证券；’F‘表示不是融资标的证券。
    char                CrdSellUnderlying;                  ///< 融券标的标志, ’T‘表示是融拳标的证券；’F‘表示不是融券标的证券。
    char                SecurityStatus[20];                 ///< 产品状态标识, 第0位对应：‘N’表示首日上市；第1位对应：‘D’表示除权；第2位对应：‘R'表示除息；第3位对应：‘D‘表示国内主板正常交易产品，‘S’表示风险警示产品，‘P’表示退市整理产品。
    unsigned int        SampleNo;                           ///< 样本数量, 指数当前的样本数量
    double              SampleAvgPrice;                     ///< 样本均价, 指数当前样本的均价,市价总值/发行股本,若该指标不统计则输出 N/A
    double              TradeAmount;                        ///< 成交金额, 指数当前样本的当日成交金额（单位：亿元）
    double              AvgCapital;                         ///< 平均股本, 指数当前样本的平均股本,（算术平均，单位：亿股）,若该指标不统计则输出 N/A
    double              TotalMarketValue;                   ///< 总市值, 指数当前样本的总市值汇总 （算术和， 单位： 万亿元）,若该指标不统计则输出 N/A
    double              MarketValueRatio;                   ///< 占比%, 指数当前样本的总市值占上证综指全样本的总市值,百分比,若该指标不统计则输出 N/A
    double              StaticPERatio;                      ///< 静态市盈率, 指数当前样本的静态市盈率。公式：合计（人民币收盘价*发行量） /合计（每股收益*发行量）,若该指标未统计则输出 N/A
    char                IndexLevelStatus[20];               ///< 指数级别标识, 前 5 位为指数排序数值，最后一位即指数级别信息：
                                                            ///< 1 为重点指数
                                                            ///< 2 为全貌指数
                                                            ///< 其他可根据需要扩展。
    int                 TradeDate;                          ///< 市场日期,交易日 YYYYMMDD

};

/// 上交所L2实时行情
struct SSEL2_Quotation
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                   ///< 数据生成时间, 最新订单时间（毫秒）;143025001 表示 14:30:25.001
    char                Symbol[SYMBOL_LEN];     ///< 证券代码, 
    double              PreClosePrice;          ///< 昨收价, 
    double              OpenPrice;              ///< 开盘价, 
    double              HighPrice;              ///< 最高价, 
    double              LowPrice;               ///< 最低价, 
    double              LastPrice;              ///< 现价, 
    double              ClosePrice;             ///< 今收盘价, 
    char                TradeStatus[6];         ///< 当前品种交易状态,
    char                SecurityPhaseTag[PHRASE_TAG_LEN];   ///< 当前品种状态
                                                            ///< 该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。
                                                            ///< 第1位：‘S’表示启动（开市前）时段，‘C’表示集合竞价时段，‘T’表示连续交易时段，‘B’表示休市时段，‘E’表示闭市时段，‘P’表示产品停牌，‘M’表示可恢复交易的熔断时段（盘中集合竞价），‘N’表示不可恢复交易的熔断时段（暂停交易至闭市），‘D’表示集合竞价阶段结束到连续竞价阶段开始之前的时段（如有）。
                                                            ///< 第2位：‘0’表示此产品不可正常交易，‘1’表示此产品可正常交易，无意义填空格。
                                                            ///< 第3位：‘0’表示未上市，‘1’表示已上市。
                                                            ///< 第4位：‘0’表示此产品在当前时段不接受进行新订单申报，‘1’ 表示此产品在当前时段可接受进行新订单申报。无意义填空格。
    unsigned long long  TotalNo;                ///< 成交笔数, 
    unsigned long long  TotalVolume;            ///< 成交总量, 股票：股;权证：份;债券：手
    double              TotalAmount;            ///< 成交总额, （元）
    unsigned long long  TotalBuyOrderVolume;    ///< 委托买入总量, 股票：股;权证：份;债券：手
    double              WtAvgBuyPrice;          ///< 加权平均委买价, （元）
    double              BondWtAvgBuyPrice;      ///< 债券加权平均委买价, （元）
    unsigned long long  TotalSellOrderVolume;   ///< 委托卖出总量, 
    double              WtAvgSellPrice;         ///< 加权平均委卖价, （元）
    double              BondWtAvgSellPrice;     ///< 债券加权平均委卖价, 
    double              IOPV;                   ///< ETF 净值估值, 
    int                 ETFBuyNo;               ///< ETF 申购笔数, 
    long long           ETFBuyVolume;           ///< ETF 申购量, 
    double              ETFBuyAmount;           ///< ETF 申购额, 
    int                 ETFSellNo;              ///< ETF 赎回笔数, 
    long long           ETFSellVolume;          ///< ETF 赎回量, 
    double              ETFSellAmount;          ///< ETF 赎回额, 
    double              YTM;                    ///< 债券到期收益率, 
    long long           TotalWarrantExecVol;    ///< 权证执行的总数量, 
    double              WarrantDownLimit;       ///< 权证跌停价格, （元）
    double              WarrantUpLimit;         ///< 权证涨停价格, （元）
    int                 WithdrawBuyNo;          ///< 买入撤单笔数, 
    long long           WithdrawBuyVolume;      ///< 买入撤单量, 
    double              WithdrawBuyAmount;      ///< 买入撤单额, 
    int                 WithdrawSellNo;         ///< 卖出撤单笔数, 
    long long           WithdrawSellVolume;     ///< 卖出撤单量, 
    double              WithdrawSellAmount;     ///< 卖出撤单额, 
    int                 TotalBuyNo;             ///< 买入总笔数, 
    int                 TotalSellNo;            ///< 卖出总笔数, 
    int                 MaxBuyDuration;         ///< 买入成交最大等待时间, 
    int                 MaxSellDuration;        ///< 卖出成交最大等待时间, 
    int                 BuyOrderNo;             ///< 买方委托价位数, 
    int                 SellOrderNo;            ///< 卖方委托价位数, 
    unsigned int        SellLevelNo;            ///< 卖盘价位数量, 10档行情，不足时补空
    union
    {
        BuySellLevelInfo3    SellLevel[LEVEL_TEN];          ///< 十档卖行情
        struct
        {
                double              SellPrice01;            ///< 申卖价, 
                unsigned long long  SellVolume01;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo01;     ///< 卖出总委托笔数, 
                double              SellPrice02;            ///< 申卖价, 
                unsigned long long  SellVolume02;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo02;     ///< 卖出总委托笔数, 
                double              SellPrice03;            ///< 申卖价, 
                unsigned long long  SellVolume03;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo03;     ///< 卖出总委托笔数, 
                double              SellPrice04;            ///< 申卖价, 
                unsigned long long  SellVolume04;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo04;     ///< 卖出总委托笔数, 
                double              SellPrice05;            ///< 申卖价, 
                unsigned long long  SellVolume05;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo05;     ///< 卖出总委托笔数, 
                double              SellPrice06;            ///< 申卖价, 
                unsigned long long  SellVolume06;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo06;     ///< 卖出总委托笔数, 
                double              SellPrice07;            ///< 申卖价, 
                unsigned long long  SellVolume07;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo07;     ///< 卖出总委托笔数, 
                double              SellPrice08;            ///< 申卖价, 
                unsigned long long  SellVolume08;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo08;     ///< 卖出总委托笔数, 
                double              SellPrice09;            ///< 申卖价, 
                unsigned long long  SellVolume09;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo09;     ///< 卖出总委托笔数, 
                double              SellPrice10;            ///< 申卖价, 
                unsigned long long  SellVolume10;           ///< 申卖量, 
                unsigned int        TotalSellOrderNo10;     ///< 卖出总委托笔数, 
        };
    };
    unsigned int        SellLevelQueueNo01;                 ///< 卖一档揭示委托笔数, 为 0 表示不揭示
    unsigned int        SellLevelQueue[ORDER_LEVEL_FIFTY];  ///< 卖1档队列, 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
    unsigned int        BuyLevelNo;                         ///< 买盘价位数量, 10档行情，不足时补空
    union
    {
        BuySellLevelInfo3    BuyLevel[LEVEL_TEN];           ///< 十档买行情
        struct
        {
            double              BuyPrice01;                 ///< 申买价, 
            unsigned long long  BuyVolume01;                ///< 申买量, 股票：股;权证：份;债券：手
            unsigned int        TotalBuyOrderNo01;          ///< 买入总委托笔数, 
            double              BuyPrice02;                 ///< 申买价, 
            unsigned long long  BuyVolume02;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo02;          ///< 买入总委托笔数, 
            double              BuyPrice03;                 ///< 申买价, 
            unsigned long long  BuyVolume03;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo03;          ///< 买入总委托笔数, 
            double              BuyPrice04;                 ///< 申买价, 
            unsigned long long  BuyVolume04;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo04;          ///< 买入总委托笔数, 
            double              BuyPrice05;                 ///< 申买价, 
            unsigned long long  BuyVolume05;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo05;          ///< 买入总委托笔数, 
            double              BuyPrice06;                 ///< 申买价, 
            unsigned long long  BuyVolume06;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo06;          ///< 买入总委托笔数, 
            double              BuyPrice07;                 ///< 申买价, 
            unsigned long long  BuyVolume07;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo07;          ///< 买入总委托笔数, 
            double              BuyPrice08;                 ///< 申买价, 
            unsigned long long  BuyVolume08;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo08;          ///< 买入总委托笔数, 
            double              BuyPrice09;                 ///< 申买价, 
            unsigned long long  BuyVolume09;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo09;          ///< 买入总委托笔数, 
            double              BuyPrice10;                 ///< 申买价, 
            unsigned long long  BuyVolume10;                ///< 申买量, 
            unsigned int        TotalBuyOrderNo10;          ///< 买入总委托笔数, 
        };
    };
    unsigned int        BuyLevelQueueNo01;                  ///< 买一档揭示委托笔数, 为 0 表示不揭示
    unsigned int        BuyLevelQueue[ORDER_LEVEL_FIFTY];   ///< 买1档队列, 高频数据保存，先存订单数量,只有一档有此数据，50档，不足时补空
};

/// 上交所L2指数行情
struct SSEL2_Index
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                   ///< 数据生成时间(毫秒), 143025000 表示 14:30:25000
    int                 TradeTime;              ///< 成交时间(毫秒),
    char                Symbol[SYMBOL_LEN];     ///< 证券代码, 
    double              PreClosePrice;          ///< 昨收价, 
    double              OpenPrice;              ///< 开盘价, 
    double              TotalAmount;            ///< 成交总额, 
    double              HighPrice;              ///< 最高价, 
    double              LowPrice;               ///< 最低价, 
    double              LastPrice;              ///< 现价, 
    unsigned long long  TotalVolume;            ///< 成交总量, 手
    double              ClosePrice;             ///< 今收盘价, 
};

/// 上交所L2逐笔成交
struct SSEL2_Transaction
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 TradeTime;              ///< 成交时间(毫秒), 14302506 表示14:30:25.06
    unsigned int        RecID;                  ///< 业务索引, 从 1 开始，按 TradeChannel连续
    int                 TradeChannel;           ///< 成交通道, 
    char                Symbol[SYMBOL_LEN];     ///< 证券代码, 
    double              TradePrice;             ///< 成交价格, 
    unsigned int        TradeVolume;            ///< 成交数量, 股票：股;权证：份;债券：张
    double              TradeAmount;            ///< 成交金额, 
    long long           BuyRecID;               ///< 买方订单号, 
    long long           SellRecID;              ///< 卖方订单号, 
    char                BuySellFlag;            ///< 内外盘标志, B – 外盘,主动买;S – 内盘,主动卖;N – 未知
};

/// 上交所L2虚拟集合竞价
struct SSEL2_Auction
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                   ///< 数据生成时间, 143025001 表示 14:30:25.001
    char                Symbol[SYMBOL_LEN];     ///< 证券代码, 
    double              OpenPrice;              ///< 虚拟开盘参考价, 
    long long           AuctionVolume;          ///< 虚拟匹配量, 
    long long           LeaveVolume;            ///< 虚拟未匹配量, 
    char                Side;                   ///< 买卖方向, 0=无未匹配量，买卖两边的未匹配量都为 0;1=买方有未匹配量，卖方未匹配量=0;2=卖方有未匹配量，买方未匹配量=0
};

/// 上交所L2市场总览
struct SSEL2_Overview
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                   ///< 数据生成时间, 143025001 表示 14:30:25.001
    char                Symbol[SYMBOL_LEN];     ///< 证券代码, 
    int                 MarketTime;             ///< 市场时间, 百分之一秒
    int                 TradeDate;              ///< 市场日期, 
};

/// 上交所个股期权静态数据
struct SSEIOL1_Static
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 数据包头时间YYYYMMDDHHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码（合约代码）, 合约期权产品代码，8位字符;唯一标示
    char                ContractID [19];                    ///< 合约交易代码 , 合约交易代码17位，按以下顺序编写：1、第1至第6位为数字，取标的证券代码，如工商银行601398，取“601398”；2、第7位为C(Call)或者P(Put)，分别表示认购期权或者认沽期权；3、第8、9位表示到期年份；4、第10、11位表示到期月份；5、第12位期初设为“M”，表示月份合约。当合约首次调整后，“M”修改为 “A”，以表示该合约被调整过一次，如发生第二次调整，则“A”修改为“B”、“M”修改为 “A”，以此类推；6、第13至17位表示期权行权价格.留两位备用
    char                SecurityName[SECURITY_NAME_LEN];    ///< 期权合约简称（证券名称） , 
    char                SymbolUnderlying[SYMBOL_LEN];       ///< 基础证券代码, 
    char                NameUnderlying[SECURITY_NAME_LEN];  ///< 基础证券名称 , 
    char                UnderlyingType [5];                 ///< 标的证券类型 , EBS – ETF， ASH – A股 
    char                OptionType;                         ///< 行权类型, 若为欧式期权，则本字段为“E”；若为美式期权，则本字段为“A” 
    char                CallOrPut;                          ///< 认购认沽 , 认购，则本字段为“C”；若为认沽，则本字段为“P” 
    long long           ContractMultiplierUnit;             ///< 合约单位 , 经过除权除息调整后的合约单位, 一定是整数 
    double              ExercisePrice;                      ///< 期权行权价 , 经过除权除息调整后的期权行权价，右对齐，精确到厘;行权价格调整采取四舍五入方式，股票保留两位小数，ETF保留三位小数。
    int                 StartDate;                          ///< 首个交易日 , 期权首个交易日,YYYYMMDD (年*10000+月*100+日)
    int                 EndDate;                            ///< 最后交易日 , 期权最后交易日/行权日，YYYYMMDD 
    int                 ExerciseDate;                       ///< 期权行权日 , 期权行权日，YYYYMMDD 
    int                 DeliveryDate;                       ///< 行权交割日, 行权交割日，默认为行权日的下一个交易日，YYYYMMDD
    int                 ExpireDate;                         ///< 期权到期日 , 期权到期日，YYYYMMDD 
    char                Version;                            ///< 合约版本号 , 期权合约的版本号。新挂合约是’1’ 
    unsigned int        TotalLongPosition;                  ///< 合约未平仓数 , 单位是 （张） 此值为日终持仓轧差对冲之后持仓数据。
    double              PreClosePrice;                      ///< 昨收价, 昨日收盘价，如遇除权除息则为调整后的收盘价格（上市首日的文件中，填写参考价格），右对齐，精确到厘 
    double              PreSettlePrice;                     ///< 昨结算, 昨日结算价，右对齐，单位：元 
    double              PreClosePriceUnderlying;            ///< 标的昨收价, 期权标的证券除权除息调整后的前收盘价格，右对齐，单位：元 
    char                UpDownLimitType;                    ///< 涨跌幅限制类型 , ‘N’表示交易规则3.4.13规定的有涨跌幅限制类型或者权证管理办法第22条规定 ‘R’表示交易规则3.4.15和3.4.16规定的无涨跌幅限制类型 
    double              PriceUpLimit;                       ///< 涨幅价格, 当日期权涨停价格，精确到厘 
    double              PriceDownLimit;                     ///< 跌幅价格, 当日期权跌停价格，精确到厘 
    double              MarginUnit;                         ///< 单位保证金 , 当日持有一张合约所需要的保证金数 量，精确到分 
    double              MarginRatioParam1;                  ///< 保证金计算参数一 , 保证金计算参数，单位：% 
    double              MarginRatioParam2;                  ///< 保证金计算参数二 , 保证金计算参数，单位：% 
    unsigned int        RoundLot;                           ///< 整手数 , 一手等于几张合约 
    long long           LmtOrdFloor;                        ///< 限价申报下限 , 单笔限价申报的申报张数下限。 
    long long           LmtOrdCeiling;                      ///< 限价申报上限 , 单笔限价申报的申报张数上限。 
    long long           MktOrdFloor;                        ///< 市价申报下限 , 单笔市价申报的申报张数下限。 
    long long           MktOrdCeiling;                      ///< 市价申报上限 , 单笔市价申报的申报张数上限。 
    double              TickSize;                           ///< 最小报价单位, 单位：元（精确到0.1厘）
    char                SecurityStatusFlag[8];              ///< 期权合约状态, 该字段为8位字符串，左起每位表示特定的含义，无定义则填空格。
                                                            ///< 第1位：‘0’表示可开仓，‘1’表示限制卖出开仓（包括备兑开仓）和买入开仓。
                                                            ///< 第2位：‘0’表示未连续停牌，‘1’表示连续停牌。
                                                            ///< 第3位：‘0’表示未临近到期日，‘1’表示距离到期日不足5个交易日。
                                                            ///< 第4位：‘0’表示近期未做调整，‘1’表示最近5个交易日内合约发生过调整。
                                                            ///< 第5位：‘A’表示当日新挂牌的合约，‘E’表示存续的合约，‘D’表示当日摘牌的合约。
                                                            ///< 第6位：‘1’表示合约只能进行跨式和宽跨式的组合策略，‘0’表示可以进行所有的组合策略
    int                 AutoSplitDate;                      ///< 该日期表示垂直价差组合策略到期解除的日期，YYYYMMDD； 对于E-2后波动加挂的合约，此字段仍为E-2日。到期日调整时，此字段信息自动同步调整。
    int                 TradeDate;                          ///< 市场日期,交易日

};

/// 上交所个股期权实时行情
struct SSEIOL1_Quotation
{
    int                 LocalTimeStamp;         ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;        ///< 数据包头时间YYYYMMDDHHMMSSMMM
    int                 Time;                   ///< 数据生成时间, 143025001表示 14:30:25.001;如果期权合约的产品代码为“00000000”，则表示行情时间，格式为HHMMSSMMM;有成交时才会变化
    char                Symbol[SYMBOL_LEN];     ///< 合约代码（证券代码）, 期权合约的产品代码;唯一标示
    double              PreSettlePrice;         ///< 昨结算, 4 decimal places
    double              SettlePrice;            ///< 结算价, 4 decimal places
    double              OpenPrice;              ///< 开盘价, 4 decimal places
    double              HighPrice;              ///< 最高价 , 4 decimal places
    double              LowPrice;               ///< 最低价, 4 decimal places
    double              LastPrice;              ///< 现价, 4 decimal places
    double              ClosePrice;             ///< 收盘价
    double              AuctionPrice;           ///< 波动性中断参考价, 4 decimal places
    long long           AuctionVolume;          ///< 波动性中断虚拟匹配量, 
    unsigned long long  TotalPosition;          ///< 持仓量, 单位是 （张），交易时间代表此处总持仓量值为持仓余额轧差之前的数值;收盘后为闭市轧差后的未平仓合约数量。
    unsigned int        SellLevelNo;            ///< 卖盘价位数量, 五档:总是5，空档位用0填充
    union
    {
        BuySellLevelInfo    SellLevel[LEVEL_FIVE];      ///< 五档卖行情
        struct  
        {
            double              SellPrice01;            ///< 申卖价, 当10069<5时，补足5  档，即：不知的填充为0
            unsigned long long  SellVolume01;           ///< 申卖量, 当10069<5时，补足5  档，即：不知的填充为0
            double              SellPrice02;            ///< 申卖价, 当10069<5时，补足5  档，即：不知的填充为0
            unsigned long long  SellVolume02;           ///< 申卖量, 当10069<5时，补足5  档，即：不知的填充为0
            double              SellPrice03;            ///< 申卖价, 当10069<5时，补足5  档，即：不知的填充为0
            unsigned long long  SellVolume03;           ///< 申卖量, 当10069<5时，补足5  档，即：不知的填充为0
            double              SellPrice04;            ///< 申卖价, 当10069<5时，补足5  档，即：不知的填充为0
            unsigned long long  SellVolume04;           ///< 申卖量, 当10069<5时，补足5  档，即：不知的填充为0
            double              SellPrice05;            ///< 申卖价, 当10069<5时，补足5  档，即：不知的填充为0
            unsigned long long  SellVolume05;           ///< 申卖量, 当10069<5时，补足5  档，即：不知的填充为0
        };
    };
    unsigned int            BuyLevelNo;                  ///< 买盘价位数量, 五档:总是5，空档位用0填充
    union
    {
        BuySellLevelInfo    BuyLevel[LEVEL_FIVE];        ///< 五档买行情
        struct  
        {
            double              BuyPrice01;             ///< 申买价, 当10068<5时，补足5档，即：不知的填充为0
            unsigned long long  BuyVolume01;            ///< 申买量, 当10068<5时，补足5档，即：不知的填充为0
            double              BuyPrice02;             ///< 申买价, 当10068<5时，补足5档，即：不知的填充为0
            unsigned long long  BuyVolume02;            ///< 申买量, 当10068<5时，补足5档，即：不知的填充为0
            double              BuyPrice03;             ///< 申买价, 当10068<5时，补足5档，即：不知的填充为0
            unsigned long long  BuyVolume03;            ///< 申买量, 当10068<5时，补足5档，即：不知的填充为0
            double              BuyPrice04;             ///< 申买价, 当10068<5时，补足5档，即：不知的填充为0
            unsigned long long  BuyVolume04;            ///< 申买量, 当10068<5时，补足5档，即：不知的填充为0
            double              BuyPrice05;             ///< 申买价, 当10068<5时，补足5档，即：不知的填充为0
            unsigned long long  BuyVolume05;            ///< 申买量, 当10068<5时，补足5档，即：不知的填充为0
        };
    };
    unsigned long long  TotalVolume;            ///< 成交总量, Trade volume of this security.如果期权合约的产品代码为“00000000”，则表示行情日期，格式为YYYYMMDD
    double              TotalAmount;            ///< 成交金额, 2 decimal places
    char                SecurityPhaseTag[PHRASE_TAG_LEN];    ///< 该字段为4位字符串，左起每位表示特定的含义，无定义则填空格。
                                                             ///< 第1位：‘S’表示启动（开市前）时段，‘C’表示集合竞价时段，‘T’表示连续交易时段，‘B’表示休市时段，‘E’表示闭市时段，‘V’表示波动性中断，‘P’表示临时停牌、‘U’表示收盘集合竞价。
                                                             ///<        ‘M’表示可恢复交易的熔断（盘中集合竞价），‘N’表示不可恢复交易的熔断（暂停交易至闭市）
                                                             ///< 第2位：‘0’表示未连续停牌，‘1’表示连续停牌。（预留，暂填空格）。
                                                             ///< 第3位：‘0’表示不限制开仓，‘1’表示限制备兑开仓，‘2’表示卖出开仓，‘3’表示限制卖出开仓、备兑开仓，‘4’表示限制买入开仓，‘5’表示限制买入开仓、备兑开仓，‘6’表示限制买入开仓、卖出开仓，‘7’表示限制买入开仓、卖出开仓、备兑开仓。
                                                             ///< 第4位：‘0’表示此产品在当前时段不接受进行新订单申报，‘1’表示此产品在当前时段可接受进行新订单申报。

};

/// 集合竞价涨跌幅
struct AuctionUpDown{
    double          LimitUpRate;                     ///< 集合竞价上涨幅度，3位小数
    double          LimitDownRate;                   ///< 集合竞价下跌幅度，3位小数
    double          LimitUpAbsolute;                 ///< 集合竞价上涨限价，4位小数
    double          LimitDownAbsolute;               ///< 集合竞价下跌限价，4位小数
    double          AuctionUpDownRate;               ///< 集合竞价有效范围涨跌幅度，3位小数
    double          AuctionUpDownAbsolute;           ///< 集合竞价有效范围涨跌价格，4位小数
};

/// 深交所L1实时行情
struct SZSEL1_Quotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志. 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           Time;                               ///< 数据生成时间YYYYMMDDHHMMSSsss
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码. 证券代码以395打头的记录为成交量统计指标记录
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源，102 = 深圳证券交易所,103=香港交易所
    double              PreClosePrice;                      ///< 昨收价.
    double              OpenPrice;                          ///< 开盘价.
    double              LastPrice;                          ///< 现价.
    double              HighPrice;                          ///< 最高价. 
    double              LowPrice;                           ///< 最低价.
    double              PriceUpLimit;                       ///< 涨停价.
    double              PriceDownLimit;                     ///< 跌停价.
    double              PriceUpdown1;                       ///< 升跌一
    double              PriceUpdown2;                       ///< 升跌二
    unsigned long long  TotalNo;                            ///< 总成交笔数. 证券代码以395打头的记录为成交量统计指标记录，该字段表示各类别对应的成交笔数
    double              TotalVolume;                        ///< 成交总量. 证券代码以395打头的记录为成交量统计指标记录，该字段表示各类别对应的成交单位数
    double              TotalAmount;                        ///< 成交总额. 证券代码以395打头的记录为成交量统计指标记录，该字段表示各类别对应的成交金额。所有单位均为人民币。
    double              ClosePrice;                         ///< 今收盘价 盘前、盘中赋为0值；盘后，由闭市后发的静态数据：现货证券收盘行情(cashsecurityclosemd.xml)进行填充，按证券代码进行填充。无可取值的代码，则赋0值
    char                SecurityPhaseTag[8];                ///< 当前品种交易状态. 产品所处的交易阶段代码：
                                                            ///<  第 0 位：S=启动(开市前) O=开盘集合竞价 T=连续     B=休市
                                                            ///<  C=收盘集合竞价 E=已闭市 H=临时停牌 A=盘后交易
                                                            ///<  V=波动性中断
                                                            ///<  第 1 位：0=正常状态 1=全天停牌
    double              PERatio1;                           ///< 市盈率1.
    double              PERatio2;                           ///< 市盈率2.
    double              NAV;                                ///< 基金 T-1 日净值
    double              IOPV;                               ///< 基金实时参考净值(包括 ETF的 IOPV)
    double              PremiumRate;                        ///< 权证溢价率
    unsigned int        SampleNo;                           ///< 样本个数    
    union
    {
        SZSE_BuySellLevelInfo    SellLevel[LEVEL_FIVE];      ///< 五档卖行情
        struct  
        {
            double              SellPrice01;            ///< 申卖价一
            double              SellVolume01;           ///< 申卖量一
            double              SellPrice02;            ///< 申卖价二
            double              SellVolume02;           ///< 申卖量二
            double              SellPrice03;            ///< 申卖价三
            double              SellVolume03;           ///< 申卖量三
            double              SellPrice04;            ///< 申卖价四
            double              SellVolume04;           ///< 申卖量四
            double              SellPrice05;            ///< 申卖价五
            double              SellVolume05;           ///< 申卖量五
        };
    };
    union
    {
        SZSE_BuySellLevelInfo    BuyLevel[LEVEL_FIVE];        ///< 五档买行情
        struct  
        {
            double              BuyPrice01;             ///< 申买价一
            double              BuyVolume01;            ///< 申买量一
            double              BuyPrice02;             ///< 申买价二
            double              BuyVolume02;            ///< 申买量二
            double              BuyPrice03;             ///< 申买价三
            double              BuyVolume03;            ///< 申买量三
            double              BuyPrice04;             ///< 申买价四
            double              BuyVolume04;            ///< 申买量四
            double              BuyPrice05;             ///< 申买价五
            double              BuyVolume05;            ///< 申买量五
        };
    };

    double              WtAvgRate;                          ///< 实时加权平均利率    质押式回购产品实时行情增加三个字段
    double              WtAvgRateUpdown;                    ///< 加权平均利率涨跌BP
    double              PreWtAvgRate;                       ///< 昨收盘加权平均利率
};

/// 深交所公告消息头
struct SZSEL1_BulletinHead
{
    int             LocalTimeStamp;                     ///< 数据接收时间
    char            QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志，第1位对应：’1’表示国上海南汇机房行情源，’2’表示深圳福永机房行情源
    long long       Time;                               ///< 数据生成时间YYYYMMDDHHMMSSsss
    unsigned int    SetID;                              ///< 频道代码
    char            NewsID[12];                         ///< 公告标识, 空串代表公告摘要, 会重复发送. 通过摘要可确定公告是否有遗漏
    char            Headline[128];                      ///< 公告标题
    char            RawDataFormat[8];                   ///< 二进制数据格式
    unsigned int    RawDataLength;                      ///< 二进制数据长度
};

/// 深交所公告消息
struct SZSEL1_Bulletin
{
    SZSEL1_BulletinHead RawHead;                        ///< 公告头
    char                RawData[1];                     ///< 二进制数据,长度由RawDataLength确定
};

/// 深交所L2静态数据
struct SZSEL2_Static
{
    int             LocalTimeStamp;                     ///< 数据接收时间  格式为：HHMMSSsss
    char            QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志，第1位对应：’1’表示国上海南汇机房行情源，’2’表示深圳福永机房行情源
    char            Symbol[SYMBOL_LEN];                 ///< 证券代码，8个字符
    char            SecurityName[40];                   ///< 证券名称，可能包含中文字符，表示最多 40个 UTF-8 字符
    char            SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源，102 = 深圳证券交易所
    char            SecurityEN[40];                     ///< 证券英文简称
    char            ISINCode[12];                       ///< ISIN 代码
    char            SymbolUnderlying[20];               ///< 基础证券代码
    char            UnderlyingSecurityIDSource[4];      ///< 基础证券代码源
    int             SecurityType;                       ///< 证券类别代码，1主板 A股； 2中小板股票；3创业板股票；4主板 B股；5国债 （含地方债）；6企业债；
                                                        ///< 7公司债；8可转债；9私募债；10可交换私募债；11证券公司次级债；12质押式回购；13资产支持证券；
                                                        ///< 14本市场股票ETF；15跨市场ETF；16跨境ETF；17本市场实物债券ETF；18现金债券ETF；19黄金ETF；20货币ETF；
                                                        ///< 21（预留）杠杆ETF;22(预留)商品期货ETF；23标准LOF;24分级子基金；25封闭式基金；26仅申赎基金；28权证；
                                                        ///< 29个股期权；30ETF期权；33优先股；34证券公司短期债；35可交换公司债
    char            SecurityStatusTag[20];              ///< 证券状态标识，该字段为20位字符串（后六位备用），
                                                        ///< 每位表示特定的含义，“1”表示位数有业务意义，“0”表示该位数无业务意义。第1位对应：“1”表示停牌；
                                                        ///< 第2位对应：“1”表示除权；第3位对应：“1”表示除息；第4位对应：“1”表示ST;第5位对应：“1”表示*ST;
                                                        ///< 第6位对应：“1”表示上市首日；第7位对应：“1”表示公司再融资；第8位对应：“1”表示恢复上市首日；
                                                        ///< 第9位对应：“1”表示网络投票；第10位对应：“1”表示退市整理期；第12位对应：“1”表示增发股份上市；
                                                        ///< 第13位对应：“1”表示合约调整；第14位对应：“1”表示暂停上市后协议转让。
    double          PreClosePrice;                      ///< 昨日收盘价，4位或5位小数
    int             ListingDate;                        ///< 上市日期
    char            Currency[5];                        ///< 币种，CNY = 人民币，HKD = 港币
    double          ParValue;                           ///< 每股面值，4位小数
    double          IssuedVolume;                       ///< 总发行量，2位小数
    double          OutstandingShare;                   ///< 流通股数，2位小数
    char            IndustryType[5];                    ///< 行业种类
    double          PreYearEPS;                         ///< 上年每股利润，4位小数
    double          YearEPS;                            ///< 本年每股利润，4位小数
    char            OfferingFlag;                       ///< 收购（转股、行权）标志，股票：要约收购；债券、优先股：转股回售；Y=是，N=否
    double          NAV;                                ///< 基金T-1日累计净值，4位小数
    double          CouponRate;                         ///< 票面利率，4位小数
    double          IssuePrice;                         ///< 贴现发行价，4位小数
    double          Interest;                           ///< 每百元应计利息，8位小数，优先股：0.0000 表示浮动股息率
    int             InterestAccrualDate;                ///< 起息日
    int             MaturityDate;                       ///< 到期交割日
    double          ConversionPrice;                    ///< 行权价格，4位小数
    double          ConversionRatio;                    ///< 行权比例，4位小数
    int             ConversionBeginDate;                ///< 行权开始日
    int             ConversionEndDate;                  ///< 行权结束日
    char            CallOrPut;                          ///< 认购认沽，C = Call，P = Put
    char            WarrantClearingType;                ///< 权证结算方式，S = 证券结算，C = 现金结算
    double          ClearingPrice;                      ///< 结算价格，适用于权证，4位小数
    char            OptionType;                         ///< 行权类型，A = 美式，E = 欧式，B = 百慕大式
    int             EndDate ;                           ///< 最后交易日
    int             ExpirationDays;                     ///< 购回期限
    char            DayTrading;                         ///< 回转交易标志，是否支持当日回，转交易：Y=支持，N=不支持
    char            GageFlag;                           ///< 保证金证券标志，是否可作为融资，融券可充抵保证，金证券：Y=是，N=否
    double          GageRate;                           ///< 担保品折算率，2位小数
    char            CrdBuyUnderlying;                   ///< 融资标的标志，Y=是，N=否
    char            CrdSellUnderlying;                  ///< 融券标的标志，Y=是，N=否
    char            CrdPriceCheckType[2];               ///< 提价检查方式，0=不检查，1=不低于最近成交价，2=不低于昨收价，3=不低于最高叫买，4=不低于最低叫卖
    char            PledgeFlag;                         ///< 质押入库标志，是否可质押入库:Y=是，N=否
    double          ContractMultiplier;                 ///< 债券折合回购标准券比例，4位小数
    char            RegularShare[8];                    ///< 对应回购标准券
    char            QualificationFlag;                  ///< 投资者适当性管理标志，是否需要对该证券作投资者适当性管理，Y=是，N=否
    char            MarketMakerFlag;                    ///< 做市商标志，标识是否有做市商，Y=是，N=否
    double          RoundLot;                           ///< 整手数(2位有效小数)，对于某一证券申报的委托，其委托数量字段必须为该证券数量单位的整数倍
    double          TickSize;                           ///< 最小报价单位，4位小数
    double          BuyQtyUpperLimit;                   ///< 买数量上限，2位小数
    double          SellQtyUpperLimit;                  ///< 卖数量上限，2位小数
    double          BuyVolumeUnit;                      ///< 买数量单位，每笔买委托的委托数量必须是买数量单位的整数倍，2位小数
    double          SellVolumeUnit;                     ///< 卖数量单位，每笔卖委托的委托数量必须是卖数量单位的整数倍，2位小数
    union{
        AuctionUpDown OTC[OTC_ELEMENT_LEN];
        struct  
        {
            double          LimitUpRateO;                     ///< 开盘集合竞价上涨幅度，3位小数
            double          LimitDownRateO;                   ///< 开盘集合竞价下跌幅度，3位小数
            double          LimitUpAbsoluteO;                 ///< 开盘集合竞价上涨限价，4位小数
            double          LimitDownAbsoluteO;               ///< 开盘集合竞价下跌限价，4位小数
            double          AuctionUpDownRateO;               ///< 开盘集合竞价有效范围涨跌幅度，3位小数
            double          AuctionUpDownAbsoluteO;           ///< 开盘集合竞价有效范围涨跌价格，4位小数
            double          LimitUpRateT;                     ///< 连续竞价上涨幅度，3位小数
            double          LimitDownRateT;                   ///< 连续竞价下跌幅度，3位小数
            double          LimitUpAbsoluteT;                 ///< 连续竞价上涨限价，4位小数
            double          LimitDownAbsoluteT;               ///< 连续竞价下跌限价，4位小数
            double          AuctionUpDownRateT;               ///< 连续竞价有效范围涨跌幅度，3位小数
            double          AuctionUpDownAbsoluteT;           ///< 连续竞价有效范围涨跌价格，4位小数
            double          LimitUpRateC;                     ///< 收盘集合竞价上涨幅度，3位小数
            double          LimitDownRateC;                   ///< 收盘集合竞价下跌幅度，3位小数
            double          LimitUpAbsoluteC;                 ///< 收盘集合竞价上涨限价，4位小数
            double          LimitDownAbsoluteC;               ///< 收盘集合竞价下跌限价，4位小数
            double          AuctionUpDownRateC;               ///< 收盘集合竞价有效范围涨跌幅度，3位小数
            double          AuctionUpDownAbsoluteC;           ///< 收盘集合竞价有效范围涨跌价格，4位小数
        };
    };
    int             TradeDate;                          ///< 市场日期,交易日
    char            QualificationClass[2];              ///< 投资者适当性管理分类

};

typedef SZSEL2_Static SZSEL1_Static;

/// 深交所L2证券状态
struct SZSEL2_Status
{
    int         LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char        QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志
    char        Symbol[SYMBOL_LEN];                 ///< 证券代码
    char        SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源，102=深圳证券交易所，103=香港交易所
    long long   Time;                               ///< 数据生成时间YYYYMMDDHHMMSSMMM
    char        FinancialStatus[8];                 ///< A=上市公司早间披露提示,
                                                    ///< B=上市公司午间披露提示,
                                                    ///< 每个字节揭示一种状态，最多可同时揭示八种状态
    char        CrdBuyStatus;                       ///< 适用于融资标的证券。1= 开放,0=暂停，空格无意义
    char        CrdSellStatus;                      ///< 适用于融券标的证券。1= 开放,0=暂停，空格无意义
    char        SubscribeStatus;                    ///< 适用于 ETF， LOF 等开放式基金，对于黄金 ETF 是指现金申购。1= 是,0= 否，空格无意义
    char        RedemptionStatus;                   ///< 适用于 ETF， LOF 等开放式基金，对于黄金 ETF 是指现金赎回开关。1= 是,0= 否，空格无意义
    char        PurchasingStatus;                   ///< 适用于网上发行认购代。1= 是,0= 否，空格无意义
    char        StockDiviStatus;                    ///< 适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    char        PutableStatus;                      ///< 适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    char        ExerciseStatus;                     ///< 适用于处于行权期的权证。1= 是,0=否，空格无意义
    char        GoldPurchase;                       ///< 适用于黄金ETF实物申购。1= 是,0= 否，空格无意义
    char        GoldRedemption;                     ///< 适用于黄金ETF实物赎回。1= 是,0= 否，空格无意义
    char        AcceptedStatus;                     ///< 适用于处于要约收购期的股票。1= 是,0= 否，空格无意义
    char        ReleaseStatus;                      ///< 适用于处于要约收购期的股票。1= 是,0= 否，空格无意义
    char        CancStockDiviStatus;                ///< 适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    char        CancPutableStatus;                  ///< 适用于处于转股回售期的可转债。1= 是,0= 否，空格无意义
    char        PledgeStatus;                       ///< 适用于质押式回购可质押入库证券。1= 是,0= 否，空格无意义
    char        RemovePledge;                       ///< 适用于质押式回购可质押入库证券。1= 是,0= 否，空格无意义
    char        VoteStatus;                         ///< 适用于优先股。1= 是,0= 否，空格无意义
    char        StockPledgeRepo;                    ///< 适用于可开展股票质押式回购业务的证券。1= 是,0= 否，空格无意义
    char        DivideStatus;                       ///< 适用于分级基金。1= 是,0= 否，空格无意义
    char        MergerStatus;                       ///< 适用于分级基金。1= 是,0= 否，空格无意义
};

/// 深交所L2实时行情
struct SZSEL2_Quotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           Time;                               ///< 数据生成时间YYYYMMDDHHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源
    double              PreClosePrice;                      ///< 昨日收盘价,4位小数
    double              OpenPrice;                          ///< 开盘价,6位小数
    double              LastPrice;                          ///< 现价,6位小数
    double              HighPrice;                          ///< 最高价,6位小数
    double              LowPrice;                           ///< 最低价,6位小数
    double              PriceUpLimit;                       ///< 涨停价,6位小数
    double              PriceDownLimit;                     ///< 跌停价,6位小数
    double              PriceUpdown1;                       ///< 升跌一,6位小数
    double              PriceUpdown2;                       ///< 升跌二,6位小数
    unsigned long long  TotalNo;                            ///< 成交笔数
    double              TotalVolume;                        ///< 成交总量,2位小数
    double              TotalAmount;                        ///< 成交总金额,4位小数
    double              ClosePrice;                         ///< 今收盘价，6位小数
    char                SecurityPhaseTag[PHRASE_TAG_LEN];   ///< 当前品种交易状态：产品所处的交易阶段代码：
                                                            ///< 第 0 位：S=启动（开市前），O=开盘集合竞价， T=连续,B=休市
                                                            ///<          C=收盘集合竞价,E=已闭市,H=临时停牌,A=盘后交易,V=波动性中断;
                                                            ///< 第 1 位：0=正常状态,1=全天停牌"
    double              PERatio1;                           ///< 市盈率 1,6位小数
    double              NAV;                                ///< 基金 T-1 日净值,6位小数
    double              PERatio2;                           ///< 市盈率 2,6位小数
    double              IOPV;                               ///< 基金实时参考净值,6位小数
    double              PremiumRate;                        ///< 权证溢价率,6位小数
    double              TotalSellOrderVolume;               ///< 委托卖出总量（有效竞价范围内）2位小数
    double              WtAvgSellPrice;                     ///< 加权平均卖出价格（有效竞价范围内）,6位小数
    unsigned int        SellLevelNo;                        ///< 申卖档位数（价格由低至高）
    union
    {
        SZSE_BuySellLevelInfo3    SellLevel[LEVEL_TEN];      ///< 十档卖行情
        struct
        {
            double              SellPrice01;            ///< 申卖价,6位小数
            double              SellVolume01;           ///< 申卖量，2位小数
            unsigned long long  TotalSellOrderNo01;     ///< 卖价位总委托笔数
            double              SellPrice02;            ///< 申卖价
            double              SellVolume02;           ///< 申卖量
            unsigned long long  TotalSellOrderNo02;     ///< 卖价位总委托笔数
            double              SellPrice03;            ///< 申卖价
            double              SellVolume03;           ///< 申卖量
            unsigned long long  TotalSellOrderNo03;     ///< 卖价位总委托笔数
            double              SellPrice04;            ///< 申卖价
            double              SellVolume04;           ///< 申卖量
            unsigned long long  TotalSellOrderNo04;     ///< 卖价位总委托笔数
            double              SellPrice05;            ///< 申卖价
            double              SellVolume05;           ///< 申卖量
            unsigned long long  TotalSellOrderNo05;     ///< 卖价位总委托笔数
            double              SellPrice06;            ///< 申卖价
            double              SellVolume06;           ///< 申卖量
            unsigned long long  TotalSellOrderNo06;     ///< 卖价位总委托笔数
            double              SellPrice07;            ///< 申卖价
            double              SellVolume07;           ///< 申卖量
            unsigned long long  TotalSellOrderNo07;     ///< 卖价位总委托笔数
            double              SellPrice08;            ///< 申卖价
            double              SellVolume08;           ///< 申卖量
            unsigned long long  TotalSellOrderNo08;     ///< 卖价位总委托笔数
            double              SellPrice09;            ///< 申卖价
            double              SellVolume09;           ///< 申卖量
            unsigned long long  TotalSellOrderNo09;     ///< 卖价位总委托笔数
            double              SellPrice10;            ///< 申卖价
            double              SellVolume10;           ///< 申卖量
            unsigned long long  TotalSellOrderNo10;     ///< 卖价位总委托笔数
        };
    };
    unsigned int        SellLevelQueueNo01;                 ///< 卖一档揭示委托笔数
    double              SellLevelQueue[ORDER_LEVEL_FIFTY];  ///< 揭示卖一价前50笔委托，50档，不足补0，2位小数
    double              TotalBuyOrderVolume;                ///< 委托买入总量（有效竞价范围内），2位小数
    double              WtAvgBuyPrice;                      ///< 加权平均买入价格（有效竞价范围内）,6位小数
    unsigned int        BuyLevelNo;                         ///< 申买档位数（价格由高至低）
    union
    {
        SZSE_BuySellLevelInfo3    BuyLevel[LEVEL_TEN];      ///< 十档买行情
        struct
        {
            double              BuyPrice01;             ///< 申买价,6位小数
            double              BuyVolume01;            ///< 申买量,2位小数
            unsigned long long  TotalBuyOrderNo01;      ///< 买价位总委托笔数
            double              BuyPrice02;             ///< 申买价
            double              BuyVolume02;            ///< 申买量
            unsigned long long  TotalBuyOrderNo02;      ///< 买价位总委托笔数
            double              BuyPrice03;             ///< 申买价
            double              BuyVolume03;            ///< 申买量
            unsigned long long  TotalBuyOrderNo03;      ///< 买价位总委托笔数
            double              BuyPrice04;             ///< 申买价
            double              BuyVolume04;            ///< 申买量
            unsigned long long  TotalBuyOrderNo04;      ///< 买价位总委托笔数
            double              BuyPrice05;             ///< 申买价
            double              BuyVolume05;            ///< 申买量
            unsigned long long  TotalBuyOrderNo05;      ///< 买价位总委托笔数
            double              BuyPrice06;             ///< 申买价
            double              BuyVolume06;            ///< 申买量
            unsigned long long  TotalBuyOrderNo06;      ///< 买价位总委托笔数
            double              BuyPrice07;             ///< 申买价
            double              BuyVolume07;            ///< 申买量
            unsigned long long  TotalBuyOrderNo07;      ///< 买价位总委托笔数
            double              BuyPrice08;             ///< 申买价
            double              BuyVolume08;            ///< 申买量
            unsigned long long  TotalBuyOrderNo08;      ///< 买价位总委托笔数
            double              BuyPrice09;             ///< 申买价
            double              BuyVolume09;            ///< 申买量
            unsigned long long  TotalBuyOrderNo09;      ///< 买价位总委托笔数
            double              BuyPrice10;             ///< 申买价
            double              BuyVolume10;            ///< 申买量
            unsigned long long  TotalBuyOrderNo10;      ///< 买价位总委托笔数
        };
    };
    unsigned int        BuyLevelQueueNo01;                  ///< 买一档揭示委托笔数
    double              BuyLevelQueue[ORDER_LEVEL_FIFTY];   ///< 揭示买一价前50笔委托，50档，不足补0，2位小数

    double              WtAvgRate;                          ///< 实时加权平均利率    质押式回购产品实时行情增加三个字段
    double              WtAvgRateUpdown;                    ///< 加权平均利率涨跌BP
    double              PreWtAvgRate;                       ///< 昨收盘加权平均利率

};

/// 深交所L2指数行情
struct SZSEL2_Index
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           Time;                               ///< 数据生成时间YYYYMMDDHHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 102=深圳证券交易所, 103=香港交易所
    double              PreClosePrice;                      ///< 前收盘指数,6位小数
    double              OpenPrice;                          ///< 今开盘指数,6位小数
    double              HighPrice;                          ///< 最高指数,6位小数
    double              LowPrice;                           ///< 最低指数,6位小数
    double              LastPrice;                          ///< 最新指数,6位小数
    double              TotalAmount;                        ///< 参与计算相应指数的成交金额,4位小数
    unsigned long long  TotalNo;                            ///< 参与计算相应指数的成交笔数
    double              TotalVolume;                        ///< 参与计算相应指数的交易数量，2位小数
    char                SecurityPhaseTag[PHRASE_TAG_LEN];   ///< 当前品种交易状态
    unsigned int        SampleNo;                           ///< 样本个数
};

/// 深交所L2逐笔委托
struct SZSEL2_Order
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    unsigned int        SetID;                              ///< 频道代码
    unsigned long long  RecID;                              ///< 消息记录号:从 1 开始计数，同一频道连续
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源
    long long           Time;                               ///< 委托时间YYYYMMDDHHMMSSMMM
    double              OrderPrice;                         ///< 委托价格,4位小数
    double              OrderVolume;                        ///< 委托数量,2位小数
    char                OrderCode;                          ///< 买卖方向：1=买 2=卖 G=借入 F=出借
    char                OrderType;                          ///< 订单类别：1=市价 2=限价 U=本方最优
};

/// 深交所L2逐笔成交
struct SZSEL2_Transaction
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    unsigned int        SetID;                              ///< 证券集代号
    unsigned long long  RecID;                              ///< 消息记录号 从 1 开始计数，同一频道连续
    unsigned long long  BuyOrderID;                         ///< 买方委托索引
    unsigned long long  SellOrderID;                        ///< 卖方委托索引
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 证券代码源
    long long           TradeTime;                          ///< 成交时间YYYYMMDDHHMMSSMMM
    double              TradePrice;                         ///< 成交价格,4位小数
    double              TradeVolume;                        ///< 成交数量,2位小数
    char                TradeType;                          ///< 成交类别：4=撤销，主动或自动撤单执行报告；F=成交，成交执行报告
};

/// 中金所L2实时行情
struct CFFEXL2_Quotation
{
    int                 LocalTimeStamp;              ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                   ///< 成交日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                        ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];          ///< 证券代码
    double              PreClosePrice;               ///< 昨收价 指数有效小数位为2位，国债有效小数位为3位
    double              PreSettlePrice;              ///< 昨结算 指数有效小数位为2位，国债有效小数位为3位
    double              OpenPrice;                   ///< 开盘价 指数有效小数位为2位，国债有效小数位为3位
    double              HighPrice;                   ///< 最高价 指数有效小数位为2位，国债有效小数位为3位
    double              LowPrice;                    ///< 最低价 指数有效小数位为2位，国债有效小数位为3位
    double              TradePrice;                  ///< 最新价 指数有效小数位为2位，国债有效小数位为3位
    double              ClosePrice;                  ///< 收盘价 指数有效小数位为2位，国债有效小数位为3位
    double              SettlePrice;                 ///< 结算价 指数有效小数位为2位，国债有效小数位为3位
    double              PriceUpLimit;                ///< 涨停价 指数有效小数位为2位，国债有效小数位为3位
    double              PriceDownLimit;              ///< 跌停价 指数有效小数位为2位，国债有效小数位为3位
    unsigned long long  PreTotalPosition;            ///< 昨持仓量
    unsigned long long  TotalPosition;               ///< 持仓量
    double              PreDelta;                    ///< 昨虚实度 前一日对冲值
    double              Delta;                       ///< 今虚实度 对冲值：期权价格变化/期货价格变化
    char                SettleGroupID[SETTLE_LEN];   ///< 结算组代码
    unsigned int        SettleID;                    ///< 结算编号
    unsigned long long  TotalVolume;                 ///< 总成交量
    double              TotalAmount;                 ///< 总成交额 有效小数位为3位
    union
    {
        BuySellLevelInfo    SellLevel[LEVEL_FIVE];   ///< 五档卖行情
        struct
        {
            double              SellPrice01;         ///< 卖价一 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  SellVolume01;        ///< 卖量一
            double              SellPrice02;         ///< 卖价二 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  SellVolume02;        ///< 卖量二
            double              SellPrice03;         ///< 卖价三 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  SellVolume03;        ///< 卖量三
            double              SellPrice04;         ///< 卖价四 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  SellVolume04;        ///< 卖量四
            double              SellPrice05;         ///< 卖价五 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  SellVolume05;        ///< 卖量五
        };
    };
    union
    {
        BuySellLevelInfo    BuyLevel[LEVEL_FIVE];    ///< 五档买行情
        struct
        {
            double              BuyPrice01;          ///< 买价一 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  BuyVolume01;         ///< 买量一
            double              BuyPrice02;          ///< 买价二 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  BuyVolume02;         ///< 买量二
            double              BuyPrice03;          ///< 买价三 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  BuyVolume03;         ///< 买量三
            double              BuyPrice04;          ///< 买价四 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  BuyVolume04;         ///< 买量四
            double              BuyPrice05;          ///< 买价五 指数有效小数位为2位，国债有效小数位为3位
            unsigned long long  BuyVolume05;         ///< 买量五
        };
    };
};
/// 中金所L2静态数据
typedef CFFEXL2_Quotation CFFEXL2_Static;

/// 郑商所L1实时行情
struct CZCEL1_Quotation
{
    int                 LocalTimeStamp;              ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                   ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                        ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];          ///< 证券代码
    char                Version ;                    ///< 合约版本号，目前为0
    double              PreClosePrice;                      ///< 昨收价    
    double              PreSettlePrice;                     ///< 昨结算
    unsigned long long  PreTotalPosition;                   ///< 昨持仓量
    double              OpenPrice;                          ///< 开盘价
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
    double              LastPrice;                          ///< 最新价
    double              AveragePrice;                       ///< 均价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    double              BuyPrice01;                         ///< 申买价
    unsigned long long  BuyVolume01;                        ///< 申买量
    double              SellPrice01;                        ///< 申卖价
    unsigned long long  SellVolume01;                       ///< 申卖量
    unsigned long long  TotalPosition;                      ///< 持仓量
    unsigned long long  TotalVolume;                        ///< 成交总量
    double              TotalAmount;                        ///< 成交总额
    double              ClosePrice;                         ///< 今收盘价
    double              ClearPrice;                         ///< 当日交割结算价
    double              SettlePrice;                        ///< 结算价
    double              DeriveBidPrice;                     ///< 组合买入价
    double              DeriveAskPrice;                     ///< 组合卖出价
    unsigned long long  DeriveBidLot;                       ///< 组合买入数量
    unsigned long long  DeriveAskLot;                       ///< 组合卖出数量
    unsigned long long  LastLot;                            ///< 最后一笔成交手数
};
/// 郑商所静态数据
typedef CZCEL1_Quotation CZCEL1_Static;

/// 易盛指数行情
struct ESUNNY_Index
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码
    unsigned long long  Time;                               ///< 数据生成时间 
    double              PreClosePrice;                      ///< 昨收价
    double              PreSettlePrice;                     ///< 昨结算
    unsigned long long  PreTotalPosition;                   ///< 昨持仓量
    double              OpenPrice;                          ///< 开盘价
    double              LastPrice;                          ///< 最新价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    unsigned long long  TotalVolume;                        ///< 成交总量
    unsigned long long  TotalPosition;                      ///< 持仓量
    double              AveragePrice;                       ///< 均价
    double              ClosePrice;                         ///< 今收盘价
    double              TotalAmount;                        ///< 成交总额
    double              SettlePrice;                        ///< 结算价
    unsigned long long  LastMatchQty;                       ///< 瞬时成交量
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
};

/// 大商所L1深度行情
struct DCEL1_Quotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码 含期货合约品种、期权合约品种
    unsigned int        RoutineNo;                          ///< 事务编号 该值始终是0，没有用
    char                SecurityName[40];                   ///< 合约名称
    double              PreClosePrice;                      ///< 昨收价    
    double              PreSettlePrice;                     ///< 昨结算
    unsigned long long  PreTotalPosition;                   ///< 昨持仓量
    double              OpenPrice;                          ///< 开盘价
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
    double              LastPrice;                          ///< 最新价
    double              AveragePrice;                       ///< 均价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    unsigned long long  LastMatchQty;                       ///< 最新成交量
    unsigned long long  TotalVolume;                        ///< 成交总量
    double              TotalAmount;                        ///< 成交总额
    unsigned long long  TotalPosition;                      ///< 持仓量
    long long           InterestChg;                        ///< 持仓量变化
    double              BuyPrice01;            ///< 申买价
    unsigned long long  BuyVolume01;           ///< 申买量
    unsigned long long  BidImplyQty01;         ///< 申买推导量
    double              SellPrice01;           ///< 申卖价
    unsigned long long  SellVolume01;          ///< 申卖量
    unsigned long long  AskImplyQty01;         ///< 申卖推导量
    double              ClosePrice;                         ///< 今收盘价
    double              SettlePrice;                        ///< 结算价

    double  Delta;          ///< Delta
    double  Gamma;          ///< Gamma
    double  Rho;            ///< Rho
    double  Theta;          ///< Theta
    double  Vega;           ///< Vega
};

/// 大商所L1静态数据
typedef DCEL1_Quotation DCEL1_Static;

/// 大商所L1套利深度行情
struct DCEL1_ArbiQuotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    unsigned int        RoutineNo;                          ///< 事务编号 该值始终是0，没有用
    char                Symbol[SYMBOL_LEN];                 ///< 套利合约号 格式：套利策略 腿1合约代码&腿2合约代码 套利策略有SP跨期套利，SPC跨品种套利 
    double              LastPrice;                          ///< 最新价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
    double              BuyPrice01;            ///< 申买价
    unsigned long long  BuyVolume01;           ///< 申买量
    double              SellPrice01;           ///< 申卖价
    unsigned long long  SellVolume01;          ///< 申卖量
};

/// 大商所L2最优深度行情
struct DCEL2_Quotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码 含期货合约品种、期权合约品种
    unsigned int        RoutineNo;                          ///< 事务编号 该值始终是0，没有用
    char                SecurityName[40];                   ///< 合约名称
    double              PreClosePrice;                      ///< 昨收价    
    double              PreSettlePrice;                     ///< 昨结算
    unsigned long long  PreTotalPosition;                   ///< 昨持仓量
    double              OpenPrice;                          ///< 开盘价
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
    double              LastPrice;                          ///< 最新价
    double              AveragePrice;                       ///< 均价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    unsigned long long  LastMatchQty;                       ///< 最新成交量
    unsigned long long  TotalVolume;                        ///< 成交总量
    double              TotalAmount;                        ///< 成交总额
    unsigned long long  TotalPosition;                      ///< 持仓量
    long long           InterestChg;                        ///< 持仓量变化
    double              ClosePrice;                         ///< 今收盘价
    double              SettlePrice;                        ///< 结算价

    unsigned int        MBLQuotBuyNum;                      ///< 深度买行情数量
    union
    {
        DCE_BuySellLevelInfo3     BuyLevel[LEVEL_FIVE];        ///< 五档买行情
        struct  
        {
            double              BuyPrice01;            ///< 申买价一
            unsigned long long  BuyVolume01;           ///< 申买量一
            unsigned long long  BidImplyQty01;         ///< 申买推导量一
            double              BuyPrice02;            ///< 申买价二
            unsigned long long  BuyVolume02;           ///< 申买量二
            unsigned long long  BidImplyQty02;         ///< 申买推导量二
            double              BuyPrice03;            ///< 申买价三
            unsigned long long  BuyVolume03;           ///< 申买量三
            unsigned long long  BidImplyQty03;         ///< 申买推导量三
            double              BuyPrice04;            ///< 申买价四
            unsigned long long  BuyVolume04;           ///< 申买量四
            unsigned long long  BidImplyQty04;         ///< 申买推导量四
            double              BuyPrice05;            ///< 申买价五
            unsigned long long  BuyVolume05;           ///< 申买量五
            unsigned long long  BidImplyQty05;         ///< 申买推导量五
        };
    };

    unsigned int                MBLQuotSellNum;        ///< 深度卖行情数量
    union
    {
        DCE_BuySellLevelInfo3    SellLevel[LEVEL_FIVE];      ///< 五档卖行情
        struct  
        {
            double              SellPrice01;           ///< 申卖价一
            unsigned long long  SellVolume01;          ///< 申卖量一
            unsigned long long  AskImplyQty01;         ///< 申卖推导量一
            double              SellPrice02;           ///< 申卖价二
            unsigned long long  SellVolume02;          ///< 申卖量二
            unsigned long long  AskImplyQty02;         ///< 申卖推导量二
            double              SellPrice03;           ///< 申卖价三
            unsigned long long  SellVolume03;          ///< 申卖量三
            unsigned long long  AskImplyQty03;         ///< 申卖推导量三
            double              SellPrice04;           ///< 申卖价四
            unsigned long long  SellVolume04;          ///< 申卖量四
            unsigned long long  AskImplyQty04;         ///< 申卖推导量四
            double              SellPrice05;           ///< 申卖价五
            unsigned long long  SellVolume05;          ///< 申卖量五
            unsigned long long  AskImplyQty05;         ///< 申卖推导量五        
        };
    };

    unsigned int        BuyLevelQueueNo01;             ///< 买一档揭示委托笔数
    unsigned int        BuyLevelQueue[LEVEL_TEN];      ///< 买最优价委托量队列

    unsigned int        SellLevelQueueNo01;            ///< 卖一档揭示委托笔数
    unsigned int        SellLevelQueue[LEVEL_TEN];     ///< 卖最优价委托量队列
    
    double  Delta;          ///< Delta
    double  Gamma;          ///< Gamma
    double  Rho;            ///< Rho
    double  Theta;          ///< Theta
    double  Vega;           ///< Vega
};
/// 大商所L2静态数据
typedef DCEL2_Quotation DCEL2_Static;

/// 大商所L2最优套利深度行情
struct DCEL2_ArbiQuotation
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 套利合约号 格式：套利策略 腿1合约代码&腿2合约代码 套利策略有SP跨期套利，SPC跨品种套利 
    unsigned int        RoutineNo;                          ///< 事务编号 该值始终是0，没有用
    double              LastPrice;                          ///< 最新价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    double              LifeHigh;                           ///< 历史最高成交价格
    double              LifeLow;                            ///< 历史最低成交价格
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格

    unsigned int    MBLQuotBuyNum;                          ///< 深度买行情数量
    union
    {
        DCE_BuySellLevelInfo3    BuyLevel[LEVEL_FIVE];        ///< 五档买行情
        struct  
        {
            double              BuyPrice01;            ///< 申买价一
            unsigned long long  BuyVolume01;           ///< 申买量一
            unsigned long long  BidImplyQty01;         ///< 申买推导量一
            double              BuyPrice02;            ///< 申买价二
            unsigned long long  BuyVolume02;           ///< 申买量二
            unsigned long long  BidImplyQty02;         ///< 申买推导量二
            double              BuyPrice03;            ///< 申买价三
            unsigned long long  BuyVolume03;           ///< 申买量三
            unsigned long long  BidImplyQty03;         ///< 申买推导量三
            double              BuyPrice04;            ///< 申买价四
            unsigned long long  BuyVolume04;           ///< 申买量四
            unsigned long long  BidImplyQty04;         ///< 申买推导量四
            double              BuyPrice05;            ///< 申买价五
            unsigned long long  BuyVolume05;           ///< 申买量五
            unsigned long long  BidImplyQty05;         ///< 申买推导量五
        };
    };

    unsigned int                MBLQuotSellNum;        ///< 深度卖行情数量
    union
    {
        DCE_BuySellLevelInfo3    SellLevel[LEVEL_FIVE];      ///< 五档卖行情
        struct  
        {
            double              SellPrice01;           ///< 申卖价一
            unsigned long long  SellVolume01;          ///< 申卖量一
            unsigned long long  AskImplyQty01;         ///< 申卖推导量一
            double              SellPrice02;           ///< 申卖价二
            unsigned long long  SellVolume02;          ///< 申卖量二
            unsigned long long  AskImplyQty02;         ///< 申卖推导量二
            double              SellPrice03;           ///< 申卖价三
            unsigned long long  SellVolume03;          ///< 申卖量三
            unsigned long long  AskImplyQty03;         ///< 申卖推导量三
            double              SellPrice04;           ///< 申卖价四
            unsigned long long  SellVolume04;          ///< 申卖量四
            unsigned long long  AskImplyQty04;         ///< 申卖推导量四
            double              SellPrice05;           ///< 申卖价五
            unsigned long long  SellVolume05;          ///< 申卖量五
            unsigned long long  AskImplyQty05;         ///< 申卖推导量五        
        };
    };

    unsigned int        BuyLevelQueueNo01;             ///< 买一档揭示委托笔数
    unsigned int        BuyLevelQueue[LEVEL_TEN];      ///< 买最优价委托量队列

    unsigned int        SellLevelQueueNo01;            ///< 卖一档揭示委托笔数
    unsigned int        SellLevelQueue[LEVEL_TEN];     ///< 卖最优价委托量队列
};

/// 大商所L2实时结算价
struct DCEL2_RealTimePrice
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码 含期货合约品种、期权合约品种
    double              SettlePrice ;                       ///< 实时结算价    
};

/// 大商所L2委托统计行情
struct DCEL2_OrderStatistic
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码 含期货合约品种、期权合约品种
    unsigned long long  TotalBuyOrderVolume ;               ///< 委托买入总量 
    unsigned long long  TotalSellOrderVolume ;              ///< 委托卖出总量 
    double              WtAvgBuyPrice ;                     ///< 加权平均委买价    
    double              WtAvgSellPrice ;                    ///< 加权平均委卖价    
};

/// 大商所L2分价成交量行情
struct DCEL2_MarchPriceQty
{
    int                 LocalTimeStamp;                     ///< 数据接收时间HHMMSSMMM 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 暂时未用。后续添加。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码 含期货合约品种、期权合约品种
    union
    {
        DCE_PriceLevelInfo    PriceLevel[LEVEL_FIVE];      ///< 五档分价位成交量
        struct  
        {
            double              Price01;               ///< 价格一
            unsigned long long  PriceBOQty01;          ///< 买开数量一
            unsigned long long  PriceBEQty01;          ///< 买平数量一
            unsigned long long  PriceSOQty01;          ///< 卖开数量一
            unsigned long long  PriceSEQty01;          ///< 卖平数量一
            double              Price02;               ///< 价格二
            unsigned long long  PriceBOQty02;          ///< 买开数量二
            unsigned long long  PriceBEQty02;          ///< 买平数量二
            unsigned long long  PriceSOQty02;          ///< 卖开数量二
            unsigned long long  PriceSEQty02;          ///< 卖平数量二
            double              Price03;               ///< 价格三
            unsigned long long  PriceBOQty03;          ///< 买开数量三
            unsigned long long  PriceBEQty03;          ///< 买平数量三
            unsigned long long  PriceSOQty03;          ///< 卖开数量三
            unsigned long long  PriceSEQty03;          ///< 卖平数量三
            double              Price04;               ///< 价格四
            unsigned long long  PriceBOQty04;          ///< 买开数量四
            unsigned long long  PriceBEQty04;          ///< 买平数量四
            unsigned long long  PriceSOQty04;          ///< 卖开数量四
            unsigned long long  PriceSEQty04;          ///< 卖平数量四
            double              Price05;               ///< 价格五
            unsigned long long  PriceBOQty05;          ///< 买开数量五
            unsigned long long  PriceBEQty05;          ///< 买平数量五
            unsigned long long  PriceSOQty05;          ///< 卖开数量五
            unsigned long long  PriceSEQty05;          ///< 卖平数量五
        };
    };
};

/// 上期所实时行情
struct SHFEL1_Quotation
{
    int                 LocalTimeStamp;                     ///< 本地时间 
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志。1=上海南汇机房行情源；2=深圳福永机房行情源
    int                 ActionDay;                          ///< 业务发生日期,交易实际发生的日期，如周五的夜盘业务发生日期为周五的日期，但市场日期属于下周一对应的日期。
    int                 Time;                               ///< 数据生成时间 两个字段合成一个：HHMMSSMMM
    int                 TradeDate;                          ///< 交易日期 YYYYMMDD 统一为交易归属日期，后续交易所均遵循此原则
    char                SettleGroupID[SETTLE_GROUP_ID_LEN]; ///< 结算组代码
    unsigned int        SettleID;                           ///< 结算编号
    char                Symbol[SYMBOL_LEN];                 ///< 合约代码
    double              PreSettlePrice;                     ///< 昨结算
    double              PreClosePrice;                      ///< 昨收价
    double              PreTotalPosition;                   ///< 昨持仓量
    double              OpenPrice;                          ///< 开盘价
    double              PriceUpLimit;                       ///< 涨停价格
    double              PriceDownLimit;                     ///< 跌停价格
    double              LastPrice;                          ///< 最新价
    double              HighPrice;                          ///< 最高价
    double              LowPrice;                           ///< 最低价
    unsigned long long  TotalVolume;                        ///< 总成交量
    double              TotalAmount;                        ///< 成交总额
    double              TotalPosition;                      ///< 持仓量
    double              ClosePrice;                         ///< 今收盘价
    double              SettlePrice;                        ///< 今结算价
    double              PreDelta;                           ///< 昨虚实度
    double              Delta;                              ///< 今虚实度
    double              BuyPrice01;                         ///< 买价一
    unsigned long long  BuyVolume01;                        ///< 买量一
    double              SellPrice01;                        ///< 卖价一
    unsigned long long  SellVolume01;                       ///< 卖量一
};

struct stUnderlyingSecuritiesItem
{
    char                UnderlyingSecurityCode[20];         ///< 权证对应的5位数的标的证券代码。
    double              UnderlyingSecurityWeight;           ///< 标的证券代码的权重。3位有效小数
};

/// 港交所L2静态数据
struct HKEXL2_Static
{
    int                 LocalTimeStamp;                     ///< 采集时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志，该字段为4位字符串，每位表示特定的含义，无定义则填空格。1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    unsigned int        MessageID;                          ///< 消息包序号
    unsigned int        ChannelID;                          ///< 信道ID
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码，5 位数的证券代码，取值范围在1–99999之间，40字节
    char                MarketType[5];                      ///< 市场代码,MAIN：主板市场 GEM：创业板市场 NASD：纳斯达克市场 ETS：延续交易市场
    char                ISINCode[12];                       ///< ISIN代码
    char                CFICode[5];                         ///< 证券类型, BOND：债券 BWRT：一篮子认股权证 EQTY：股票 TRST：信托 WRNT：认股权证和结构性产品（窝轮、牛熊证）
    char                SpreadTableCode[3];                 ///< 证券价差表，'01'：Part A  '02'：Part B
    char                SecurityName[40];                   ///< 证券简称
    char                Currency[5];                        ///< 货币代码, HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币
    char                SecurityNameGCCS[90];               ///< 证券名称的繁体中文编码，UTF8编码
    char                SecurityNameGB[90];                 ///< 证券名称的简体中文编码，UTF8编码
    unsigned int        RoundLot;                           ///< 每手数目
    double              PreClosePrice;                      ///< 昨收价, 3位小数；前收盘价格（如有除权除息，为除权除息后的收盘价）；
    char                VCMFlag;                            ///< VCM标的证券标志，Y：适用于VCM  N：不适用于VCM
    char                ShortSellFlag;                      ///< 卖空授权标志。Y：允许卖空  N：不允许卖空
    char                CASFlag;                            ///< CAS标的证券。Y：适用于CAS  N：不适用于CAS
    char                CCASSFlag;                          ///< 中央结算证券。Y：中央结算证券  N：非中央结算证券
    char                DummySecurityFlag;                  ///< 虚拟证券标志。Y：虚拟证券  N：正常证券
    char                TestSecurityFlag;                   ///< 测试证券标志。Y：测试证券  N：正常证券
    char                StampDutyFlag;                      ///< 印花税标志。Y：需要印花税  N：不需要印花税
    int                 ListingDate;                        ///< 上市日期。格式为YYYYMMDD。未知的上市日期取值为19000101
    int                 EndDate;                            ///< 退市日期。格式为YYYYMMDD。如该日期不存在取值为0。
    char                FreeText[38];                       ///< 自由文本。固定长度的数组. 当没有自由文档时由空格代替
    char                EFNFlag;                            ///< 外汇基金债券标志。Y：是外汇基金债券  N：不是外汇基金债券
    double              AccruedInterest;                    ///< 应计利息。3位有效小数
    double              CouponRate;                         ///< 票面利率。3位有效小数
    double              ConversionRatio;                    ///< 仅以股票为标的的结构化产品的换股率。3位有效小数
    double              StrikePrice;                        ///< 执行价格。3位有效小数
    int                 MaturityDate;                       ///< 到期日。注：为认股权证、结构化产品的到期日，格式为YYYYMMDD
    char                CallOrPut;                          ///< 认购认沽标志。对于衍生权证以及一篮子权证：C 认购、P认沽、O其他；对于股票挂钩票据、牛熊证：C 看涨；P看跌。注：揭示认股权证或结构性产品是认购还是认沽
    char                OptionType;                         ///< 一篮子认股权证方式。A：美式  E：欧式  空格：其他
    unsigned int        NoUnderlyingSecurities;             ///< 标的证券数目。0：到 20 一揽子认股权证；1：认股权证和结构性产品
    stUnderlyingSecuritiesItem   UnderlyingSecurities[UNDERLYINGSECURITYES_LEN];  ///< 标的证券代码和权重列表。20
    unsigned int        NoLiquidityProviders;               ///< 做市商数目。50个
    unsigned int        LPBrokerNumber[LPBROKERNUMBER_LEN]; ///< 做市商经纪人ID列表。
    double              Yield;                              ///< 债券当期收益率：基于息票率和名义价格。3位有效小数
    unsigned int        StatusID;                           ///< 证券交易状态。2：交易停牌  3：复牌
};


/// 港交所L2行情数据
struct HKEXL2_Quotation
{
    int                 LocalTimeStamp;                     ///< 采集时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志。该字段为4位字符串，每位表示特定的含义，无定义则填空格。 第1位对应：’1’表示国上海机房行情源，’2’表示深圳机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    unsigned int        MessageID;                          ///< 消息包序号
    unsigned int        ChannelID;                          ///< 信道ID
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码，5 位数的证券代码，取值范围在1–99999之间，40字节
    long long           TradeTime;                          ///< 成交时间，交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss不适用于TrdCancelFlag=Y
    double              OpenPrice;                          ///< 开盘价，以开市前时段的最终参考平衡价为开盘价。若开市前时段未产生对盘价格的，以当日第一笔成交价格作为开盘价
    double              LastPrice;                          ///< 最新成交价，3位小数
    double              NominalPrice;                       ///< 按盘价，3位小数
    double              HighPrice;                          ///< 最高价，3位小数
    double              LowPrice;                           ///< 最低价，3位小数
    unsigned long long  TotalVolume;                        ///< 成交总量
    double              TotalAmount;                        ///< 成交总金额，3位小数
    double              VWAP;                               ///< 成交量加权平均价格，3位小数
    unsigned int        ShortSellSharesTraded;              ///< 卖空成交总量
    double              ShortSellTurnover;                  ///< 卖空成交金额，3位小数
    double              ClosePrice;                         ///< 收盘价，3位小数
    unsigned int        TotalNo;                            ///< 成交手数，收盘的成交手数。SS接入暂不揭示，为0值
    unsigned int        RecID;                              ///< 唯一标识每一笔成交情况，按代码，从1开始，随每笔成交递增
    unsigned long long  TradeVolume;                        ///< 瞬时成交量，如果TrdCancelFlag=Y，表示未成交的剩余量
    int                 TradeType;                          ///< 公共成交类型
                                                            ///< 0：自动对盘的非双边客成交 (AMS<space>)
                                                            ///< 4：开市前成交(Off-exchange previous day) (AMS “P”)
                                                            ///< 22：非自动对盘或者特别买卖单位的非双边客成交(AMS “M”)
                                                            ///< 100：自动对盘的双边客成交(AMS“Y”)
                                                            ///< 101：非自动对盘或者特别买卖单位的双边客成交(AMS “X”)
                                                            ///< 102：碎股成交(AMS “D”)
                                                            ///< 103：竞价成交 (AMS “U”)
                                                            ///< 104：海外交易
                                                            ///< 当TrdCancelFlag = Y时不适用
    char                TradeCancelFlag;                    ///< 指示：原来交易收录器中的交易已经被取消了。Y：取消  N：没有取消
    double              AggregatePrice;                     ///< 参考平衡价格。3位小数，IEP不适用时取值为0
    unsigned long long  AggregateQuantity;                  ///< 参考平衡成交量
    double              CASReferencePrice;                  ///< 委托订单参考价格（CAS阶段）。3位有效小数。如果参考价不适用，取值为0
    double              CASLowerPrice;                      ///< 委托订单参考价格（CAS阶段）。3位有效小数。取值0表示N/A
    double              CASUpperPrice;                      ///< 委托订单上限价格（CAS阶段）。3位有效小数。取值0表示N/A
    char                OrderImbalanceDirection;            ///< 买卖数量不平衡方向。N：买卖相等 B：买方剩余 S：卖方剩余 空格：不适用，当参考平衡价不适用
    unsigned long long  OrderImbalanceQuantity;             ///< 买卖不平衡数量。当不平衡方向OrderImbalanceDirection为空格，此值无意义。 注：在参考平衡价格阶段，买卖不平衡的数量
    long long           CoolingOffStartTime;                ///< 冷静期开始时间。交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss，取值0表示N/A
    long long           CoolingOffEndTime;                  ///< 冷静期结束时间。交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss，取值0表示N/A
    double              VCMReferencePrice;                  ///< 冷静期参考价。3位小数，取值0表示N/A
    double              VCMLowerPrice;                      ///< 冷静期可交易价格下限。3位小数，取值0表示N/A
    double              VCMUpperPrice;                      ///< 冷静期可交易价格上限。3位小数，取值0表示N/A
    unsigned int        BuyLevelNo;                         ///< 买盘价位数
    union
    {
        BuySellLevelInfo3 BuyLevel[LEVEL_TEN];
        struct
        {
            double              BuyPrice01;                         ///< 申买价一。3位小数
            unsigned long long  BuyVolume01;                        ///< 申买量一
            unsigned int        TotalBuyOrderNo01;                  ///< 申买价一总委托笔数
            double              BuyPrice02;                         ///< 申买价二。3位小数
            unsigned long long  BuyVolume02;                        ///< 申买量二
            unsigned int        TotalBuyOrderNo02;                  ///< 申买价二总委托笔数
            double              BuyPrice03;                         ///< 申买价三。3位小数
            unsigned long long  BuyVolume03;                        ///< 申买量三
            unsigned int        TotalBuyOrderNo03;                  ///< 申买价三总委托笔数
            double              BuyPrice04;                         ///< 申买价四。3位小数
            unsigned long long  BuyVolume04;                        ///< 申买量四
            unsigned int        TotalBuyOrderNo04;                  ///< 申买价四总委托笔数
            double              BuyPrice05;                         ///< 申买价五。3位小数
            unsigned long long  BuyVolume05;                        ///< 申买量五
            unsigned int        TotalBuyOrderNo05;                  ///< 申买价五总委托笔数
            double              BuyPrice06;                         ///< 申买价六。3位小数
            unsigned long long  BuyVolume06;                        ///< 申买量六
            unsigned int        TotalBuyOrderNo06;                  ///< 申买价六总委托笔数
            double              BuyPrice07;                         ///< 申买价七。3位小数
            unsigned long long  BuyVolume07;                        ///< 申买量七
            unsigned int        TotalBuyOrderNo07;                  ///< 申买价七总委托笔数
            double              BuyPrice08;                         ///< 申买价八。3位小数
            unsigned long long  BuyVolume08;                        ///< 申买量八
            unsigned int        TotalBuyOrderNo08;                  ///< 申买价八总委托笔数
            double              BuyPrice09;                         ///< 申买价九。3位小数
            unsigned long long  BuyVolume09;                        ///< 申买量九
            unsigned int        TotalBuyOrderNo09;                  ///< 申买价九总委托笔数
            double              BuyPrice10;                         ///< 申买价十。3位小数
            unsigned long long  BuyVolume10;                        ///< 申买量十
            unsigned int        TotalBuyOrderNo10;                  ///< 申买价十总委托笔数
        };
    };
    unsigned int                SellLevelNo;                        ///< 卖盘价位数
    union
    {
        BuySellLevelInfo3 SellLevel[LEVEL_TEN];
        struct
        {
            double              SellPrice01;                        ///< 申卖价一。3位小数
            unsigned long long  SellVolume01;                       ///< 申卖量一
            unsigned int        TotalSellOrderNo01;                 ///< 申卖价一总委托笔数
            double              SellPrice02;                        ///< 申卖价二。3位小数
            unsigned long long  SellVolume02;                       ///< 申卖量二
            unsigned int        TotalSellOrderNo02;                 ///< 申卖价二总委托笔数
            double              SellPrice03;                        ///< 申卖价三。3位小数
            unsigned long long  SellVolume03;                       ///< 申卖量三
            unsigned int        TotalSellOrderNo03;                 ///< 申卖价三总委托笔数
            double              SellPrice04;                        ///< 申卖价四。3位小数
            unsigned long long  SellVolume04;                       ///< 申卖量四
            unsigned int        TotalSellOrderNo04;                 ///< 申卖价四总委托笔数
            double              SellPrice05;                        ///< 申卖价五。3位小数
            unsigned long long  SellVolume05;                       ///< 申卖量五
            unsigned int        TotalSellOrderNo05;                 ///< 申卖价五总委托笔数
            double              SellPrice06;                        ///< 申卖价六。3位小数
            unsigned long long  SellVolume06;                       ///< 申卖量六
            unsigned int        TotalSellOrderNo06;                 ///< 申卖价六总委托笔数
            double              SellPrice07;                        ///< 申卖价七。3位小数
            unsigned long long  SellVolume07;                       ///< 申卖量七
            unsigned int        TotalSellOrderNo07;                 ///< 申卖价七总委托笔数
            double              SellPrice08;                        ///< 申卖价八。3位小数
            unsigned long long  SellVolume08;                       ///< 申卖量八
            unsigned int        TotalSellOrderNo08;                 ///< 申卖价八总委托笔数
            double              SellPrice09;                        ///< 申卖价九。3位小数
            unsigned long long  SellVolume09;                       ///< 申卖量九
            unsigned int        TotalSellOrderNo09;                 ///< 申卖价九总委托笔数
            double              SellPrice10;                        ///< 申卖价十。3位小数
            unsigned long long  SellVolume10;                       ///< 申卖量十
            unsigned int        TotalSellOrderNo10;                 ///< 申卖价十总委托笔数
        };
    };
};

/// 经纪人队列条目
struct HKBrokerIDItem
{
    unsigned int    BrokerID;                           ///< 经纪人ID
    unsigned int    BQPriceLevel;                       ///< 该标志表明经纪人ID所对应的价格档位
};

/// 港交所L2经纪人队列
struct HKEXL2_BrokerQueue
{
    int                 LocalTimeStamp;                     ///< 采集时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志，该字段为4位字符串，每位表示特定的含义，无定义则填空格。1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    unsigned int        MessageID;                          ///< 消息包序号
    unsigned int        ChannelID;                          ///< 信道ID
    char                Symbol[SYMBOL_LEN];                 ///< 证券代码，5 位数的证券代码，取值范围在1–99999之间，40字节
    char                BuyBQMoreFlag;                      ///< 买方经纪人队列标识。Y：队列中还有经纪人 N：队列中已没有经纪人
    unsigned int        BuyBQItemCount;                     ///< 买方经纪人队列数。0-40
    HKBrokerIDItem      BuyBrokerQueue[BROKERQUEUE_LEN];    ///< 买方经纪人队列，根据BQItemCount确定个数，最大40
    char                SellBQMoreFlag;                     ///< 卖方经纪人队列标识。Y：队列中还有经纪人 N：队列中已没有经纪人
    unsigned int        SellBQItemCount;                    ///< 卖方经纪人队列数。0-40
    HKBrokerIDItem      SellBrokerQueue[BROKERQUEUE_LEN];   ///< 卖方经纪人队列，根据BQItemCount确定个数，最大40

};

/// 港交所L2指数行情
struct HKEXL2_Index
{
    int                 LocalTimeStamp;                     ///< 采集时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志，该字段为4位字符串，每位表示特定的含义，无定义则填空格。1=上海南汇机房行情源；2=深圳福永机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    unsigned int        MessageID;                          ///< 消息包序号
    unsigned int        ChannelID;                          ///< 信道ID
    char                Symbol[SYMBOL_LEN];                 ///< 指数代码，5 位数的证券代码，取值范围在1–99999之间，40字节
    char                SymbolSource[SYMBOLSOURCE_LEN];     ///< 指数来源。C：中证指数有限公司或者其他行情源；H：恒生指数有限公司；S：标准普尔道琼斯指数；T：汤森路透
    long long           Time;                               ///< 数据生成时间。交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
    double              OpenPrice;                          ///< 开盘价，4位有效小数
    double              LastPrice;                          ///< 最新价，4位有效小数
    double              PriceUpdown1;                       ///< 净变化值，4位有效小数。昨收价与最新价的净变化值
    double              HighPrice;                          ///< 最高价，4位小数
    double              LowPrice;                           ///< 最低价，4位小数
    double              TotalAmount;                        ///< 成交总额，4位小数
    long long           TotalVolume;                        ///< 成交总量
    double              PreviousSesClose;                   ///< 前交易阶段收盘值，4位小数。（CSI、S&P为前一个交易日收盘价，HIS、TR为上一交易阶段的收盘值）
    double              EASValue;                           ///< 清算价平均估算值。2位有效小数
    double              ClosePrice;                         ///< 收盘价。4位有效小数
    char                Currency[5];                        ///< 指数货币代码。HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币
    char                Exception;                          ///< 异常指示器
    char                SecurityPhaseTag[8];                ///< 指数状态。C：收盘取值 I：Indicative O：开盘 P：前收盘价 R：初步收盘 S：止损 T：实时；如果第三方公司没有定义,取值可以为空
};

/// 港交所L2市场总览
struct HKEXL2_Overview
{
    int                 LocalTimeStamp;                     ///< 采集时间HHMMSSMMM
    char                QuotationFlag[QUOTATION_FLAG_LEN];  ///< 行情源标志。该字段为4位字符串，每位表示特定的含义，无定义则填空格。 第1位对应：’1’表示国上海机房行情源，’2’表示深圳机房行情源
    long long           PacketTimeStamp;                    ///< 包头时间YYYYMMDDHHMMSSMMM
    unsigned int        MessageID;                          ///< 消息包序号
    unsigned int        ChannelID;                          ///< 信道ID
    char                MarketType[5];                      ///< 市场代码，MAIN：主板市场 GEM：创业板市场 NASD：纳斯达克市场 ETS：延续交易市场
    char                MarketName[40];                     ///< 市场名称
    char                BasicCurrency[5];                   ///< 货币代码。HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币
    char                Currency[5];                        ///< 货币代码。HKD：港元 USD：美元 EUR：欧元 JPY：日元 GBP：英镑 CAD：加拿大元 SGD：新加坡元 CNY：人民币，空值：配合“成交金额”使用。
    int                 CurrencyFactor;                     ///< 货币转换因子。非零值 n 意味着这个代码的所有价格字段应该如此表示：价值= 该价格 * 10^n
    double              ExchangeRate;                       ///< 汇率。比率，用港币来表达一单位的外币. 4位小数.
    unsigned int        SampleNo;                           ///< 证券数量
    double              TotalAmount;                        ///< 成交金额。3位小数
    unsigned int        TradingSessionID;                   ///< 交易阶段标识符。1：日中
    unsigned int        TradingSessionSubID;                ///< 交易阶段子标识符。
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
    unsigned int        TradingSesStatus;                   ///< 当前交易阶段状态。
                                                            ///< 0：未知（为NO）
                                                            ///< 1：停牌（对于BL，EI）
                                                            ///< 2：开盘（对于[POS] OI，[POS] NC，[POS] MA，CT，OC）
                                                            ///< 3：关闭（针对CL）                 
                                                            ///< 5：预关闭（对于[CAS] RP，[CAS] NW，[CAS] RC，[CAS] MA，[CAS] OI）
                                                            ///< 100：日关闭(对于DC)
    char                TradingSesControlFlag;              ///< 不同时段切换方式标志。‘0’：自动的 (默认) ‘1’：手动的 (这天的正常调度无效)
    long long           StartDateTime;                      ///< 交易状态开始时间。交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
    long long           EndDateTime;                        ///< 交易状态结束时间。交易所发包时间为UTC格式，转化成datetime格式：YYYYMMDDHHMMSSsss
};

/// 上期所静态数据
typedef SHFEL1_Quotation SHFEL1_Static;


/// 证券代码
struct StockSymbol
{
    char    Symbol[SYMBOL_LEN + MARKET_TAG_LEN];        ///< 查询个股列表返回的股票代码，带市场标志
};

/// 服务器地址
struct ServerState
{
    MsgType msgType;            ///< 消息类型
    char    IP[16];             ///< 服务器地址，127.0.0.1
    unsigned short   Port;      ///< 端口
    bool    Connected;          ///< 连接状态，true-连接中，false-未连接
};

#pragma pack()





#endif // GTA_QTS_STRUCT_H_
