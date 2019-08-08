#include <func.h>

int main(int argc,char* argv[])
{
	args_check(argc,3);
	int fdr,fdw;
	fdw=open(argv[1],O_WRONLY);
	fdr=open(argv[2],O_RDONLY);
	printf("chat2 fdr=%d,fdw=%d\n",fdr,fdw);
	char buf[128]={0};
	int sh;
	sh=shmget(1000,4096,IPC_CREAT|0600);
	char *pmap=(char*)shmat(sh,NULL,0);
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
	while(1)
	{
		memset(buf,0,sizeof(buf));
		read(0,buf,sizeof(buf));
		write(fdw,buf,strlen(buf)-1);
		if(!strcmp(buf,"quit\n")){
			semop(semArrId,&sopp,1);
			memset(pmap,0,4096);
			strcpy(pmap,buf);
			semop(semArrId,&sopv,1); 
			exit(0);
		}
		memset(buf,0,sizeof(buf));
		read(fdr,buf,sizeof(buf));

		semop(semArrId,&sopp,1);
		memset(pmap,0,4096);
		strcpy(pmap,buf);
		if(!strcmp(buf,"quit"))
			exit(0);
		semop(semArrId,&sopv,1);
	}
	return 0;
}

