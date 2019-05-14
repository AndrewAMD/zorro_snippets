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

int main(){
	HANDLE h = bgp_launch("notepad",0);
	printf("\nhandle: %d",h);
	while(true){
		if(!wait(5000)) break; // wait five seconds
		DWORD code = bgp_exit_code(h);
		if(code==STILL_ACTIVE){
			printf("\nNotepad is still open...");
		} 
		else if (code == EXITCODE_ERROR){
			printf("\nbgp_exit_code encountered an error.");
			break;
		}
		else
		{
			printf("\nNotepad is finally closed!");
			break;
		}
	}
	return 0;
}


HANDLE bgp_launch(string Program, string Options){
	DWORD pid = exec(Program,Options,0);
	if(!pid){
		printf("\nCannot launch: %s %s",Program,ifelse((int)Options,Options,""));
		return 0;
	}
	printf("\nLaunched: %s %s (pid:%d)",Program,ifelse((int)Options,Options,""),pid);
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS,TRUE,pid);
	if(!h){
		printf("\nCannot get handle for pid:%d",pid);
		return 0;
	}
	return h;
}

bool bgp_exit_code(HANDLE h){
	DWORD code;
	bool good = GetExitCodeProcess(h,&code);
	if(!good){
		int err = GetLastError();
		printf("\nGetExitCodeProcess failed, error %d", GetLastError());
		if (err == 5) printf(": ERROR_ACCESS_DENIED");
		if (err == 6) printf(": ERROR_INVALID_HANDLE");
		return EXITCODE_ERROR;
	}
	if(code!=STILL_ACTIVE){
		CloseHandle(h);
	}
	return code;
}
