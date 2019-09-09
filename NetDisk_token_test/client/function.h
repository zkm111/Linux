#include <sys/uio.h>
#include <crypt.h>
#include <errno.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/msg.h>
#include <strings.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#define args_check(argc,val) {if(argc!=val) \
	{printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,funcName) {if(ret==retval) \
	{printf("%d:",__LINE__);fflush(stdout);perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) \
	{printf("%s:%s\n",funcName,strerror(ret));return -1;}}
typedef struct{
	int dataLen;
	char buf[1000];
}Train_t;
typedef struct{
	char *ip;
	char *port;
	char filename[50];
	char* token;
	char* username;
}Client_th,*pClient_th;
int recvCycle(int,void*,int);
int logon(int,char*,char*);//登录函数
int reg(int,char*,char*);//注册函数
int command(int,char*,char*,char*,char*);//和服务器交互，客户端发送命令
int recv_file(int);//负责接收文件
