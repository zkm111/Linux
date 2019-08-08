#include "factory.h"
int exitfds[2];
void cleanup(void *p)
{
	pthread_mutex_unlock(&((pQue_t)p)->queMutex);

}
void* threadfunc(void* p)
{
	pFactory pf=(pFactory)p;
	pQue_t pq=&pf->que;
	pthread_cleanup_push(cleanup,pq);
	pNode_t pcur;
	int ret;
	while(1){
		pthread_mutex_lock(&pq->queMutex);
		if(!pq->queSize){//如果队列为空，子线程睡觉
			pthread_cond_wait(&pf->cond,&pq->queMutex);
		}
		ret=queGet(pq,&pcur);
		pthread_mutex_unlock(&pq->queMutex);
		if(0==ret){
			tranFile(pcur->nd_sockfd);
			free(pcur);
		}
		pcur=NULL;
	}
	pthread_cleanup_pop(1);
}
void sigExitFunc(int signum)
{
	write(exitfds[1],&signum,1);
}
int main(int argc,char **argv)
{
	args_check(argc,5);
	pipe(exitfds);
	if(fork()){
		close(exitfds[0]);
		signal(SIGUSR1,sigExitFunc);
		wait(NULL);
		exit(0);
	}
	close(exitfds[1]);
	Factory f;
	int threadNum=atoi(argv[3]);
	int factoryCapacity=atoi(argv[4]);
	factoryInit(&f,threadNum,factoryCapacity,threadfunc);
	factoryStart(&f);
	int socketfd;
	socketfd=NET_PORTS(argv[1],argv[2]);
	int new_fd;
	pQue_t pq=&f.que;
	pNode_t pnew;
	int epfd=epoll_create(1);
	struct epoll_event event,evs[2];
	event.events=EPOLLIN;
	event.data.fd=exitfds[0];
	epoll_ctl(epfd,EPOLL_CTL_ADD,exitfds[0],&event);
	event.data.fd=socketfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,socketfd,&event);
	int fdReadyNum;
	while(1){
		fdReadyNum=epoll_wait(epfd,evs,2,-1);
		for(int i=0;i<fdReadyNum;i++){
			if(socketfd==evs[i].data.fd){
				new_fd=accept(socketfd,NULL,NULL);
				pnew=(pNode_t)calloc(1,sizeof(Node_t));
				pnew->nd_sockfd=new_fd;
				pthread_mutex_lock(&pq->queMutex);
				queInsert(pq,pnew);
				pthread_mutex_unlock(&pq->queMutex);
				pthread_cond_signal(&f.cond);
			}
			if(exitfds[0]==evs[i].data.fd){
				close(socketfd);
				for(int i=0;i<threadNum;i++){
					pthread_cancel(f.pthid[i]);
				}
				for(int i=0;i<threadNum;i++){
					pthread_join(f.pthid[i],NULL);
				}
				printf("thread_pool exit success\n");
				exit(0);
			}
		}
	}
	return 0;
}
