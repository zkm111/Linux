#include "function.h"
int recv_file(int socketFd)
{
	int ret;
	int dataLen;
	char buf[1000]={0};
	//接收文件名
	recvCycle(socketFd,&dataLen,4);
	recvCycle(socketFd,buf,dataLen);
	int fd;
	fd=open(buf,O_CREAT|O_WRONLY,0666);
	ERROR_CHECK(fd,-1,"open");
	//接文件大小
	off_t fileSize=0,oldSize=0,downLoadSize=0,sliceSize;
	recvCycle(socketFd,&dataLen,4);
	recvCycle(socketFd,&fileSize,dataLen);
	//接文件内容
	sliceSize=fileSize/10000;
	while(1)
	{
		ret=recvCycle(socketFd,&dataLen,4);
		if(-1==ret)
		{
			printf("server is update\n");
			break;
		}
		if(dataLen>0)
		{
			ret=recvCycle(socketFd,buf,dataLen);
			if(-1==ret)
			{
				printf("server is update\n");
				break;
			}
			write(fd,buf,dataLen);
			downLoadSize+=dataLen;
			if(downLoadSize-oldSize>sliceSize)
			{
//				printf("\r%5.2f%%",(float)downLoadSize/fileSize*100);
//				fflush(stdout);
				oldSize=downLoadSize;
			}
		}else{
//			printf("\r100%%   \n");
			break;
		}
	}
	close(fd);
	return 0;
}
