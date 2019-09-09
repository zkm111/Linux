#include "factory.h"
int vd_init(char* name)
{
	MYSQL *conn;
	const char* server="localhost";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";
	char insert[200]="insert into vd(procode,name,type,belong) values(0,'";
	sprintf(insert,"%s%s%s%s%s",insert,name,"','d','",name,"')");
	puts(insert);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,insert);
	if(t)
	{
		printf("Error making insert:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;


}
