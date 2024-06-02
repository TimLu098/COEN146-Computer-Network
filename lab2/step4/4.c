#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
        if(argc!=2)
        {
                // check correct usage
 		fprintf(stderr, "usage: %s <n connections> \n", argv[0]);
                exit(1);
        }

        //Declare and values to n (n connections), np (np parralel connections),
        int RTT0 = 3, RTT1 = 20, RTT2 = 26, RTTHTTP = 47, RTTNP;
        int RTTDNS = (RTT0 + RTT1 + RTT2);

        printf("One object: %d msec\n",RTTDNS+2*RTTHTTP);
        printf("Non-Persistent 6 objects: %d msec\n",RTTDNS+2*RTTHTTP+2*6*RTTHTTP);

        //find how many np (parralel connections)
        int np = atoi(argv[1]);

        // Find Non-Persistant RTT
        RTTNP = (6/np + 1)*2*RTTHTTP;
	int RTT= (6/np + 1)*RTTHTTP;
        printf("%d parallel connection - Persistent: %d msec\n",np,RTTDNS+2*RTTHTTP+RTT);
        printf("%d parallel connection - Non-Persistent: %d msec\n",np,RTTDNS+RTTNP+2*RTTHTTP);
        return 0;
}          
