// Trade Every Minute for Test Purposes ///////////////////

#include <contract.c>

int AutoTrade = 0;
int Strike, Expiry;
int Chain = 0;
long hContracts = 1;

function click(int row,int col)
{
	if(!is(RUNNING) || !Chain) return; // only clickable when session is active
	sound("click.wav");	
	string Text = panelGet(row,col);
	
	if(Text == "Auto Off") {
		panelSet(row,col,"Auto On",0,0,0); 
		AutoTrade = 1;
	}
	else if(Text == "Auto On") {
		panelSet(row,col,"Auto Off",0,0,0); 
		AutoTrade = 0;
	}
	else {
		Lots = slider(1);	// get current slider position
		Strike = priceClose()+slider(2);
		Expiry = slider(3);
		if(Text == "Buy Call") {
			contract(CALL,Expiry,Strike);
			contractPrice(ThisContract);
			enterLong();
		} else if(Text == "Buy Put") {
			contract(PUT,Expiry,Strike);
			contractPrice(ThisContract);
			enterLong();
		} else if(Text == "Sell Call") {
			contract(CALL,Expiry,Strike);
			contractPrice(ThisContract);
			enterShort();
		} else if(Text == "Sell Put") {
			contract(PUT,Expiry,Strike);
			contractPrice(ThisContract);
			enterShort();	
		} else if(Text == "Close All") {
			exitShort();	
			exitLong();	
		}
	}
}

function run()
{
#ifndef COMPILE
	if(is(TRAINMODE))
#endif
		set(EXE);
	if(mode(EXE) && !is(EXE))
		return;
	if(is(TESTMODE)) {
		quit("Click [Trade]!");
		return;
	} 
	
	BarPeriod = 1;
	NumYears = 1;
	LookBack = 0;
	Verbose = 3;
	set(LOGFILE);
	SaveMode = 0;

	Multiplier = 100;
	Lots = slider(1,1,0,20,"Lots",0);
	Strike = priceClose()+slider(2,0,-50,50,"Strike","Strike offset in points");
	Expiry = slider(3,30,1,100,"Expiry","Expiration in days");

	if(is(INITRUN)) {
		AutoTrade = 0;
		panel(5,1,GREY,80);
		int row = 0;
//		panelSet(row++,0,"Auto Off",ColorPanel[2],1,4);
//		row++;
		panelSet(row++,0,"Buy Call",ColorPanel[3],1,4);
		panelSet(row++,0,"Buy Put",ColorPanel[3],1,4);
		panelSet(row++,0,"Sell Call",ColorPanel[3],1,4);
		panelSet(row++,0,"Sell Put",ColorPanel[3],1,4);
		panelSet(row++,0,"Close All",ColorPanel[3],1,4);
	}
	

	static int UpdateTime;
	//assetList("AssetsIB");
	asset("SPY");
	Multiplier = 100;
	if(is(INITRUN)) {
		printf("\nNY Time: %02d:%02d",lhour(ET),minute());
		UpdateTime = 60;
	}
	
	if(UpdateTime++ >= 60) {
		UpdateTime = 0;
		printf("\nDownloading chain.. ");
		Chain = contractUpdate(Asset,hContracts,PUT|CALL); 
		printf("\n%d contracts",Chain);
		if(!Chain) { quit(); return; }
		if(is(INITRUN)){
			printf("\nsaving file contractlist.csv");
			file_write("contractlist.csv","Multiplier,Strike,Expiry,Type\r\n",0);
			int i = 1;
			CONTRACT* pC = NULL;
			char buf[128];
			for(i = 1; i<= Chain; i++){
				pC = contract(i);
				memset(buf,0,sizeof(char) * 128);
				sprintf(buf,"%0.0f,%f,%d,%d\r\n",(double)pC->fVal,(double)pC->fStrike,pC->Expiry,pC->Type);
				file_append("contractlist.csv",buf,0);
			}
		}
	}	

// check for expired or exercised contracts
	for(open_trades) {
		contract(ThisTrade);
		contractPrice(ThisTrade);
		printf("\nAsk %.4f Unl %.4f Vol %.0f",ContractAsk,ContractUnl,ContractVol);
		contractCheck(ThisTrade,3);
	}
}





