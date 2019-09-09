#include "function.h"
void *threadFunc(void* p)
{
	pClient_th pc=(pClient_th)p;
	Train_t train;
	int socketFd;
	socketFd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(pc->port));
	ser.sin_addr.s_addr=inet_addr(pc->ip);
	connect(socketFd,(struct sockaddr*)&ser,sizeof(ser));
	//以上与服务器建立连接，之后要让服务器能把自己与客户端的主线程区分
	train.dataLen=4;
	strcpy(train.buf,"gets");
	send(socketFd,&train,4+train.dataLen,0);
	//发送username
	train.dataLen=strlen(pc->username);
	strcpy(train.buf,pc->username);
	send(socketFd,&train,4+train.dataLen,0);
	//发送token
	train.dataLen=strlen(pc->token);
	strcpy(train.buf,pc->token);
	send(socketFd,&train,4+train.dataLen,0);
	//发送filename
	train.dataLen=strlen(pc->filename);
	strcpy(train.buf,pc->filename);
	send(socketFd,&train,4+train.dataLen,0);
	//开始准备接收文件
	recv_file(socketFd);	
	pthread_exit((void*)3);
}
int command(int socketFd,char* name,char* ip,char* port,char* token)
{
	//结构体child为了传给子线程
	Client_th child;
	child.port=port;
	child.ip=ip;
	child.username=name;
	child.token=token;

	Train_t train;
	char send_buf[200]={0};
	char recv_buf[200]={0};
	int dataLen=0;
	while(1){
		memset(send_buf,0,sizeof(send_buf));
		memset(recv_buf,0,sizeof(recv_buf));
		printf("\nNow,you can input your command:\n");
		read(0,send_buf,sizeof(send_buf));
		if(!strncmp(send_buf,"cd",2)||!strncmp(send_buf,"mkdir",5)){
			train.dataLen=strlen(send_buf)-1;
			strcpy(train.buf,send_buf);
			send(socketFd,&train,4+train.dataLen,0);
			continue;
		}
		if(!strncmp(send_buf,"gets",4)){//下载文件模块,开一个新的线程和服务器连接
//			int iRet;
			char* p=send_buf;
			p=p+5;
			p[strlen(p)-1]='\0';
			strcpy(child.filename,p);
			pthread_t pthID;
			pthread_create(&pthID,NULL,threadFunc,&child);
		//	pthread_join(pthID,(void**)&iRet);
		//	printf("iRet=%d\n",iRet);
			continue;
		}
		train.dataLen=strlen(send_buf)-1;
		strcpy(train.buf,send_buf);
		send(socketFd,&train,4+train.dataLen,0);
		recvCycle(socketFd,&dataLen,4);
		recvCycle(socketFd,recv_buf,dataLen);
		printf("%s\n",recv_buf);

	}

	return 0;
}
