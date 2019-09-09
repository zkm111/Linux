#include "mysql.h"
int Sql_Query(pMysql data,int flag)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";
	char select1[300]="select * from Logon where Username='";
	char select3[300]="select * from Logon where Password='";
	if(1==flag){
		sprintf(select1,"%s%s%s",select1,data->name,"'");
	}
	if(3==flag){
		sprintf(select3,"%s%s%s",select3,data->password,"'");
	}
	int t1,t3;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected to MySql\n");
	}
	if(1==flag){//flag为1是查询名字，返回salt
		t1=mysql_query(conn,select1);
		if(t1){
			printf("Error making query:%s\n",mysql_error(conn));
			return -1;
		}else{
			res=mysql_use_result(conn);
			if(res){//只要查询语句合法，就能进这个if，它对应的else是语句不合法的时候进入，只要语句合法，就算查询结果为空，也能进这个if
				while((row=mysql_fetch_row(res))!=NULL){//当查询到内容才能进while
					printf("query success!\n");
					char temp[20]={0};
					strcpy(temp,row[2]);
					data->salt=temp;//为了让调用该函数的地方通过查找用户名拿到盐值
					return 1;
				}
			}else{
				printf("Query statement incorrent\n");
				return 0;//查询语句出错返回0
			}
			printf("query error\n");
			mysql_free_result(res);
			return 0;//查询失败返回0
		}
	}
	if(3==flag){//仅仅比对客户端传过来的密文是否存在
		t3=mysql_query(conn,select3);
		if(t3){
			printf("Error making query:%s\n",mysql_error(conn));
			return -1;
		}else{
			res=mysql_use_result(conn);
			if(res){
				while((row=mysql_fetch_row(res))!=NULL){
					printf("query success!\n");
					return 1;//比对成功，不需要传出参数，只返回1代表成功。
				}
			}else{
				printf("Query statement incorrent\n");
				return 0;
			}
			printf("query error\n");
			mysql_free_result(res);
			return 0;//查询失败返回0
		}
	}
	mysql_close(conn);
	return 0;
}

