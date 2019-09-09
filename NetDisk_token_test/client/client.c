#include "function.h"

int main(int argc,char* argv[])
{
	Train_t train;
	args_check(argc,3);
	int socketFd;
	char select[10]={0};//选择用于登录还是注册的缓冲区
	socketFd=socket(AF_INET,SOCK_STREAM,0);
	ERROR_CHECK(socketFd,-1,"socket");
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);//点分十进制转为32位的网络字节序
	int ret;
	ret=connect(socketFd,(struct sockaddr*)&ser,sizeof(ser));
	ERROR_CHECK(ret,-1,"connect");
	printf("connect success\n");
	//选择注册还是登录
	printf("Register or Logon,Input R or L:\n");
	read(0,select,sizeof(select));
	train.dataLen=strlen(select)-1;
	strcpy(train.buf,select);
	send(socketFd,&train,4+train.dataLen,0);

	if(!strcmp(select,"R\n")){
		reg(socketFd,argv[1],argv[2]);//负责注册的函数
	}
	if(!strcmp(select,"L\n")){
		logon(socketFd,argv[1],argv[2]);//负责登录的函数
	}
	close(socketFd);
	return 0;
}

