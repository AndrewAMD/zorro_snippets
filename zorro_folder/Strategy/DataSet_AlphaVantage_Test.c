#include <default.c>

const char* can_get_av_key();
const char* get_av_intraday_csv(const char* symbol, int barsize, bool compact);

// typedef struct T6
// {
	// DATE	time;	
	// float fHigh, fLow;	// (f1,f2)
	// float fOpen, fClose;	// (f3,f4)	
	// float fVal, fVol; // optional data, like spread and volume (f5,f6)
// } T6; // 6-stream tick, .t6 file content

int run(){
	Verbose = 7;
	if(!is(INITRUN))
		return 0;
	
	printf("\nhello, world");
	const char* key = can_get_av_key();
	if(key){
		printf("\nkey: %s",key);
	} else {
		printf("\nno key!");
	}
	
	const char* csvstr = get_av_intraday_csv("IBM",1,false);
	const char* filename = "ibm_1m_full.csv";
	char fullfilename[64];
	sprintf(fullfilename,".\\History\\%s",filename);
	file_write(fullfilename,csvstr,strlen(csvstr));
	printf("\nsaved file \"%s\"",filename);
	
	// let's load the csv file we just saved
	
	int h = 1;
	const char* format = "1%Y-%m-%d %H:%M:%S,f3,f1,f2,f4,f6";
	int numparsed = dataParse(h,format,filename);
	printf("\nnumparsed = %d",numparsed);
	
	char t6file[64];
	memset(t6file,0,64);
	memcpy(t6file,filename,strlen(filename)-3); // remove "csv"
	strcat(t6file,"t6");
	printf("saving t6 file: %s",t6file);
	dataSave(h,t6file);
	
	// free memory
	dataNew(h,0,0);
	
	
	return 0;
}




// returns AV key string
const char* can_get_av_key(){
	static char* out = 0;
	if(out) return out; // no need to repeat
	
	
	
	// read zorro.ini file
	char* ini = file_content("Zorro.ini");
	// parse for av key
	
	char* output = strtext(ini, "AVApiKey", "");
	
	// check for bad outcome, if so return false
	if(!strlen(output)) return NULL;
	
	// output av key return true
	out = malloc(strlen(output)+1);
	if(!out) return NULL;
	strcpy(out,output);
	return out;
}


const char* get_av_intraday_csv(const char* symbol, int barsize, bool compact){
	static char* out = 0;
	if(out) {
		free(out);
		out = 0;
	}
	
	const char* fn = "TIME_SERIES_INTRADAY";
	char interval[6];
	switch(barsize){
	case 1:
	case 5:
	case 15:
	case 30:
	case 60:
		sprintf(interval,"%dmin",barsize); // e.g. 1min, 60min
		break;
	default:
		printf("\nBar size %d unavailable");
		return NULL;
	}
	char* outputsize = 0;
	if(compact) outputsize = "compact";
	else outputsize = "full";
	
	const char* datatype = "csv";
	const char* apikey = can_get_av_key();
	if(!apikey){
		printf("\nCannot load AlphaVantage Key! Check Zorro.ini");
		return NULL;
	}
	
	char url[512];
	sprintf(
		url,
		"https://www.alphavantage.co/query?function=%s&symbol=%s&interval=%s&outputsize=%s&apikey=%s&datatype=%s",
		fn,
		symbol,
		interval,
		outputsize,
		apikey,
		datatype);
	//printf("\nurl: %s", url);
	
	int h = http_send(url,0,0);
	int bytes = 0;
	for(;;){
		bytes = http_status(h);
		if(bytes==0) // still in progress
		{
			if(!wait(100))  // pause 100ms, abort if necessary
			{
				http_free(h);
				return NULL;
			}
		} 
		else if ((bytes == -1) || (bytes == -2)) // failed or invalid
		{
			http_free(h);
			return NULL;
		}
		else // transfer completed
			break;
	}
	
	printf("\nbytes: %d",bytes);
	//get the string
	out = malloc(bytes);
	memset(out,0,bytes);
	http_result(h,out,bytes);
	
	
	
	//free the handle
	http_free(h);
	
	return out;
}



