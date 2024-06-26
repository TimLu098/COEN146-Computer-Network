#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Define the number of clients/threads that can be served

#define N 100
#define BUF_SIZE 2048
int threadCount = 0;
pthread_t clients[N]; //declaring N threads

 //Declare socket and connection file descriptors.
 

int sockfd, connfd, rb, sin_size;
 //Declare receiving and sending buffers of size 10 bytes
 

char sbuf[10], rbuf[10];
 //Declare server address to which to bind for receiving messages and client address to fill in sending address


struct sockaddr_in servAdder, clienAddr;
//Connection handler (thread function) for servicing client requests for file transfer

void* connectionHandler(void* sock){
   //declare buffer holding the name of the file from client
   char filename[1024];

   int sockfd, rb, sin_size;
   char sbuf[10], rbuf[20];
   //int cd = *((int *)sock);

   //Connection established, server begins to read and write to the connecting client
   printf("Connection Established with client IP: %s and Port: %d\n", inet_ntoa(clienAddr.sin_addr), ntohs(clienAddr.sin_port));
   
        //receive name of the file from the client
   read(connfd, rbuf, sizeof(rbuf));
   printf("File transfer complete: %s\n", rbuf);



   //open file and send to client
   FILE *fp = fopen(rbuf, "r");


   //read file and send to connection descriptor
   char *buf = malloc((size_t)BUF_SIZE);
   int m; 
   while ((m = fread(buf , 1, BUF_SIZE, fp)) >0 )
   {
      printf("%d\n", m);
      write(connfd, buf, m);
   }
   printf("File transfer complete\n");
   
   //close file
   fclose(fp);
   //Close connection descriptor
   close(connfd);
   pthread_exit(0);
}


int main(int argc, char *argv[]){
 //Get from the command line, server IP, src and dst files.
 if (argc != 2){
        printf ("Usage: %s <port #> \n",argv[0]);
        exit(0);
 } 
 //Open a TCP socket, if successful, returns a descriptor
 if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) < 0)
 {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

 //Setup the server address to bind using socket addressing structure
 servAdder.sin_family = AF_INET;
 servAdder.sin_port = htons(atoi(argv[1]));
 servAdder.sin_addr.s_addr = INADDR_ANY;

 //bind IP address and port for server endpoint socket
 
if((bind(sockfd, (struct sockaddr *)&servAdder, sizeof(struct sockaddr))) < 0)
 {
    perror("Failure to bind server address to th endpoint socket");
    exit(1);
 }

 // Server listening to the socket endpoint, and can queue 5 client requests
 printf("Server listening/waiting for client at port 5000\n");
 listen(sockfd, 5);
 sin_size = sizeof(struct sockaddr_in);

 while (1){
   //Server accepts the connection and call the connection handler
   if((connfd = accept(sockfd, (struct sockaddr*)&clienAddr, (socklen_t*)&sin_size)) < 0)
   {
      perror("Failure to accept connection to the client");
      exit(1);
   }

   if(pthread_create(&clients[threadCount], NULL, connectionHandler, (void*) &connfd) < 0){
      perror("Unable to create a thread");
      exit(0);
   }
   else 
      printf("Thread %d has been created to service client request\n",++threadCount);
 }
 int i;
 for( i = 0; i < threadCount; i++){
      pthread_join(clients[i], NULL);
 }
 return 0;
}


// write in to the file 