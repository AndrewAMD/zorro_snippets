#include<default.c>
#include<stdio.h>

#define SB_CAP 8
typedef struct SENDBUFFER{
	char buf[SB_CAP];
	int pos;  			// beginning of all data.
	int size;    		// size of pushed data
} SENDBUFFER;

typedef struct SB_EXPOSURE{
	char* p1; // ptr 1
	char* p2; // ptr 2
	int s1; // size at ptr 1
	int s2; // size at ptr 2
} SB_EXPOSURE;

typedef int SB_ERR;
#define SB_ERR_OK 				   0
#define SB_ERR_BAD_ARGUMENT	-101
#define SB_ERR_NULL_POINTER	-102
#define SB_ERR_BUF_TOO_SMALL	-103
#define SB_ERR_EXCESSIVE_PULL	-104
#define SB_ERR_MEMCPY_FAILURE	-105

SB_ERR sb_init(SENDBUFFER* pSB){
	if(!pSB){
		return SB_ERR_NULL_POINTER;
	}
	memset(pSB,0,sizeof(SENDBUFFER));
	return SB_ERR_OK;
}

SB_ERR sb_push(SENDBUFFER* pSB, SB_EXPOSURE* pEX, char* data, int len){
	if(!pSB || !pEX || !data){
		return SB_ERR_NULL_POINTER;
	}
	if(len <= 0){
		return SB_ERR_BAD_ARGUMENT;
	}
	if((pSB->size + len) > SB_CAP){
		return SB_ERR_BUF_TOO_SMALL;
	}
	memset(pEX,0,sizeof(SB_EXPOSURE));
	int pos1 = pSB->pos + pSB->size;
	pos1 -= SB_CAP * (pos1/SB_CAP); //wrap-around buffer
	pEX->p1 = pSB->buf + pos1;
	pEX->s1 = min(SB_CAP-pos1, len);
	pEX->s2 = len - pEX->s1;
	if(pEX->s2){
		pEX->p2 = pSB->buf;
	}
	int err = memcpy_s(pEX->p1,pEX->s1,data,pEX->s1);
	if(err){
		return SB_ERR_MEMCPY_FAILURE;
	}
	if(pEX->s1){
		err = memcpy_s(pEX->p2,pEX->s2,data+pEX->s1,pEX->s2);
		if(err){
			return SB_ERR_MEMCPY_FAILURE;
		}
	}
	pSB->size += len;
	return SB_ERR_OK;
}

SB_ERR sb_pop(SENDBUFFER* pSB, int len){
	if(!pSB){
		return SB_ERR_NULL_POINTER;
	}
	if(len <= 0){
		return SB_ERR_BAD_ARGUMENT;
	}
	if((pSB->size - len) < 0){
		return SB_ERR_EXCESSIVE_PULL;
	}
	pSB->size -= len;
	pSB->pos += len;
	pSB->pos -= SB_CAP * (pSB->pos/SB_CAP); //wrap-around buffer
	return SB_ERR_OK;
}


void analyze(int err, SENDBUFFER* pSB, SB_EXPOSURE* pEX){
	if(err){printf("\nerr: %d",err);	return;}
	printf("\npos: %d, size: %d, s1: %d, s2: %d", pSB->pos, pSB->size, pEX->s1, pEX->s2);
	
	int i;
	for(i=0;i<8;i++){
		if(!i){
			printf(" array: %c",pSB->buf[i]);
		}
		else{
			printf(",%c",pSB->buf[i]);
		}
	}
	char s1[9], s2[9];
	memset(s1,0,9);
	memset(s2,0,9);
	memcpy(s1,pEX->p1,pEX->s1);
	if(pEX->s2){
		memcpy(s2,pEX->p2,pEX->s2);
	}
	printf(" [%s] [%s]",s1,s2);
	
}

int main(){
	SENDBUFFER sb;	
	SB_EXPOSURE ex;
	int err = 0;
	
	sb_init(&sb);
	memset(&ex,0,sizeof(SB_EXPOSURE));
	printf("\ninitializing...");
	analyze(err,&sb,&ex);
	
	printf("\npush 7...");
	err = sb_push(&sb,&ex,"0123456",7);
	analyze(err,&sb,&ex);
	
	printf("\npop 7...");
	err = sb_pop(&sb,7);
	analyze(err,&sb,&ex);
	
	printf("\npush 8...");
	err = sb_push(&sb,&ex,"78901234",8);
	analyze(err,&sb,&ex);
	
	printf("\npop 5...");
	err = sb_pop(&sb,5);
	analyze(err,&sb,&ex);
	
	printf("\npush 4...");
	err = sb_push(&sb,&ex,"5678",4);
	analyze(err,&sb,&ex);
	
}
