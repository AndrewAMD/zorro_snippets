/*
example1.csv file
** must be located in History folder
** Date must start with 40000 to prevent errors.

DATE,id,name,age,height_cm
40000,10,Andrew,35,178
40001,11,Neal,60,190
40002,21,Boris,41,165
40003,22,Brian,33,179

*/


int main(){
	string format = "%w,i,sss,i,i";
	string filename = "example1.csv";
	int h = 1;
	int num_records = dataParse(h,format,filename);
	
	printf("\nnum_records: %d",num_records);
	
	int i;
	for(i=0; i<4; i++){
		int id = dataInt(h,i,1);
		string name = dataStr(h,i,2);
		int age = dataInt(h,i,5);
		int height_cm = dataInt(h,i,6);
		printf("\ni: %d, id: %d, name: %s, age: %d, height_cm: %d",i,id,name,age,height_cm);
	}
	
}

/*

Expected output:

num_records: 4
i: 0, id: 10, name: Andrew, age: 35, height_cm: 178
i: 1, id: 11, name: Neal, age: 60, height_cm: 190
i: 2, id: 21, name: Boris, age: 41, height_cm: 165
i: 3, id: 22, name: Brian, age: 33, height_cm: 179

*/
