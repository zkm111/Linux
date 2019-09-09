#include "factory.h"
int client_gets(int newFd)
{
	int code=0;
	char username[50]={0};
	char token[1000]={0};
	char filenmae[100]={0};
	int dataLen;
	//接用户名
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,username,dataLen);
	//接token
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,token,dataLen);
	//接filename
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,filenmae,dataLen);
	//code记录该用户当前目录的code值；
	client_get_code(username,token,&code);
	//开始发送文件给客户端
	tranFile(newFd,filenmae);	
	return 0;

}
