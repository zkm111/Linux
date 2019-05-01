#ifndef __WORK_QUE__
#define __WORK_QUE__
#include "head.h"
typedef struct tag_node{
	int nd_sockfd;
	struct tag_node* nd_next;
}Node_t,*pNode_t;
typedef struct{
	pNode_t queHead,queTail;
	int queCapacity;
	int queSize;
	pthread_mutex_t queMutex;
}Que_t,*pQue_t;
void queInit(pQue_t,int);
void queInsert(pQue_t,pNode_t);
int queGet(pQue_t,pNode_t*);
#endif
