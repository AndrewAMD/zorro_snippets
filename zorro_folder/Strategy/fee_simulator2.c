#include<default.c>

#define ASSET_FEE "DUMMY"
#define ALGO_FEE "FEE"

#define ASSET_TRADING "EUR/USD"
#define ALGO_TRADING "TRADING"

//#define ENABLE_PRINT_CHECKPOINTS

void cp(const char* str){
#ifdef ENABLE_PRINT_CHECKPOINTS
	printf("\ncheckpoint %s, Bar=%d",str,Bar);
#endif
}

void incur_fee(int flags, var amt){
	if(amt<=0) return;
	if(Live) return;
	printf("#\nIncurring fee of %0.2f",amt);
	algo(ALGO_FEE);
	asset(ASSET_FEE);
	Commission = amt - 0.01;
	enterLong(1);
	exitLong();
}


void run(){
	cp("a");
	if(!Test){
		print(TO_ANY,"\nOnly [Test] mode supported.");
		quit("#Quitting...");
		return;
	}
	
	cp("b");
	if(Init){
		set(LOGFILE);
		Hedge = 5;
		Verbose = 7|DIAG;
		Capital = 100000;
		BarPeriod = 1440;
		LookBack = 10;
		BarMode = BR_FLAT|BR_WEEKEND;
		StartDate = 2017;
		EndDate = 2019;
		assetList("AssetsFix");
		
		printf("\nsetting up dummy asset...");
		assetAdd(ASSET_FEE,0.01,0.00,0,0,0.00,0.00,0,2,1,0.00,ASSET_FEE);
		algo(ALGO_FEE);
		asset(ASSET_FEE);
		algo(ALGO_TRADING);
		asset(ASSET_TRADING);
		
	}
	
	cp("c");
	if(!Init && !is(LOOKBACK) && ((int)Equity)>0){
		algo(ALGO_TRADING);
		asset(ASSET_TRADING);
		if(Bar%5){
			if(Bar%2){
				enterLong(100);
			}
			else{
				exitLong();
				printf("#\nEquity: %0.2f",Equity);
				var fee = roundto(random(10.00),0.01);
				printf("#\nfee = %0.2f",fee);
				call(1,incur_fee,0,fee);
			}
		}
	}
	
	cp("d");
	if(is(EXITRUN)){
		printf("#\nFinal equity is: %0.2f",Equity);
	}
	cp("e");
}
