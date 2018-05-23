// Luxor system by Jaeckle/Tomasini /////////////
// Make a system profitable by creative selection
// of start and end date 

function run()
{
	set(LOGFILE);
	Verbose = 7;

	StartDate = 2001;
	EndDate = 2001;
	BarPeriod = 30;
	LookBack = 30;
	asset("GBP/USD");

// no trade costs...	
	Spread = 0;
	Slippage = 0;
	RollLong = RollShort = 0; 
	TRADE* pTrade = NULL;
	
	vars Price = series(priceClose()),
		Fast = series(SMA(Price,3)),
		Slow = series(SMA(Price,30));
	
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
		pTrade  = enterLong(1,BuyStop);
	if(!NumOpenShort && Fast[0] < Slow[0] && Price[0] > SellLimit)
		pTrade = enterShort(1,SellStop);

	if (pTrade)
	{			
		print(TO_LOG,"\nopen: EntryPrice [%.4f]", (var)(pTrade->fEntryPrice));
		print(TO_LOG,"\nopen: StopLimit [%.4f]", (var)(pTrade->fStopLimit));
		print(TO_LOG,"\nopen: ProfitLimit [%.4f]", (var)(pTrade->fProfitLimit));
	}
	
	if(is(EXITRUN))
	{
		for(all_trades) 
		{
			
			
			
		}
	}

	PlotWidth = 1000;
	PlotBars = 0; // plot all
}