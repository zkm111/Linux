#include "factory.h"
int vd_pwd(int code,char** buf)
{
        char buf1[1000]={0};
        char buf2[1000]={0};
        char buf3[1000]={0};
        char head[10]="/";
        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW row;
        const char* server="127.0.0.1";
        const char* user="zkm";
        const char* password="196045";
        const char* database="NetDisk";//要访问的数据库名称
        char select_dir[300]="select * from vd where code=";
        sprintf(select_dir,"%s%d",select_dir,code);
        puts(select_dir);
        int t;
        conn=mysql_init(NULL);
        if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
        {
                printf("Error connecting to database:%s\n",mysql_error(conn));
                return -1;
        }else{
                printf("Connected...\n");
        }
A:      printf("\nfor pwd\n");
	   	memset(select_dir,0,sizeof(select_dir));
        strcpy(select_dir,"select * from vd where code=");
        sprintf(select_dir,"%s%d",select_dir,code);
        puts(select_dir);
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
                                memset(buf3,0,sizeof(buf3));
                                code=atoi(row[0]);
                                if(code!=0){
                                        strcpy(buf3,row[2]);
                                        sprintf(buf3,"%s%s",buf3,"/");
                                        sprintf(buf1,"%s%s",buf3,buf2);
                                        strcpy(buf2,buf1);
                                        mysql_free_result(res);
                                        goto A;
                                }
                                else{
                                        strcpy(buf3,head);
                                        sprintf(buf1,"%s%s",buf3,buf2);
                                        mysql_free_result(res);
                                }
                        }
                }else{
                        printf("Don't find data\n");
                }
        }
        *buf=buf1;
        mysql_close(conn);
        return 0;
}

