#include<func.h>
typedef struct mgbuf{
	long mytype;
	char mtext[64];
}MSG;
int main()
{
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
	int meg_id;
	meg_id=msgget(1000,0666|IPC_CREAT);
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	while(1){
		msgrcv(meg_id,&msg,sizeof(msg.mtext),1,0);
		semop(semArrId,&sopp,1);
		if(strlen(msg.mtext)>0){
			if(!strcmp(msg.mtext,"quit\n")||!strcmp(msg.mtext,"quit")){
				exit(0);
			}
			else{
				printf("Gets from chat2:%s\n",msg.mtext);
				memset(&msg.mtext,0,64);
			}
		}
		semop(semArrId,&sopv,1);
	}
}
