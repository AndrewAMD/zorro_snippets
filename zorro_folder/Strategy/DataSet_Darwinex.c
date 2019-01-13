#include <default.c>

const char* can_get_darwinex_key();
const char* can_get_darwinex_json(const char* productName, const int bar_period, const int start, const int end);
int epoch(int yyyymmdd){return utm(dmy(yyyymmdd));}

// typedef struct T6
// {
	// DATE	time;	
	// float fHigh, fLow;	// (f1,f2)
	// float fOpen, fClose;	// (f3,f4)	
	// float fVal, fVol; // optional data, like spread and volume (f5,f6)
// } T6; // 6-stream tick, .t6 file content

int main(){
	Verbose = 7;
	if(!is(INITRUN))
		return 0;
	
	const char* key = can_get_darwinex_key();
	if(key){
		printf("\nkey: %s",key);
	} else {
		printf("\nno key!");
		return 1;
	}
	
	printf("\n%d",epoch(20190113));
	
	const char* jsonstr = can_get_darwinex_json("SCS", 1440, 20180101, 20190113);
	const char* filename = ".\\History\\scs_1d.json";
	file_write(filename,jsonstr,strlen(jsonstr));
	printf("\nsaved file to \"%s\"",filename);
	
	// let's load the json file we just saved
	
	int h = 1;
	const char* format = "candles,timestamp,%t,max,min,open,close";
	int numparsed = dataParse(h,format,filename);
	printf("\nnumparsed = %d",numparsed);
	
	// char t6file[64];
	// memset(t6file,0,64);
	// memcpy(t6file,filename,strlen(filename)-3); // remove "csv"
	// strcat(t6file,"t6");
	// printf("saving t6 file: %s",t6file);
	// dataSave(h,t6file);
	
	// free memory
	// dataNew(h,0,0);
	
	printf("\nAll done.");
	return 0;
}

// returns darwin key string
const char* can_get_darwinex_key(){
	static char* out = 0;
	if(out) return out; // no need to repeat
	
	// read zorro.ini file
	char* ini = file_content("Zorro.ini");
	// parse for darwin key
	
	char* output = strtext(ini, "DarwinexKey", "");
	
	// check for bad outcome, if so return false
	if(!strlen(output)) return NULL;
	
	// output darwin key return true
	out = malloc(strlen(output)+1);
	if(!out) return NULL;
	strcpy(out,output);
	return out;
}


const char* can_get_darwinex_json(const char* productName, const int bar_period, const int start, const int end){
	static char* out = 0;
	if(out) {
		free(out);
		out = 0;
	}
	
	const char* apikey = can_get_darwinex_key();
	if(!apikey){
		printf("\nCannot load Darwin Key! Check Zorro.ini");
		return NULL;
	}
	
	char resolution[4];
	memset(resolution,0,4);
	switch(bar_period){
		case 1:		strcpy(resolution,"1m"); 	break;
		case 5:		strcpy(resolution,"5m"); 	break;
		case 15:		strcpy(resolution,"15m"); 	break;
		case 30:		strcpy(resolution,"30m"); 	break;
		case 60:		strcpy(resolution,"1h"); 	break;
		case 240:	strcpy(resolution,"4h"); 	break;
		case 1440:	strcpy(resolution,"1d"); 	break;
		default: // leave blank
	}
	
	char url[512];
	sprintf(
		url,
		"https://api.darwinex.com/darwininfo/products/%s/candles?from=%d&to=%d",
		productName,
		epoch(start),
		epoch(end)
		);
	if(strlen(resolution)){
		strcat(url,strf("&resolution=%s",resolution));
	}
	
	char header[512];
	sprintf(
		header,
		"Authorization: Bearer %s",
		can_get_darwinex_key()
	);
	
	int h = http_send(url,0,header);
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







