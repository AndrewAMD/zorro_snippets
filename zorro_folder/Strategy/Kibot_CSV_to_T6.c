#define ASSETNAME "ABT"

char* get_FileIn(string sAsset){
	return strf(".\\History\\%s.csv",sAsset);
}
char* get_FileOut(string sAsset, int yr){
	return strf(".\\History\\%s_%d.t6",sAsset,yr);
}

char* Format = "%m/%d/%Y,%H:%M,f3,f1,f2,f4,f6"; 

/*csv format (no headers)
05/27/2016,08:00,35.96,35.96,35.96,35.96,216
05/27/2016,08:43,36.02,36.02,36.02,36.02,216
05/27/2016,08:44,36.02,36.02,36.02,36.02,433
05/27/2016,08:45,36.02,36.02,36.02,36.02,541
05/27/2016,08:49,36.02,36.02,36.02,36.02,865
05/27/2016,08:50,36.02,36.02,36.02,36.02,216

typedef struct T6
{
  DATE  time; // UTC timestamp of the close, OLE date/time format
  float fHigh,fLow;	
  float fOpen,fClose;	
  float fVal,fVol; // additional data, like ask-bid spread, volume etc.
} T6;

*/

void main(void){
	int h1 = 1, h2 = 2;
	int n = dataParse(h1,Format,get_FileIn(ASSETNAME));
	printf("\n%d records parsed",n);
	if(!n){
		printf("\nFailed!");
		return;
	}
	//convert to UTC and get year boundaries
	int yr_min = 9999, yr_max = 0;
	int i;
	var hr = 1;
	hr /= 24;
	for(i=0;i<n;i++){
		var d = dataVar(h1,i,0);
		Now = d;
		yr_min = min(year(NOW),yr_min);
		yr_max = max(year(NOW),yr_max);
		if(dst(ET,NOW)){//EDT: -4 hours
			dataSet(h1,i,0,d+(4*hr));
		}
		else{ //EST: -5 hours
			dataSet(h1,i,0,d+(5*hr));
		}
		Now = 0;
	}
	dataSort(h1);
	
	
	//save by year
	int yr=0;
	for(yr=yr_min; yr<=yr_max; yr++){
		dataNew(h2,n,7);
		int j=0;
		for(i=0;i<n;i++){
			Now = dataVar(h1,i,0);
			if(year(NOW)==yr){
				dataSet(h2,j,0,dataVar(h1,i,0));
				dataSet(h2,j,1,dataVar(h1,i,1));
				dataSet(h2,j,2,dataVar(h1,i,2));
				dataSet(h2,j,3,dataVar(h1,i,3));
				dataSet(h2,j,4,dataVar(h1,i,4));
				dataSet(h2,j,5,dataVar(h1,i,5));
				dataSet(h2,j,6,dataVar(h1,i,6));
				j++;
			}
			Now = 0;
		}
		dataSave(h2,get_FileOut(ASSETNAME,yr),0,j);
	}
	
	printf("\nDone!");
}
