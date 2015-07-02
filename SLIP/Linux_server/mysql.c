#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mysql.h"

int MYSQL_initilize()
{
	con = mysql_init(NULL);

	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		return FALSE;
	}
	
	if (mysql_real_connect(con, "localhost", "root", "user1234", 
		"InternetOfThings", 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return FALSE;
	}
	
	return TRUE;
}

int MYSQL_insert(char* str)
{
	char query[128], buff[16];
	time_t current_time;
	
	current_time = time(NULL);
	strftime(buff, 16, "%Y-%m-%d", localtime(&current_time));
	sprintf( query, "%s,'%s');", str, buff);
	printf("%s\n",query);
	
	if (mysql_query(con, query)) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return FALSE;
	}
	
	return TRUE;
}

void MYSQL_close()
{
	mysql_close(con);
}
