#ifndef __MYSQL_H__
#define __MYSQL_H__
#include "head.h"
typedef struct{
	char* name;//=NULL;
	char* salt;//=NULL;
	char* password;//=NULL;
}Mysql,*pMysql;
int Sql_Query(pMysql,int);
int Sql_Insert(pMysql); 
#endif
