#ifndef __D31_ITEM_H__
#define __D31_ITEM_H__

struct D31ItemStruct{
	unsigned int 	nStockCode;
	unsigned int	nTradeTime;
	float	afZbBidAmount[10];
	float	afZbBidVolume[10];
	unsigned short int	anZbBidOrderNum[10];
	float	afZbAskAmount[10];
	float	afZbAskVolume[10];
	unsigned short int	anZbAskOrderNum[10];
	float	afZdBidAmount[10];
	float	afZdBidVolume[10];
	unsigned short int	anZdBidOrderNum[10];
	float	afZdAskAmount[10];
	float	afZdAskVolume[10];
	unsigned short int	anZdAskOrderNum[10];
        float   fTenBidVolume;
        float   fTenAskVolume;
        float   fTenBidAmnt;
        float   fTenAskAmnt;
        float   fTotalBidVolume;
        float   fTotalAskVolume;
        float   fTotalBidAmnt;
        float   fTotalAskAmnt;
        float   fWtAvgBidPrice;
        float   fWtAvgAskPrice;
        float   fLastClose;
        float   fCurPrice;
        float   fAvgTotalBidAmnt;
        float   fAvgTotalAskAmnt;
        float   fBidAmount20;
        float   fAskAmount20;
        float   fBidAmount50;
        float   fAskAmount50;
        float   fBidAmount100;
        float   fAskAmount100;
        float   afReserve[27];
};

#endif
