#include "factory.h" 
int reg(int newFd)
{
	int dataLen;
	char name[50]={0};
	char str[11]={0};//保存salt的字符串
	char pass[50]={0};//接受客户端传来的密码
	pMysql data;
	data=(pMysql)calloc(1,sizeof(Mysql));
	//接收用户名
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,name,dataLen);
	data->name=name;
	//接收密码,同时用随机产生salt，并与密码产生密文一并存入数据库
	recvCycle(newFd,&dataLen,4);
	recvCycle(newFd,pass,dataLen);
	ran(str);
	data->salt=str;
	data->password=crypt(pass,str);
	Sql_Insert(data);
	vd_init(data->name);//每当有用户注册，就要往虚拟目录内加一行内容，即注册用户自己目录的初始化
	logon(newFd);//走完注册函数，走登录函数
	return 0;
}
