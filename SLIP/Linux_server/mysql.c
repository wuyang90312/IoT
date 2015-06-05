#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mysql.h"
/*
int main(int argc, char **argv)
{  
	MYSQL *con = mysql_init(NULL);
	char query[128], buff[16];
	time_t current_time;

	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "root", "user1234", 
		"InternetOfThings", 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	current_time = time(NULL);
	printf("%lld\n",current_time/3600);
	printf("%s",ctime(&current_time));
	strftime(buff, 16, "%Y-%m-%d", localtime(&current_time));
	sprintf( query, "INSERT INTO acceleration_records (ID,X,Y,Z, DATEs) VALUES ('ESP-001',0.3, 1.2, -0.9,'%s');", buff);
	printf("%s\n",query);

	if (mysql_query(con, query)) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	mysql_close(con);
	exit(0);
}*/

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
