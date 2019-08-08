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
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,funcName) {if(ret==retval) \
        {perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName) {if(ret!=0) \
        {printf("%s:%s\n",funcName,strerror(ret));return -1;}}
#define NET_PORTS(ip,port) ({int socketFD;socketFD=socket(AF_INET,SOCK_STREAM,0);\
        ERROR_CHECK(socketFD,-1,"socket");int rett;int reuse=1;\
        rett=setsockopt(socketFD,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));\
        ERROR_CHECK(rett,-1,"setsockopt");\
        struct sockaddr_in ser;bzero(&ser,sizeof(ser));\
        ser.sin_family=AF_INET;ser.sin_addr.s_addr=inet_addr(ip);ser.sin_port=htons(atoi(port));\
        rett=bind(socketFD,(struct sockaddr*)&ser,sizeof(ser));ERROR_CHECK(rett,-1,"bind");\
        listen(socketFD,10);\
        socketFD;})
//管理每一个子进程的数据结构
typedef struct{
	pid_t pid;//子进程的pid
	int fd;//子进程的管道对端
	short busy;//子进程是否忙碌，0代表非忙碌，1代表忙碌
}Process_Data;
typedef struct{
	int dataLen;
	char buf[1000];
}Train_t;
#define FILENAME "file"
int makeChild(Process_Data*,int);
int childHandle(int);
int sendFd(int,int,int);
int recvFd(int,int*,int*);
int tranFile(int);
