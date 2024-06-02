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

FILE *fileAddress;
void *start_routine(void *arg);
pthread_t tids[5];
struct sockaddr_in servAddr, clientAddr;
int connfd;

int main(int argc, char *argv[]){
    //Get from the command line, server IP, src and dst files.
   if (argc != 3){
    printf ("Usage: %s <port #> <dst file>\n",argv[0]);
    	exit(0);
} 
    //Declare socket file descriptor and buffer
    	int sockfd, connfd, rb, sin_size;
        char rbuf[1024], sbuf[1024];
    

	//Declare server address to accept                           
    
	


	
	//Declare host
                              
    	//get hostname
       
	//Open a socket, if successful, returns                                                     
	if((sockfd = socket(AF_INET, SOCK_STREAM,0))<0){

		perror("Faulure to setup an endpoint socket");
		exit(1);
	}


	//Set the server address to send using socket addressing structures     
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[1]));
	servAddr.sin_addr.s_addr = INADDR_ANY;                    


	if((bind(sockfd, (struct sockaddr * )&servAddr, sizeof(struct sockaddr)))<0)
	{
		perror("Failure to bind server address to the endpoint socket");
		exit(1);
	}
	
	printf("Server waiting for client at port %d\n", atoi(argv[1]));                               
    	//Connect to the server
    	

	if (listen(sockfd,5)== -1)
	{
		perror("listen");
		exit(1);

	}
	sin_size = sizeof(struct sockaddr_in);


	//server accepting to establisj a connection
         
     int i=0;
    for(i = 0; i <= 5 ; i++){
        connfd= accept(sockfd, (struct sockaddr *)&clientAddr, (socklen_t *)&sin_size);
        pthread_create(&tids[i], NULL, start_routine, (void *) &connfd);
    }
    fclose(fileAddress);
	/*Connection established, server begin 
		printf("Connection established with ");
		while((rb = read(connfd, rbuf, sizeof(rbuf)"out.txt"
			printf("Client sent: %s\n", rbuf);

			write(connfd, "Acknowledge",20);
			
		}
 		close(connfd);CONN = malloc(sizeof(int));
        *CONN = accept(sockfd, (struct sockaddr *)&clientAddr, (socklen_t *)&sin_size);
        pthread_create(&tids[i], NULL, 
	}
	*/

	close(sockfd);

	return 0;
}                                                          
     //Client sends the name of the file to retrieve from the server
                                                                               
    //Client begins to write and read from the server
                                                                                      
    //Close socket descriptor                                                                                          
    
	void *start_routine(void *arg){
    char recv_data[1024];
    int bytes_received;
    int connfd = *(int*)arg;
    printf("%d\n", connfd);

	 fileAddress = fopen("out.txt", "w");
	 int bytes ;
	 int sin_size = sizeof(struct sockaddr_in);

        if (fileAddress != NULL){
			bytes = recvfrom(connfd, recv_data, 1024, 0, (struct sockaddr *)&clientAddr, (socklen_t *)&sin_size);
            fwrite(recv_data,1, bytes, fileAddress);
        }
        else {
            printf("\n Unable to Create or Open the sample1.txt File");
        }
        fflush(fileAddress);  //changed to fflush
    printf("Received data: %s", recv_data);
}                                                                              
