// dp_analysis.c
// Dual parameter analysis
// Iterate through all combinations of possible parameters,
// 		and export the results to a csv file

typedef struct PARAMETER
{
	char sName[32];
	var vMin;
	var vMax;
	var vStep;
	int nMaxSteps;
	int nIndex;
} PARAMETER;

typedef struct METRIC
{
	char sName[32];
	var *vpVal;
	int *ipVal;
} METRIC;

void dpSet(PARAMETER *p, char* cpName, var vMin, var vMax, var vStep)
{
	strcpy(p->sName, cpName);
	p->sName[31] = '\0';
	p->vMin = vMin;
	p->vMax = vMax;
	p->vStep = vStep;
	p->nMaxSteps = (int)(((p->vMax - p->vMin) / p->vStep) + 1.0);
	return;
}

int dpMaxSteps(PARAMETER *p1, PARAMETER *p2)
{
	return((p1->nMaxSteps) * (p2->nMaxSteps));
}

void dpGet(int nIndex, var *v1, var *v2, int *n1, int *n2, PARAMETER *p1, PARAMETER *p2)
{
	int nTotalMaxSteps = dpMaxSteps(p1, p2);
	int i = 0, j = 0, k = 0;
	for (i = 0; i < p1->nMaxSteps; i++)
	{
		for (j = 0; j < p2->nMaxSteps; j++)
		{
			if (k == nIndex)
			{
				*v1 = ((p1->vMin) + ((p1->vStep)*i));
				*n1 = i+1; // R style
				*v2 = ((p2->vMin) + ((p2->vStep)*j));
				*n2 = j+1; // R style
				return;
			}
			k++;
			}
		}
	return;
	}

int dpIterate(PARAMETER *p1, PARAMETER *p2)
{
	int nTotalMaxSteps = dpMaxSteps(p1, p2);
#ifdef litec_h
	return((int)(optimize(1, 1, nTotalMaxSteps, 1) - 1));
#endif
	return 0;
}

// Zorro user-friendly globals & wrapper functions
PARAMETER g_dp1, g_dp2;
int g_dpIndex;
#define MAX_METRICS 32
METRIC g_dpMetrics[MAX_METRICS];
void dp1Set(char* cpName, var vMin, var vMax, var vStep)
{
	dpSet(&g_dp1, cpName, vMin, vMax, vStep);
	return;
}
void dp2Set(char* cpName, var vMin, var vMax, var vStep)
{
	dpSet(&g_dp2, cpName, vMin, vMax, vStep);
	return;
}
var dp1Get()
{
	var v1 = 0., v2 = 0.;
	int n1 = 0, n2 =0;
	dpGet(g_dpIndex, &v1, &v2, &n1, &n2, &g_dp1, &g_dp2);
	return(v1);
}
var dp2Get()
{
	var v1 = 0., v2 = 0.;
	int n1 = 0, n2 =0;
	dpGet(g_dpIndex, &v1, &v2, &n1, &n2, &g_dp1, &g_dp2);
	return(v2);
}
int dp1GetIndex()
{
	var v1 = 0., v2 = 0.;
	int n1 = 0, n2 =0;
	dpGet(g_dpIndex, &v1, &v2, &n1, &n2, &g_dp1, &g_dp2);
	return(n1);
}
int dp2GetIndex()
{
	var v1 = 0., v2 = 0.;
	int n1 = 0, n2 =0;
	dpGet(g_dpIndex, &v1, &v2, &n1, &n2, &g_dp1, &g_dp2);
	return(n2);
}


int g_dpMetricCounter = 0;

void dpAddMetric(string sName, var* vpVal)
{
	strcpy(g_dpMetrics[g_dpMetricCounter].sName, sName);
	g_dpMetrics[g_dpMetricCounter].vpVal = vpVal;
	g_dpMetrics[g_dpMetricCounter].ipVal = NULL;
	g_dpMetricCounter++;
	return;
}
void dpAddMetric(string sName, int* ipVal)
{
	strcpy(g_dpMetrics[g_dpMetricCounter].sName, sName);
	g_dpMetrics[g_dpMetricCounter].vpVal = NULL;
	g_dpMetrics[g_dpMetricCounter].ipVal = ipVal;
	g_dpMetricCounter++;
	return;
}




void dpInitMetrics()
{
	static bool initialized = false;
	if(initialized) return;
	dpAddMetric("WinLong", &WinLong);
	dpAddMetric("WinShort", &WinShort);
	dpAddMetric("WinTotal", &WinTotal);
	dpAddMetric("LossLong", &LossLong);
	dpAddMetric("LossShort", &LossShort);
	dpAddMetric("LossTotal", &LossTotal);
	dpAddMetric("NumWinLong", &NumWinLong);
	dpAddMetric("NumWinShort", &NumWinShort);
	dpAddMetric("NumWinTotal", &NumWinTotal);
	dpAddMetric("NumLossLong",&NumLossLong);
	dpAddMetric("NumLossShort",&NumLossShort);
	dpAddMetric("NumLossTotal",&NumLossTotal);
	dpAddMetric("WinMaxLong",&WinMaxLong);
	dpAddMetric("WinMaxShort",&WinMaxShort);
	dpAddMetric("WinMaxTotal",&WinMaxTotal);
	dpAddMetric("LossMaxLong",&LossMaxLong);
	dpAddMetric("LossMaxShort",&LossMaxShort);
	dpAddMetric("LossMaxTotal",&LossMaxTotal);
	initialized = true;
}

void dpIterate()
{
	set(PARAMETERS);
	dpInitMetrics();
	g_dpIndex = dpIterate(&g_dp1, &g_dp2);
	return;
}


void dpCsvExport(string FileName)
{
	int i;
	char output[100];

	if(is(FIRSTINITRUN)) // make csv header
	{
		sprintf(output,"%s,%s,Index1,Index2,Asset,Algo,BarPeriod,TimeFrame",g_dp1.sName,g_dp2.sName);
		file_write(FileName,output,0);

		for(i=0; i<MAX_METRICS; i++)
		{
			if((!g_dpMetrics[i].vpVal) && (!g_dpMetrics[i].ipVal)){continue;}
			file_append(FileName,",",0);
			file_append(FileName,g_dpMetrics[i].sName,0);
		}
		file_append(FileName,"\n",0);
	} 
	if(is(EXITRUN))
	{
		sprintf(output,"%.8f,%.8f,%d,%d,%s,%s,%.8f,%d",dp1Get(),dp2Get(),dp1GetIndex(),dp2GetIndex(),Asset,Algo,BarPeriod,TimeFrame);
		file_append(FileName,output,0);
		for(i=0;i<MAX_METRICS;i++)
		{
			if(g_dpMetrics[i].vpVal)
			{
				sprintf(output,",%0.8f",*(g_dpMetrics[i].vpVal));
				file_append(FileName,output,0);
			}
			if(g_dpMetrics[i].ipVal)
			{
				sprintf(output,",%d",*(g_dpMetrics[i].ipVal));
				file_append(FileName,output,0);
			}
		}
		file_append(FileName,"\n",0);
	}
}


//int main()
//{
//	PARAMETER p1, p2;
//	dpSet(&p1, "Bananas", 1.0, 5.0, 0.1);
//	dpSet(&p2, "Cookies", 6.0, 7.0, 0.05);
//	int i = 0;
//	for (i = 0; i < 50; i++)
//	{
//		var v1, v2;
//		dpGet(i, &v1, &v2, &p1, &p2);
//		printf("\nv1: %0.2f, v2: %0.2f", v1, v2);
//		//std::cout << "v1: " << v1 << ", v2: " << v2 << std::endl;
//	}
//	printf("\n");
//
//}
//




// Luxor system by Jaeckle/Tomasini /////////////
// Make a system profitable by creative selection
// of start and end date 





function run()
{
	StartDate = 2005;
	EndDate = 2007;
	BarPeriod = 30;
	LookBack = 100;
	asset("GBP/USD");

// no trade costs...	
	Spread = 0;
	Slippage = 0;
	RollLong = RollShort = 0; 
	
	dp1Set("Fast", 1,  29, 1);
	dp2Set("Slow", 30, 60, 1);
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

	PlotWidth = 1000;
	PlotBars = 0; // plot all
	//dumpTradeData("LuxorDump002.csv", fast, slow);
	dpCsvExport("Luxor103.csv");
}