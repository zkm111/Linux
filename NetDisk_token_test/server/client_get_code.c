#include "factory.h"
int client_get_code(char* username,char* token,int* code)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="localhost";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";
	char select[300]="select * from Logon where Username='";
	sprintf(select,"%s%s%s%s%s",select,username,"' and Token='",token,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected to MySql\n");
	}
	t=mysql_query(conn,select);
	if(t){
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)//这个while解决匹配数据有多行的问题
			{
				*code=atoi(row[5]);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}
