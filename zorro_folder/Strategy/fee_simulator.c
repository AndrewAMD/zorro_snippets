#include<default.c>

#define ASSET_FEE "DUMMY"
#define ALGO_FEE "FEE"

void incur_fee(var amt){
	if(amt<=0) return;
	if(Live) return;
	algo(ALGO_FEE);
	asset(ASSET_FEE);
	Commission = amt - 0.01;
	enterLong(1);
	exitLong();
}


void run(){
	if(!Test){
		print(TO_ANY,"\nOnly [Test] mode supported.");
		quit("#Quitting...");
		return;
	}
	
	if(Init){
		set(LOGFILE);
		Verbose = 3;
		Capital = 100000;
		BarPeriod = 1440;
		LookBack = 0;
		BarMode = BR_FLAT|BR_WEEKEND;
		StartDate = 2017;
		EndDate = 2019;
		printf("\nloading dummy asset list...");
		assetList("AssetsFix");
		printf("\nsetting up dummy asset...");
		assetAdd(ASSET_FEE,0.01,0.00,0,0,0.00,0.00,0,2,1,0.00,ASSET_FEE);
		algo(ALGO_FEE);
		asset(ASSET_FEE);
	}
	
	if(!Init && !is(LOOKBACK) && ((int)Equity)>0){
		printf("\nEquity: %0.2f",Equity);
		var fee = roundto(random(1000.0),50.0);
		printf("\nfee = %0.2f",fee);
		incur_fee(fee);
	}
	
	if(is(EXITRUN)){
		printf("\nFinal equity is: %0.2f",Equity);
	}
}
