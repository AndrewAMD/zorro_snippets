string dayofweek_tostring(int dayofweek){
	const char* DayOfWeek = "err";
	switch(dayofweek){
		case MONDAY: 		DayOfWeek = "Mon"; break;
		case TUESDAY: 		DayOfWeek = "Tue"; break;
		case WEDNESDAY: 	DayOfWeek = "Wed"; break;
		case THURSDAY: 	DayOfWeek = "Thu"; break;
		case FRIDAY: 		DayOfWeek = "Fri"; break;
		case SATURDAY: 	DayOfWeek = "Sat"; break;
		case SUNDAY: 		DayOfWeek = "Sun"; break;
	}
	return DayOfWeek;
}
string datestring(int offset){
	static char Out[256];
	char* DayOfWeek = dayofweek_tostring(dow(offset));
	sprintf(Out,"%s %04d-%02d-%02d",DayOfWeek,year(NOW),month(NOW),day(NOW));
	return Out;
}
string timestring_eastern(int offset){
	static char Out[256];
	char* DayOfWeek = dayofweek_tostring(ldow(ET,offset));
	sprintf(Out,"%s %02d:%02d ET",DayOfWeek,lhour(ET,NOW),minute(NOW));
	return Out;
}

void run(){
	Now = 0;
	printf("\ntimestring_eastern(NOW): %s",timestring_eastern(NOW));
	quit("#done");
}
