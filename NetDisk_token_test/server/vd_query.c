#include "factory.h"
int vd_query(int code,char *name,char **data)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char* server="127.0.0.1";
	const char* user="zkm";
	const char* password="196045";
	const char* database="NetDisk";//要访问的数据库名称
	char dir[300]={0};
	char file[300]={0};
	char select_dir[300]="select * from vd where belong='";
	char select_file[300]="select * from vd where belong='";
	sprintf(select_dir,"%s%s%s%d%s",select_dir,name,"' and procode=",code," and type='d'");
	sprintf(select_file,"%s%s%s%d%s",select_file,name,"' and procode=",code," and type='f'");
	puts(select_dir);
	puts(select_file);
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
				sprintf(dir,"%s%s%s",dir,row[2],"/  ");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	t=mysql_query(conn,select_file);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)//这个while解决匹配数据有多行的问题
			{
				sprintf(file,"%s%s%s",file,row[2],"  ");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	sprintf(dir,"%s%s",dir,file);
	*data=dir;
	return 0;
}
