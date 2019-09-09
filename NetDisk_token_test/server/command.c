#include "factory.h"
int command(int newFd,char *name) 
{
	int code=0;//记录当前客户端在哪一层目录
	vd_root(name,&code);//找到当前用户根目录的code值
	logon_code_init(code,name);//把客户端所在目录的code存到表logon中
	printf("\nafter logon ,code=%d\n",code);
	char buf[100]={0};
	int dataLen=0;
	Train_t train;
	while(1){
		//接受客户端的命令
		memset(buf,0,sizeof(buf));
		recvCycle(newFd,&dataLen,4);
		recvCycle(newFd,buf,dataLen);

		if(!strncmp(buf,"ls",2)){//把当前目录下所有内容传给客户端
			char *ls=NULL;
			vd_query(code,name,&ls);//data中就是当前目录下的所有内容
			train.dataLen=strlen(ls);
			strcpy(train.buf,ls);
			send(newFd,&train,4+train.dataLen,0);
		}
		if(!strncmp(buf,"cd",2)){//cd 没有给客户端发字符串！
			char *p=buf;
			char *phead=NULL;
			char *ptrail=NULL;
			p=p+3;
			if(p[0]=='/'){//如果是绝对路径，就把当前code改为根目录的code
				p=p+1;
				vd_root(name,&code);//从根目录开始找
				goto A;
			}
			if(p[0]=='.'){//如果回上级目录
				vd_back(&code);
			}
A:			phead=p;//不走if就是处理相对路径
			ptrail=p;
			while(*ptrail!=0){
				if(*ptrail=='/'){
					*ptrail='\0';
					vd_cd(phead,name,&code);//可以改变code的值，从而知道客户端所在哪个目录下面
					phead=ptrail+1;
				}
				ptrail++;
			}
			printf("\nnow,code=%d\n",code);
			logon_code_init(code,name);
		}
		if(!strncmp(buf,"pwd",3)){
			char *pwd=NULL;
			vd_pwd(code,&pwd);
			train.dataLen=strlen(pwd);
			strcpy(train.buf,pwd);
			send(newFd,&train,4+train.dataLen,0);
		}
		if(!strncmp(buf,"mkdir",5)){
			char* mkdir=buf;
			mkdir=mkdir+6;
			vd_mkdir(code,mkdir,name);
		}
	}
		return 0;
}
