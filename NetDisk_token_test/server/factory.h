#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "tranfile.h"
#include "mysql.h"
typedef void* (*threadfunc_t)(void*);
typedef struct{
	pthread_t *pthid;
	int threadNum;
	Que_t que;
	pthread_cond_t cond;
	threadfunc_t downFileFunc;
	short statFlag;
}Factory,*pFactory;
void factoryInit(pFactory pf,int threadNum,int capacity,threadfunc_t threadfunc);
void factoryStart(pFactory);
int recvCycle(int,void*,int);
int logon(int);//负责登录的函数
int reg(int);//负责注册的函数 
void ran(char*);//注册时产生salt的函数
int vd_init(char*);//注册时对相应用户虚拟目录初始化 
int command(int,char*);//处理客户端的各种命令
int vd_root(char*,int*);//找到用户根目录
int vd_query(int,char*,char**);//找到用户当前目录下所有内容
int vd_cd(char*,char*,int*);//处理客户端cd命令，关键能改变服务器记录的客户端的当前的目录层级别（code值）
int vd_pwd(int,char**);//处理客户端pwd命令
int vd_mkdir(int,char*,char*);//处理客户端mkdir命令
int vd_back(int*);//处理客户端的cd..
int token_init(char*,char*);
int logon_code_init(int,char*);
int client_gets(int);
int client_get_code(char*,char*,int*);//获取该用户当前路径
#define FILENAME "file"
#endif
