#include <default.c>

// Required for background process (bgp) functions
#include <windows.h>
#define PROCESS_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFFF)
#define STATUS_PENDING		((DWORD   )0x00000103L)    
#define STILL_ACTIVE       STATUS_PENDING
#define EXITCODE_ERROR 		200

// Returns bgp handle or 0 if failure.
HANDLE bgp_launch(string Program, string Options);

// Returns STILL_ACTIVE if process is still running. Else closes handle and returns code.
// On error, it returns EXITCODE_ERROR and prints the error code to the log.
DWORD bgp_exit_code(HANDLE h);

// Returns true if task completed or 0 if there was a problem.
bool bgp_launch_and_block(string Program, string Options);


#define MB_NUM_CLIENTS 7 // Number of clients allowed to run simultaneously
int mb_NumTasks = 0; // Counts number of batched items to do.
string mb_get_filename_program(int num){return strf(".\\Data\\%s%d_prog.txt",Script,num);}
string mb_get_filename_options(int num){return strf(".\\Data\\%s%d_opt.txt",Script,num);}
void mb_add_task(string Program, string Options){
	lock();
	char fnProg[64],fnOpt[64];
	strcpy(fnProg,mb_get_filename_program(mb_NumTasks));
	strcpy(fnOpt,mb_get_filename_options(mb_NumTasks));
	file_write(fnProg,Program,strlen(Program));
	file_write(fnOpt,Options,strlen(Options));
	unlock();
	mb_NumTasks++;
}
void mb_reset(){
	int i;
	lock();
	for(i=0;i<mb_NumTasks;i++){
		file_delete(mb_get_filename_program(i));
		file_delete(mb_get_filename_options(i));
	}
	unlock();
	mb_NumTasks = 0;
}
void mb_do_block(){
	HANDLE handles[MB_NUM_CLIENTS];
	memset(handles,0,MB_NUM_CLIENTS*sizeof(HANDLE));
	
	int task = 0; //task number
	while(true){
		//clear unused slots
		int s;
		for(s=0;s<MB_NUM_CLIENTS;s++){
			if(handles[s]){
				if(bgp_exit_code(handles[s])!=STILL_ACTIVE){
					handles[s] = 0;
				}
			}
		}
		
		// assign tasks to unused slots
		int openslots = 0;
		for(s=0;s<MB_NUM_CLIENTS;s++){
			if(!handles[s] && task<mb_NumTasks){
				string prog[64],opt[64];
				strcpy(prog,file_content(mb_get_filename_program(task)));
				strcpy(opt,file_content(mb_get_filename_options(task)));
				handles[s] = bgp_launch(prog,opt);
				task++;
			}
			if(!handles[s])
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
		mb_add_task("zorro",opt);
	}
	mb_do_block();
	
	quit("#Done");
}






HANDLE bgp_launch(string Program, string Options){
	DWORD pid = exec(Program,Options,0);
	if(!pid){
		print(TO_ANY,"\nCannot launch: %s %s",Program,ifelse((int)Options,Options,""));
		return 0;
	}
	print(TO_LOG|TRAINMODE,"#\nLaunched: %s %s (pid:%d)",Program,ifelse((int)Options,Options,""),pid);
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS,TRUE,pid);
	if(!h){
		print(TO_ANY,"\nCannot get handle for pid:%d",pid);
		return 0;
	}
	return h;
}

DWORD bgp_exit_code(HANDLE h){
	DWORD code;
	bool good = GetExitCodeProcess(h,&code);
	if(!good){
		int err = GetLastError();
		print(TO_ANY,"\nGetExitCodeProcess failed, error %d", GetLastError());
		if (err == 5) print(TO_ANY,": ERROR_ACCESS_DENIED");
		if (err == 6) print(TO_ANY,": ERROR_INVALID_HANDLE");
		return EXITCODE_ERROR;
	}
	if(code!=STILL_ACTIVE){
		CloseHandle(h);
	}
	return code;
}

bool bgp_launch_and_block(string Program, string Options){
	HANDLE h = bgp_launch(Program,Options);
	while(true){
		if(!wait(1000)) {CloseHandle(h); return false;} // wait one second
		DWORD code = bgp_exit_code(h);
		if(code==STILL_ACTIVE){
			// nothing to do
		} 
		else if (code == EXITCODE_ERROR){
			print(TO_ANY,"\nbgp_exit_code encountered an error.");
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}
