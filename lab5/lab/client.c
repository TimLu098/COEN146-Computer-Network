/* Yen-Jung Lu

   COEN146 LAB5 - client.c

   05/06/2022

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

//Declare a Header structure that holds, sequence/ acknowledgement number, checksum, and length of a packet
typedef struct {
    int seq_ack;
    int len;
    int cksum;
} Header;

//Declare a packet structure that holds data and header
typedef struct {
    Header header;
    char data[10];
} Packet;

//Calculating the Checksum
int getChecksum(Packet packet) {
    packet.header.cksum = 0;
    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;
    while (ptr < end) {
        checksum ^= *ptr++;
    }
    return checksum;
}

//Printing received packet, sequence/acknowledgement number, and checksum
void printPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//client sending packet with checksum and sequence number and waits for acknowledgement
void clientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {	
        //verify rdt2.2 protocol by simulate a packet corruption
        int corrupt;
        
        
        corrupt = rand() % 2;
        printf("corrupt is: %d\n", corrupt);
        //send checksum = 0 (to simulate packet corruption)
        if(corrupt == 1)
        {
            packet.header.cksum = 0;
        }
        //or calculate and send the right checksum of packet
        if(corrupt == 0)
        {
            packet.header.cksum = getChecksum(packet);
        }
        //send the packet
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        //receive an ACK from the server
        Packet recvpacket;
        recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)address, (socklen_t *)&addrlen);
        //print received packet (ACK) and checksum
        printf("Client received ACK %d, checksum %d - \n", recvpacket.header.seq_ack, recvpacket.header.cksum);

        //calculate checksum of received packet (ACK)
        int e_cksum = getChecksum(recvpacket);
        //check the checksum
        if(recvpacket.header.cksum != e_cksum){
        //if bad checksum, resend packet
            printf("Client: Bad checksum, expected checksum was: %d\n", e_cksum);
            
        //check the sequence number
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack) {
        //if incorrect sequence number, resend packet
            printf("Client: Bad seqnum, expected sequence number was: %d\n",packet.header.seq_ack);
            
        //if the ckechsum and sequence numbers are correct, break and return to the main to get the next packet to send
        } else {
            // good ACK, we're done
            printf("Client: Good ACK\n");
            break;
        }
    }
}


int main(int argc, char *argv[]) {
    srand(time(0));
    //Get from the command line, server IP, Port and src file
    if (argc != 4) {
        printf("Usage: %s <ip> <port> <srcfile>\n", argv[0]);
        exit(0);
    }
    
    //Declare socket file descriptor.
    int sockfd; 

    //Open a UDP socket, if successful, returns a descriptor    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failure to setup an endpoint socket");
        exit(1);
    }


    //Declare server address to connect to
    struct sockaddr_in servAddr;
    struct hostent *host;
    host = (struct hostent *) gethostbyname(argv[1]);

    ///Set the server address to send using socket addressing structure
    memset(&servAddr, 0, sizeof(servAddr));
    
    //initialize servAddr structure
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[2]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //open file using argv[3]
    int fp=open(argv[3],O_RDWR);
    if(fp<0){
    	perror("Failed to open file\n");
	    exit(1);
    }

    //send file contents to server packet by packet
    int n, seq = 0;
    socklen_t addr_len = sizeof(servAddr);
    Packet packet;
    printf("prepare to send...");
    while((n = read(fp, packet.data, sizeof(packet.data)))>0){
        //assign seq and checksum to packet and send
    	packet.header.cksum = getChecksum(packet);
        packet.header.seq_ack = seq;
        packet.header.len=n;
        clientSend(sockfd,(struct sockaddr *)&servAddr,addr_len,packet);
    	seq=(seq+1)%2;
    }

    // send zero-length packet to server to end connection
    Packet final;
    final.header.seq_ack=seq;
    final.header.len=0;
    final.header.cksum=getChecksum(final);
    clientSend(sockfd,(struct sockaddr *)&servAddr,addr_len,final);
    
	//close file and socket
    close(fp);
    close(sockfd);
    return 0;
}
