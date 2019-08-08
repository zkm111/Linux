#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"
typedef struct{
	int dataLen;
	char buf[100];
}train;
#define FILENAME "file"
int tranFile(int new_fd);
#endif
