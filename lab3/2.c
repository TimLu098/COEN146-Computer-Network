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
#include<sys/stat.h>
#include <fcntl.h> 

int main(int argc, char *argv[]){
    //Get from the command line, server IP, port, src and dst files.
    if (argc != 5){
		printf ("Usage: %s <ip of server> <port #> <src_file> <dest_file>\n",argv[0]);
		exit(0);
    } 
    //Declare socket file descriptor and buffer
    int sockfd, recvFP, wFP, read_bytes, write_bytes;
    char rbuf[1024];
    

        //Declare server address to accept
        struct sockaddr_in servAddr;

    //Declare host
    struct hostent *host;

    //get hostname
    host = (struct hostent *)gethostbyname(argv[1]);

    //Open a socket, if successful, returns
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("Failure to setup an endpoint socket");
      exit(1);
    }

    //Set the server address to send using socket addressing structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));
    //servAddr.sin_addr = *((struct in_addr *)host->h_addr);
    servAddr.sin_addr = *((struct in_addr*)host->h_addr);

    //Connect to the server
    if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr)))
    {
      perror("Failure to connect to the server");
      exit(1);
    }
    printf("[+]Connected to Server.\n");
    //Client sends the name of the file to retrieve from the server
    //sbuf = argv[3]
    write(sockfd, argv[3], strlen(argv[3])+1);
    
    printf("Server: sent message: %s\n", rbuf);
    //Client begins to write and read from the server
    wFP = open(argv[4],O_WRONLY | O_CREAT | O_TRUNC,0644);
    while((read_bytes = read(sockfd, rbuf, sizeof(rbuf))) > 0)
    {
      write_bytes = write(wFP, rbuf, read_bytes);
      if(write_bytes != read_bytes)
      {
         printf("write bytes(%d) different from read bytes(%d): ",write_bytes, read_bytes);
         exit(1);
      }
    }
    
    
    //Close socket descriptor
    close(wFP);
    close(sockfd);
    return 0;
}
