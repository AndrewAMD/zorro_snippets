// Note: this only works if arbitrary input is first!
// i.e. no other quoted items are allowed unless they come later in the command line.
// example commandline entry:
// zorro -u "alpha beta" -run commandline_string.c
// output: 'alpha beta'

// returns a temporary string with -u output
string get_u_output(){
	int len = strlen(report(3));
	string str = zalloc(len);
	strcpy(str,report(3));
	strtok(str,"\"");
	return strtok(0,"\"");
}

int main(void){
	printf("\n\'%s\'",get_u_output());
}
