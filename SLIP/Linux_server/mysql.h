#ifndef __MYSQL_H__
#define __MYSQL_H__
#include <mysql.h>

#define		TRUE		1
#define		FALSE		0

static MYSQL *con;

int MYSQL_initilize();
int MYSQL_insert(char* str);
void MYSQL_close();

#endif
