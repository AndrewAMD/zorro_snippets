#include <default.c>

#define MB_NUM_CLIENTS 7 // Number of background Zorro clients.
int mb_NumTasks = 0; // Counts number of batched items to do.
string mb_get_filename_program(int num){return strf(".\\Data\\%s%d_prog.txt",Script,num);}
string mb_get_filename_options(int num){return strf(".\\Data\\%s%d_opt.txt",Script,num);}
void mb_add_task(string Options){
	lock();
	char fnOpt[64];
	strcpy(fnOpt,mb_get_filename_options(mb_NumTasks));
	file_write(fnOpt,Options,strlen(Options));
	unlock();
	mb_NumTasks++;
}
void mb_reset(){
	int i;
	lock();
	for(i=0;i<mb_NumTasks;i++){
		file_delete(mb_get_filename_options(i));
	}
	unlock();
	mb_NumTasks = 0;
}
void mb_do_block(){
	bool clients_occupied[MB_NUM_CLIENTS]; // Core = 2,3,4, etc...
	memset(clients_occupied,0,MB_NUM_CLIENTS*sizeof(bool));
	
	int task = 0; //task number
	while(true){
		//clear unused slots
		int s;
		for(s=0;s<MB_NUM_CLIENTS;s++){
			int id = s+2;
			if(!zStatus(id)){
				clients_occupied[s] = false;
			}
		}
		
		// assign tasks to unused slots
		int openslots = 0;
		for(s=0;s<MB_NUM_CLIENTS;s++){
			if(!clients_occupied[s] && task<mb_NumTasks){
				int id = s + 2;
				string opt[64];
				strcpy(opt,file_content(mb_get_filename_options(task)));
				if(zOpen(id,opt)){
					clients_occupied[s] = true;
				}
				else{
					printf("\nzOpen failure!");
				}
				task++;
			}
			if(!clients_occupied[s])
				openslots++;
		}
		if(openslots==MB_NUM_CLIENTS && task==mb_NumTasks){
			//done
			printf("\nmb_do_block() has completed");
			mb_reset();
			return;
		}
		
		if(!wait(1000)){
			printf("\nmb_do_block() aborted");
			mb_reset();
			return;
		}
	}
}


#define ClientNum Command[0]
void run(){
	zInit(MB_NUM_CLIENTS+1,0);
	if(ClientNum){
		static char thisScript[24];
		sprintf(thisScript,"%s_%d",Script,ClientNum);
		Script = thisScript;
		LogNumber = ClientNum;
		printf("\nClient number: %d",ClientNum);
		printf("\nWaiting ten seconds...");
		wait(10000);
		quit("#Done");
		return;
	}
	printf("\nMaster instance");
	int i;
	for(i=1;i<=18;i++){
		char opt[64];
		sprintf(opt,"-run %s -i %d -quiet",Script,i);
		mb_add_task(opt);
	}
	mb_do_block();
	if(Core==1) {
		zClose(0);
	}
	quit("#Done");
}

