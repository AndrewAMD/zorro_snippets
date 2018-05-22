// Indicatortest ///////////////////
#include <reversal_fractals.h>
#include <profile.c>

void tradeFunc(int signals); // empty function pointer
void tradeRF7B5B(int signals);
void tradeRF7B(int signals);
void tradeRF5B(int signals);

function run()
{
	StartDate = 2016;
	EndDate = 2018; // fixed simulation period 
	BarPeriod = 5;
	Verbose = 2;
	set(LOGFILE + PARAMETERS); // log all trades

	// get trading signals
	int signals = rf_signals(0, true);
	
	//while(asset(loop(Assets)))
	while(tradeFunc = loop(tradeRF7B5B))
	{
		tradeFunc(signals);
	}
	

	
	PlotWidth = 800;
	PlotHeight1 = 300;
	//plot("MMI_Raw",MMI_Raw,NEW,GREY);
	//plot("MMI_Smooth",MMI_Smooth,0,BLACK);
	//plotTradeProfile(-50); 
}

	int timeframes[9] =
	{
		1, //5m
		2, // 10m
		3, // 15m
		6, // 30m
		12, // 1h,
		24, // 2h,
		48, // 4h
		144, //12h
		288 // 1d
		
	};

void tradeRF7B5B(int signals)
{
	algo("RF7B5B");
	TimeFrame = optimize(4,0,8,1,0);	
	
	if(signals & RF5B_BUY) enterLong();
	if(signals & RF7B_BUY) enterLong();
	if(signals & RF5B_SELL) enterShort();
	if(signals & RF7B_SELL) enterShort();
	
	
}

void tradeRF7B(int signals)
{
	algo("RF7B");
	if(signals & RF7B_BUY) enterLong();
	if(signals & RF7B_SELL) enterShort();
}

void tradeRF5B(int signals)
{
	algo("RF5B");
	if(signals & RF5B_BUY) enterLong();
	if(signals & RF5B_SELL) enterShort();
}
