/* Yen-Jung Lu
 *
 * COEN146 lab3 client
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    //Get from the command line, server IP, src and dst files.
    	if (argc != 5){
		
		printf ("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
		exit(0);
    	}
           

	//Declare socket file descriptor
	int sockfd, nr;
	char rbuf[1024], sbuf[1024];

	//Declare server address to connect to 
	struct sockaddr_in servAddr;
	struct hostent *host;


	//get hostname
	host = (struct hostent *)gethostbyname("localhost");
	
	

	//Open socket, if successful, returns
	 if((sockfd = socket(AF_INET, SOCK_STREAM,0))<0){

                perror("Faulure to setup an endpoint socket");
                exit(1);
        }


	//Set the server address to send using socket addressing structures
		servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(atoi(argv[2]));
        servAddr.sin_addr= *((struct in_addr *)host->h_addr);

	
	if(connect(sockfd, (struct sockaddr * )&servAddr, sizeof(struct sockaddr))<0){
		perror("Failure to connect to the server");
		exit(1);

	}
	 FILE *fptr1;

	 fptr1 = fopen(argv[3], "r");

	 if(fptr1 == NULL){
		 printf("Cannot open file\n");
		 exit(0);

	 }
	 
	write(sockfd, argv[3], strlen(argv[3]));

     /*
	// client begins to write and read 
	while(1){

		printf("CLient: Type a message to send to server\n");

		scanf("%s", sbuf);
		write(sockfd, sbuf, sizeof(sbuf));

		read(sockfd, rbuf, sizeof(rbuf));
		printf("Server: sent message: %s\n", rbuf);

	} */



	close(sockfd);

	return 0;


}
