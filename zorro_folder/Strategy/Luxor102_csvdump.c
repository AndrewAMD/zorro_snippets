// Luxor system by Jaeckle/Tomasini /////////////
// Make a system profitable by creative selection
// of start and end date 

struct iteration
{
	int fast;
	int slow;
};

void dumpTradeData(string FileName, int fast, int slow)
{

	
	if(is(FIRSTINITRUN))
	{
		file_write(FileName,"Fast, Slow, NetProfit",0);
	} 
	
	char output[100];
	
	if(is(EXITRUN))
	{
		sprintf(output,"\n%d,%d,%.2f",fast,slow,(WinTotal - LossTotal));
		file_append(FileName,output,0);
	}
}



function run()
{
	set(PARAMETERS);  // generate and use optimized parameters
	
	StartDate = 2005;
	EndDate = 2007;
	BarPeriod = 30;
	LookBack = 100;
	asset("USD/JPY");

// no trade costs...	
	Spread = 0;
	Slippage = 0;
	RollLong = RollShort = 0; 
	
	
	struct iteration iter[900];
	int i, j, k;
	for(i=0;i<900;i++)
	{
		for(j=1; j<=30; j++)
		{
			for(k=31; k<=60; k++)
			{
				iter[i].fast = j;
				iter[i].slow = k;
			}
			
		}
	}
	
	int it = optimize(500,1,900,1);
	int fast, slow;
	fast = iter[it].fast; //3
	slow = iter[it].slow; //30
	
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

	PlotWidth = 1000;
	PlotBars = 0; // plot all
	dumpTradeData("LuxorDump002.csv", fast, slow);
}