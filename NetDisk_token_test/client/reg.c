#include "function.h"
int reg(int socketFd,char* ip,char* port)
{
	Train_t train; 
	char buf[100]={0};//为小火车赋值
	char *password;
	printf("Input a new name:\n");
	read(0,buf,sizeof(buf));
	//发新用户名给服务器
	train.dataLen=strlen(buf)-1;
	strcpy(train.buf,buf);
	send(socketFd,&train,4+train.dataLen,0);
	//密码发给服务器
	memset(buf,0,sizeof(buf));
	password=getpass("Input your password:");
	strcpy(buf,password);
	train.dataLen=strlen(buf);
	strcpy(train.buf,buf);
	send(socketFd,&train,4+train.dataLen,0);
	logon(socketFd,ip,port);//走完注册函数转到登录函数
	return 0;
}
