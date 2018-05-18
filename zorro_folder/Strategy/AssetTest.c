#include <parsers.h>

int AssetParseTest1();
int AssetParseTest2();
int AssetParseTest3();

int main()
{
	if(is(INITRUN))
	{
		da_delete_all();
		//AssetParseTest1();
		//AssetParseTest2();
		AssetParseTest3();
	}

	return 0;
}


void print_AssetListEntry(const AssetListEntry* e)
{
	printf("\n\"%s\",%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,%0.4f,\"%s\",%d"
		, e->sName
		, e->vPrice
		, e->vSpread
		, e->vRollLong
		, e->vRollShort
		, e->vPip
		, e->vPipCost
		, e->vMarginCost
		, e->vLeverage
		, e->vLotAmount
		, e->vCommission
		, e->sSymbol
		, e->nStrategies
	);
}

int AssetParseTest1()
{
	//const char str[128] = "USD/JPY,113.225,0.017,0.0678,-0.1783,0.01,0.000088,0.04,25.004,1,0,USD/JPY";
	//const char str[128] = "USD/JPY,113.225,0.017,0.0678,-0.1783,0.01,0.000088,0.04,25.004,1,0"; // symbol field is blank
	char str[128];
	strcpy(str,"USD/JPY,113.225,0.017,0.0678,-0.1783,0.01,0.000088,0.04,25.004,1,0,,1.1"); // symbol is blank and there are extra arguments!!

	AssetListEntry* output = ParseAssetListLine(str);
	if (!output) 
	{
		printf("\nAssetParseTest1: there is no output!");
		return 0;
	}
	print_AssetListEntry(output);
	return(0);
}

int AssetParseTest2()
{
	char str[1028];
	strcpy(str,"Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol \r\nAUD/USD,0.76738,0.00019,0.0321,-0.3726,0.0001,0.0001,0.023,33.342,1,0,AUD/USD\r\nEUR/USD,1.05523,0.00014,-0.178,-0.0471,0.0001,0.0001,0.0211,50.007,1,0,EUR/USD\r\nGBP/USD,1.24394,0.00024,-0.1609,-0.0848,0.0001,0.0001,0.0622,20.004,1,0,GBP/USD \r\n ");
	int hList = ParseAssetListString(str);

	int i; AssetListEntry* pEntry = NULL;
	for(i=0; i<da_size(hList); i++)
	{
		pEntry = (AssetListEntry*)da_data(hList, i);
		print_AssetListEntry(pEntry);
	}

	return 0;
}
int AssetParseTest3()
{
	int hList = ParseAssetList(".\\History\\AssetsFix-OandaCAN-API.csv");

	int i; AssetListEntry* pEntry = NULL;
	for(i=0; i<da_size(hList); i++)
	{
		pEntry = (AssetListEntry*)da_data(hList, i);
		print_AssetListEntry(pEntry);
	}

	return 0;
}


