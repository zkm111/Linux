#include "function.h"
int logon(int socketFd,char* ip,char* port)
{
	printf("\nWelcome to logon!\n");
	Train_t train;
	time_t timep;
	time(&timep);
	char* time=ctime(&timep);
	char token[100]={0};//保存当前时间和用户名crypt的结果
	int dataLen;
	char name[100]={0};
	char name_token[100]={0};
	char buf[100]={0};//为小火车赋值
	char *password;
	char salt[100]={0};//接受salt
	char *result;//存crypt的返回值
A:  memset(buf,0,sizeof(buf));
	memset(salt,0,sizeof(salt));
	printf("Input your name:\n");
	read(0,buf,sizeof(buf));
	strcpy(name,buf);//为了token使用用户名
	//发送用户名给服务器
	train.dataLen=strlen(buf)-1;
	strcpy(train.buf,buf);
	send(socketFd,&train,4+train.dataLen,0);
	//接受服务器反馈
	recvCycle(socketFd,&dataLen,4);
	memset(buf,0,sizeof(buf));
	recvCycle(socketFd,buf,dataLen);
	if(!strcmp(buf,"Ok")){
		goto B;
	}else{
		printf("\nThe name doesn't exist,try again\n");
		goto A;
	}
	//接受服务器的salt
B:	recvCycle(socketFd,&dataLen,4);
	recvCycle(socketFd,salt,dataLen);
	//让用户输入密码，本地把密码和salt用crypt组合，密码在前，salt在后
	memset(buf,0,sizeof(buf));
	password=getpass("Input your password:");
	result=crypt(password,salt);
	strcpy(buf,result);
	train.dataLen=strlen(buf);
	strcpy(train.buf,buf);
	send(socketFd,&train,4+train.dataLen,0);
	//接受服务器对于登录的确认信息
	recvCycle(socketFd,&dataLen,4);
	memset(buf,0,sizeof(buf));
	recvCycle(socketFd,buf,dataLen);
	if(!strcmp(buf,"Logon Success")){//这里是客户端登录成功，用crypt把当前时间和用户名加密生成token
		sprintf(name_token,"%s%s","$6$",name);
		strcpy(token,crypt(time,name_token));
		train.dataLen=strlen(token);
		strcpy(train.buf,token);
		send(socketFd,&train,4+train.dataLen,0);//把token发给客户端
		puts(train.buf);
		printf("\n%s\n",buf);
		name[strlen(name)-1]='\0';
		command(socketFd,name,ip,port,token);//登录成功，可以输入命令了
		return 1;
	}else{
		printf("\nPassword mistake,try again\n");
		goto A;
	}
	return 0;
}
