#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"
typedef struct{
	int dataLen;
	char buf[1000];
}Train_t;
int tranFile(int new_fd,char*);
#endif
