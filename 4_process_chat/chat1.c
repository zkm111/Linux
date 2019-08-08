#include <func.h>
typedef struct mgbuf{
	long mytype;
	char mtext[64];
}MSG;
int main(int argc,char* argv[])
{
	args_check(argc,3);
	int meg_id;
	meg_id=msgget(1000,0666|IPC_CREAT);
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	msg.mytype=1;
	int fdr,fdw;
	fdr=open(argv[1],O_RDONLY);
	fdw=open(argv[2],O_WRONLY);
	printf("chat1 fdr=%d,fdw=%d\n",fdr,fdw);
	int semArrId;
	semArrId=semget(2000,1,IPC_CREAT|0600);
	semctl(semArrId,0,SETVAL,1);	
	struct sembuf sopp,sopv;	
	sopp.sem_num=0;
	sopp.sem_op=-1;
	sopp.sem_flg=SEM_UNDO; 
	sopv.sem_num=0;
	sopv.sem_op=1;
	sopv.sem_flg=SEM_UNDO;
	char buf[128]={0};
	while(1)
	{
		memset(buf,0,sizeof(buf));
		read(fdr,buf,sizeof(buf));

		semop(semArrId,&sopp,1);
		memset(msg.mtext,0,64);
		strcpy(msg.mtext,buf);
		msgsnd(meg_id,&msg,sizeof(msg.mtext),0);
		if(!strcmp(buf,"quit")){
			exit(0);
		}
		semop(semArrId,&sopv,1);

		memset(buf,0,sizeof(buf));
		read(0,buf,sizeof(buf));
		write(fdw,buf,strlen(buf)-1);
		if(!strcmp(buf,"quit\n")){
			semop(semArrId,&sopp,1);
			memset(msg.mtext,0,64);
			strcpy(msg.mtext,buf);
			msgsnd(meg_id,&msg,sizeof(msg.mtext),0);
			semop(semArrId,&sopv,1);
			exit(0);
		}
	}
	return 0;
}

