#include "factory.h"
int logon(int newFd)
{
	Train_t train;
	memset(&train,0,sizeof(train));
	pMysql data;
	data=(pMysql)calloc(1,sizeof(Mysql));
	int ret;
	int dataLen;
	char name[100]={0};
	char token[100]={0};
	char pass[100]={0};
A:	memset(name,0,sizeof(name));
	memset(data,0,sizeof(Mysql));
	memset(token,0,sizeof(token));
	//接受用户名,ok!
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,name,dataLen);
	data->name=name;
	//判断数据库是否有该用户
	ret=Sql_Query(data,1);
	ERROR_CHECK(-1,ret,"Sql_Query");
	if(1==ret){//数据库有用户,把salt发给客户,并接受密文
		//告诉客户端名字匹配成功
		train.dataLen=2;
		strcpy(train.buf,"Ok");
		send(newFd,&train,4+train.dataLen,0);
		//发送salt
		train.dataLen=strlen(data->salt);
		strcpy(train.buf,data->salt);
		ret=send(newFd,&train,4+train.dataLen,0);
		ERROR_CHECK(ret,-1,"send");
		memset(pass,0,sizeof(pass));
		//接受客户端密文
		recvCycle(newFd,&dataLen,4);
		recvCycle(newFd,pass,dataLen);
		data->password=pass;
		//接收到密文之后，进数据库比对密文
		ret=Sql_Query(data,3);
		if(1==ret){
			train.dataLen=13;
			memset(train.buf,0,sizeof(train.buf));
			strcpy(train.buf,"Logon Success");
			send(newFd,&train,4+train.dataLen,0);
			recvCycle(newFd,&dataLen,4);
			recvCycle(newFd,token,dataLen);
			token_init(data->name,token);//把token插入Logon表中

			command(newFd,data->name);//用来和客户端做交互
			return 1;//1代表登录成功
		}else{
			train.dataLen=2;
			strcpy(train.buf,"No");
			send(newFd,&train,4+train.dataLen,0);
			goto A;
			return 0;//返回0代表匹配失败
		}
	}	
	else{
		train.dataLen=2;
		strcpy(train.buf,"No");
		send(newFd,&train,4+train.dataLen,0);
		goto A;
	}
	return 0;
}
