#include<default.c>
#include<stdio.h>

#define RB_CAP 8
typedef struct RINGBUFFER{
	char buf[RB_CAP];
	int pos;  			// beginning of all data.
	int size;    		// size of pushed data
} RINGBUFFER;

typedef struct RB_EXPOSURE{
	char* p1; // ptr 1
	char* p2; // ptr 2
	int s1; // size at ptr 1
	int s2; // size at ptr 2
} RB_EXPOSURE;

typedef int RB_ERR;
#define RB_ERR_OK 				   0
#define RB_ERR_BAD_ARGUMENT	-101
#define RB_ERR_NULL_POINTER	-102
#define RB_ERR_BUF_TOO_SMALL	-103
#define RB_ERR_EXCESSIVE_POP	-104
#define RB_ERR_MEMCPY_FAILURE	-105

RB_ERR rb_init(RINGBUFFER* pRB){
	if(!pRB){
		return RB_ERR_NULL_POINTER;
	}
	memset(pRB,0,sizeof(RINGBUFFER));
	return RB_ERR_OK;
}

RB_ERR rb_push(RINGBUFFER* pRB, RB_EXPOSURE* pEX, char* pBufIn, int len){
	if(!pRB || !pEX){
		return RB_ERR_NULL_POINTER;
	}
	if(len <= 0){
		return RB_ERR_BAD_ARGUMENT;
	}
	if((pRB->size + len) > RB_CAP){
		return RB_ERR_BUF_TOO_SMALL;
	}
	
	memset(pEX,0,sizeof(RB_EXPOSURE));
	int pos1 = pRB->pos + pRB->size;
	pos1 -= RB_CAP * (pos1/RB_CAP); //wrap-around buffer
	pEX->p1 = pRB->buf + pos1;
	pEX->s1 = min(RB_CAP-pos1, len);
	pEX->s2 = len - pEX->s1;
	if(pEX->s2){
		pEX->p2 = pRB->buf;
	}
	if(pBufIn){
		int err = memcpy_s(pEX->p1,pEX->s1,pBufIn,pEX->s1);
		if(err){
			return RB_ERR_MEMCPY_FAILURE;
		}
		if(pEX->s1){
			err = memcpy_s(pEX->p2,pEX->s2,pBufIn+pEX->s1,pEX->s2);
			if(err){
				return RB_ERR_MEMCPY_FAILURE;
			}
		}
	}
	pRB->size += len;
	return RB_ERR_OK;
}

RB_ERR rb_pop(RINGBUFFER* pRB, char* pBufOut, int len){
	if(!pRB){
		return RB_ERR_NULL_POINTER;
	}
	if(len <= 0){
		return RB_ERR_BAD_ARGUMENT;
	}
	if((pRB->size - len) < 0){
		return RB_ERR_EXCESSIVE_POP;
	}
	if(pBufOut){
		char *p1 = NULL, *p2 = NULL; // pointers to copy-from
		int s1=0, s2=0;
		p1 = pRB->buf + pRB->pos;
		s1 = min(RB_CAP-pRB->pos, len);
		s2 = len - s1;
		if(s2){
			p2 = pRB->buf;
		}
		int err = memcpy_s(pBufOut,s1,p1,s1);
		if(err){
			return RB_ERR_MEMCPY_FAILURE;
		}
		if(s1){
			err = memcpy_s(pBufOut+s1,s2,p2,s2);
			if(err){
				return RB_ERR_MEMCPY_FAILURE;
			}
		}
	}
	pRB->size -= len;
	pRB->pos += len;
	pRB->pos -= RB_CAP * (pRB->pos/RB_CAP); //wrap-around buffer
	return RB_ERR_OK;
}


void analyze(int err, RINGBUFFER* pRB, RB_EXPOSURE* pEX, char* pPop){
	if(err){printf("\nerr: %d",err);	return;}
	printf("\npos: %d, size: %d, s1: %d, s2: %d", pRB->pos, pRB->size, pEX->s1, pEX->s2);
	
	int i;
	for(i=0;i<8;i++){
		if(!i){
			printf(" array: %c",pRB->buf[i]);
		}
		else{
			printf(",%c",pRB->buf[i]);
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
	
	if(pPop){
		if(strlen(pPop)){
			printf(", (%s)",pPop);
		}
	}
	
}


int main(){
	RINGBUFFER rb;	
	RB_EXPOSURE ex;
	char pop[9];
	memset(pop,0,9);
	int err = 0;
	
	rb_init(&rb);
	memset(&ex,0,sizeof(RB_EXPOSURE));
	printf("\ninitializing...");
	analyze(err,&rb,&ex,pop);
	
	printf("\npush 6...");
	err = rb_push(&rb,&ex,"012345",6);
	analyze(err,&rb,&ex,pop);
	
	printf("\npop 3...");
	err = rb_pop(&rb,pop,3);
	analyze(err,&rb,&ex,pop);
	memset(pop,0,9);
	
	printf("\npush 5...");
	err = rb_push(&rb,&ex,"67890",5);
	analyze(err,&rb,&ex,pop);
	
	printf("\npop 6...");
	err = rb_pop(&rb,pop,6);
	analyze(err,&rb,&ex,pop);
	memset(pop,0,9);
	
	printf("\npush 4...");
	err = rb_push(&rb,&ex,"1234",4);
	analyze(err,&rb,&ex,pop);
	
	printf("\npop 6...");
	err = rb_pop(&rb,pop,6);
	analyze(err,&rb,&ex,pop);
	memset(pop,0,9);
	
}
