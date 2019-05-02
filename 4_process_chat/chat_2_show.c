#include<func.h>
int main()
{
	int sh;
	sh=shmget(1000,4096,IPC_CREAT|0600);
	char *pmap=(char*)shmat(sh,NULL,0);
	memset(pmap,0,4096);
	int semArrId;
	semArrId=semget(1000,1,IPC_CREAT|0600);
	semctl(semArrId,0,SETVAL,1);
	struct sembuf sopp,sopv;
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO;
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	while(1){
		semop(semArrId,&sopp,1);
		if(strlen(pmap)>0){	
			if(!strcmp(pmap,"quit")||!strcmp(pmap,"quit\n"))
				exit(0);
			else{
				printf("Gets from chat1:%s\n",pmap);
				memset(pmap,0,4096);
			}
		}
		semop(semArrId,&sopv,1);
	}

}
