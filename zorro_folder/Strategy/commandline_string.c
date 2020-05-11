// Note: this only works if arbitrary input string is in double quotations.
// example commandline entry:
// zorro -u "alpha beta" -run commandline_string.c
// output: 'alpha beta'

// returns a temporary string with -u output, if it's in double quotations.
string get_u_output(){
	int len = strlen(report(3));
	string str = zalloc(len);
	strcpy(str,report(3));
	char* p = strstr(str,"-u \"");
	if(!p) return NULL;
	if(!strtok(p,"\"")) return NULL;
	return strtok(0,"\"");
}

int main(void){
	printf("\n\'%s\'",get_u_output());
}
