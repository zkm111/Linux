#include "fun.h"
int exitFds[2];
void sigFunc(int signum)
{
	write(exitFds[1],&signum,1);
}
int main(int argc,char* argv[])
{
	while(fork()){
		int status;
		wait(&status);
		if(WIFEXITED(status)){
			printf("child exit normal\n");
			exit(0);
		}
	}
	args_check(argc,4);
	pipe(exitFds);
	signal(SIGUSR1,sigFunc);
	int childNum=atoi(argv[3]);
	Process_Data *pchild=(Process_Data*)calloc(childNum,sizeof(Process_Data));
	makeChild(pchild,childNum);
	int socketfd;
	socketfd=NET_PORTS(argv[1],argv[2]);
	int new_fd;
	int epfd;
	epfd=epoll_create(1);
	struct epoll_event event,*evs;
	evs=(struct epoll_event*)calloc(childNum+1,sizeof(struct epoll_event));
	event.events=EPOLLIN;
	event.data.fd=socketfd;
	int ret;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
	event.data.fd=exitFds[0];
	epoll_ctl(epfd,EPOLL_CTL_ADD,exitFds[0],&event);
	ERROR_CHECK(ret,-1,"epoll_ctl");
	for(int i=0;i<childNum;i++){
		event.data.fd=pchild[i].fd;
		ret=epoll_ctl(epfd,EPOLL_CTL_ADD,pchild[i].fd,&event);
		ERROR_CHECK(ret,-1,"epoll_ctl");
	}
	int readyFdNum;
	while(1){
		readyFdNum=epoll_wait(epfd,evs,childNum+2,-1);
		for(int i=0;i<readyFdNum;i++){
			if(evs[i].events==EPOLLIN&&evs[i].data.fd==socketfd){
				new_fd=accept(socketfd,NULL,NULL);
				for(int j=0;j<childNum;j++){
					if(!pchild[j].busy){
						sendFd(pchild[j].fd,new_fd,1);
						pchild[j].busy=1;
						printf("%d child is busy\n",pchild[j].pid);
						break;
					}
				}
				close(new_fd);
			}
			if(evs[i].data.fd==exitFds[0]){
				printf("start exit\n");
				close(socketfd);
				pid_t pid;
				for(int j=0;j<childNum;j++){
					sendFd(pchild[j].fd,0,0);
				}
				for(int j=0;j<childNum;j++){
					pid=wait(NULL);
					printf("%d child successful exit\n",pid);
				}
				return 0;
			}
			for(int j=0;j<childNum;j++){
				if(evs[i].events==EPOLLIN&&evs[i].data.fd==pchild[j].fd){
					read(pchild[j].fd,&ret,1);
					pchild[j].busy=0;
					printf("%d child is not busy\n",pchild[j].pid);
				}
			}

		}
	}
	return 0;
}
