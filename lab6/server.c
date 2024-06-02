/*  Yen-Jung Lu

    05/13/2022
    
    COEN146 LAB6- server.c

*/
#include <arpa/inet.h> // necessary for sockaddr_in
#include <stdio.h> // necessary for perror
#include <stdlib.h> // necessary for EXIT_FAILURE
#include <string.h> // necessary for strlen
#include <sys/socket.h> // necessary for sockaddr and socklen_t
#include <time.h> // necessary for rand
#include <unistd.h> // necessary for write and close
#include <fcntl.h> // necessary for O_RDWR



typedef struct{
	int seq_ack;
	int len;
	int cksum;
}Header;

typedef struct{
	Header header;
	char data[10];
}Packet;


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

void printPacket(Packet packet) {
    printf("Packet { header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
           packet.header.seq_ack,
           packet.header.len,
           packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

void serverSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    // Simulating a chance that ACK gets lost
    if (rand() % 5 == 0) {
        
        printf("Packet lost\n");
        return;
    }
    else {
    //prepare and send the ACK
        Packet packet;
        packet.header.seq_ack = seqnum;
        packet.header.len = sizeof(packet.data);
        packet.header.cksum = getChecksum(packet);
        sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    }
}

Packet serverReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
                     
    Packet packet;

    while (1) {
        //Receive a packet from the client
        
        recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // validate the length of the packet
       
        if(packet.header.len != strlen(packet.data)){
            printf("Packet lengths mismatch\n");
            break;
        }

        
        printf("Received: ");
        printPacket(packet);

        //verify the checksum and the sequence number
        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected %d\n", getChecksum(packet));
            
            serverSend(sockfd, address, *addrlen, !seqnum);
        }
        else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected %d\n", seqnum);
            
            serverSend(sockfd, address, *addrlen, !seqnum);
        }
        else {
            printf("Good packet\n");
            
            serverSend(sockfd, address, *addrlen, seqnum);
            break;
        }
    }
    printf("\n");
    return packet;
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        exit(0);
    }

    puts("Waiting for client...");

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket

    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failure to setup an endpoint socket");
        exit(1);
    }

 
    //Setup the server address to bind using socket addressing structure
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    if ((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0){
        perror("Failure to bind server address to the endpoint socket");
        exit(1);
    }

    // open file
    int fp = open(argv[2], O_RDWR);
    if(fp < 0){
        perror("failed to open\n");
        return EXIT_FAILURE;
    }

    // get file contents from client and save it to the file
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        
        packet = serverReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
        write(fp, packet.data, strlen(packet.data));
        seqnum = (seqnum + 1) % 2;
    } while (packet.header.len != 0);

    //close file and socket
   
    close(fp);
    close(sockfd);

    return 0;
}