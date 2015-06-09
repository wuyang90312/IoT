#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "mysql.h"
 
#define PORTNUM 2300
 
int main(int argc, char *argv[])
{
	//char* msg = "Hello World !\n";
	int bufsize=1024;        
	char *buffer=malloc(bufsize);
	char destination[1024];
	int bytesRead = 1;

	struct sockaddr_in dest; /* socket info about the machine connecting to us */
	struct sockaddr_in serv; /* socket info about our server */
	int mysocket;            /* socket used to listen for incoming connections */
	socklen_t socksize = sizeof(struct sockaddr_in);

	memset(&serv, 0, sizeof(serv));           /* zero the struct before filling the fields */
	serv.sin_family = AF_INET;                /* set the type of connection to TCP/IP */
	serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
	serv.sin_port = htons(PORTNUM);           /* set the server port number */    

	mysocket = socket(AF_INET, SOCK_STREAM, 0);

	/* bind serv information to mysocket */
	bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr));

	/* start listening, allowing a queue of up to 1 pending connection */
	listen(mysocket, 1);
	int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	printf("Into listening ... \n");
	
	MYSQL_initilize();		/* Initialize the configurations of mysql */
	while(consocket)
	{
		//printf("Incoming connection from %s - sending welcome\n", inet_ntoa(dest.sin_addr));
		//send(consocket, msg, strlen(msg), 0);
		bytesRead = read(consocket, buffer, bufsize);		/* Wait for the incoming msg from ESP89266 */
		
		memcpy(destination, buffer, strlen(buffer)-1);		/* Take all characters except for the last carriage return */
		destination[strlen(buffer)-1] = NULL;				/* End the array with NULL */
		
		printf("CLIENT RECV: [%s]\n", destination);
		//MYSQL_insert(destination); /* Insert the string into local database */
		close(consocket);
		consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	}
	printf("Out of listening ... \n");
	
	//free(destination);
	free(buffer);
	MYSQL_close();		/* Close the mysql constructor */
	close(mysocket);
	return EXIT_SUCCESS;
}
