#include <parser_assetlist.h>
#define STRAT01 1<<0
#define STRAT02 1<<1
#define STRAT03 1<<2

int run()
{
	StartDate = 2010;
	EndDate = 2016; // fixed simulation period 
	Verbose = 2;
	LookBack = 500;
	set(LOGFILE); // log all trades

	static int hList;
	if(is(FIRSTINITRUN))
	{
		hList = ParseAssetList(".\\History\\AssetsFix-OandaCAN-API.csv");
		
		// Example: 3 sub-strategies, requiring a total of four Assets total.
		requireAsset(hList,STRAT01,"EUR/USD");
		requireAsset(hList,STRAT01,"USD/JPY");
		requireAsset(hList,STRAT02,"USD/JPY");
		requireAsset(hList,STRAT02,"GBP/USD");
		requireAsset(hList,STRAT03,"USD/CAD");
		
		// Hack: export to CSV so that Asset loop will work correctly.
		saveAssetListNeeded(hList,"booyah.csv");
	}
	if(is(FIRSTRUN))
	{
		assetList("booyah.csv");
	}
	while(asset(loop(Assets)))
	{

		vars Price = series(price());
		vars Trend = series(LowPass(Price,500));
		
		Stop = 4*ATR(100);
		
		vars MMI_Raw = series(MMI(Price,300));
		vars MMI_Smooth = series(LowPass(MMI_Raw,300));
		
		if(falling(MMI_Smooth)) 
		{
			if(valley(Trend))
				enterLong();
			else if(peak(Trend))
				enterShort();
		}
		if(is(EXITRUN))
		{
			da_delete_all(); // memory cleanup
		}
	}
	return 0;
}


