#ifndef __HEAD_H__
#define __HEAD_H__ 
#include <arpa/inet.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/uio.h>
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
#endif
