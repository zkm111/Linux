#include "factory.h"
int vd_cd(char* name,char* belong,int* code)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="127.0.0.1";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";//要访问的数据库名称
	printf("\nfor cd\n");
	char select_dir[300]="select * from vd where belong='";
	sprintf(select_dir,"%s%s%s%d%s%s%s",select_dir,belong,"' and procode=",*code," and type='d' and name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,select_dir);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)//这个while解决匹配数据有多行的问题
			{
				*code=atoi(row[1]);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;


}	
