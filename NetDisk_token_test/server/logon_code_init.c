#include "factory.h"
int logon_code_init(int code,char* name)
{
	MYSQL *conn;
	const char* server="localhost";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";
	char insert[200]="update Logon set Code=";
	int t;
	sprintf(insert,"%s%d%s%s%s",insert,code," where Username='",name,"'");
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected to Mysql\n");
	}
	t=mysql_query(conn,insert);
	if(t){
		printf("Error making insert:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;


}
