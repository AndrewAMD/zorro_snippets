// dp_analysis.c
// Dual parameter analysis
// Iterate through all combinations of possible parameters,
// 		and export the results to a csv file

typedef struct DP_PARAMETER
{
	char sName[32];
	var vMin;
	var vMax;
	var vStep;
} DP_PARAMETER;

typedef struct DP_METRIC
{
	char sName[32];
	var *vpVal;
	int *ipVal;
} DP_METRIC;

typedef struct DP_ASSET
{
	char	sName[NAMESIZE];	// short name of the asset
	
} DP_ASSET;

typedef struct DP_ITERATION
{
	var v1;
	var v2;
	DP_ASSET *dpa;	
}

typedef struct DP_GLOBALS
{
	bool bInitialized;
	int nAssetsDefined;
	DP_ASSET a[1000];
	DP_PARAMETER p1;
	DP_PARAMETER p2;
	DP_METRIC m[32];
	int nIters; // number of iterations to be performed
	DP_ITERATION iter[1000000]; // the iterations
	
} DP_GLOBALS;

//globals
DP_GLOBALS g_dp;

int dpMaxSteps(DP_PARAMETER p)
{
	return((int)(((p.vMax - p.vMin)/p.vStep)+1.0));
}

void dpInitialize()
{
	if(g_dp.bInitialized) return;
	
	int max1=1, max2=1, maxAssets=1;
	if(g_dp.nAssetsDefined)	{maxAssets=g_dp.nAssetsDefined;}
	max1 = dpMaxSteps(g_dp.p1);
	max2 = dpMaxSteps(g_dp.p2);
	g_dp.nIters = (max1 * max2 * maxAssets);
	
	int i=0,j=0,k=0,nIndex=0;
	for (i=0;((i<maxAssets)&&(nIndex<g_dp.nIters)); i++)
	{
		if(nAssetsDefined) g_dp.iter[nIndex].dpa=&g_dp.a[i];
		for(j=0; j<max1; j++)
		{
			g_dp.iter[nIndex].v1 = g_dp.p1.vMin + (j)*(g_dp.p1.vMax);
			for(k=0; k<max2; k++)
			{
				g_dp.iter[nIndex].v2 = g_dp.p2.vMin + (k)*(g_dp.p2.vMax);
				nIndex++;
			}
		}
	}
	g_dp.bInitialized = true;
	return;
}




function run()
{
	StartDate = 2005;
	EndDate = 2007;
	BarPeriod = 30;
	LookBack = 100;
	//asset("GBP/USD");

// no trade costs...	
	Spread = 0;
	Slippage = 0;
	RollLong = RollShort = 0; 
	
	
	// while(asset(loop("GBP/USD","USD/JPY")))
	// {
	dp1Set("Fast", 1,  2, 1);
	dp2Set("Slow", 30, 31, 1);
	dpAddAsset("EUR/USD");
	dpAddAsset("USD/JPY");
	dpIterate();
	
	int fast, slow;
	fast = (int)dp1Get();
	slow = (int)dp2Get();
	
	
	vars Price = series(priceClose()),
		Fast = series(SMA(Price,fast)),
		Slow = series(SMA(Price,slow));
	
	static var BuyLimit,SellLimit,BuyStop,SellStop;
	
	if(crossOver(Fast,Slow)) {
		BuyStop = priceHigh() + 1*PIP;
		BuyLimit = priceHigh() + 5*PIP;
	}
	if(crossUnder(Fast,Slow)) {
		SellStop = priceLow() - 1*PIP;
		SellLimit = priceLow() - 5*PIP;
	}
		
	if(!NumOpenLong && Fast[0] > Slow[0] && Price[0] < BuyLimit)
		enterLong(1,BuyStop);
	if(!NumOpenShort && Fast[0] < Slow[0] && Price[0] > SellLimit)
		enterShort(1,SellStop);
	// }

	PlotWidth = 1000;
	PlotBars = 0; // plot all
	//dumpTradeData("LuxorDump002.csv", fast, slow);
	dpCsvExport("Luxor104.csv");
}