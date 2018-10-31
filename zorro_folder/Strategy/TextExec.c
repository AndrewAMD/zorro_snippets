#include <default.c>
#include <windows.h>
#define PROCESS_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0xFFFF)
#define STATUS_PENDING                   ((DWORD   )0x00000103L)    
#define STILL_ACTIVE                        STATUS_PENDING

int main(){
	DWORD pid = exec("notepad",0,0); // open notepad
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS,TRUE,pid);
	
	printf("\nhandle: %d",h);
	while(true){
		if(!wait(5000)) break; // wait five seconds
		DWORD code;
		bool good = GetExitCodeProcess(h,&code);
		if(!good){
			int err = GetLastError();
			printf("\nGetExitCodeProcess failed, error %d", GetLastError());
			if (err == 5) printf(": ERROR_ACCESS_DENIED");
			if (err == 6) printf(": ERROR_INVALID_HANDLE");
			break;
		}
		if(code==STILL_ACTIVE){
			printf("\nNotepad is still open...");
		} else {
			printf("\nNotepad is finally closed!");
			break;
		}
	}
	CloseHandle(h);
	return 0;
}
