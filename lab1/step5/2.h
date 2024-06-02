#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>

#define BUF_SIZE 2048
#define MODE 0666

int syscall_copy(char* src_filename, char* dst_filename)  {
        int src_fd = open(src_filename, O_RDONLY); 

        if (src_fd < 0) { 
                fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);

                close(src_fd);
                exit(2);


        }



int dst_fd = creat(dst_filename,MODE);  
        if (dst_fd < 0) { 
                fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
                close(dst_fd);
                exit(3);
        }

        char  buf[BUF_SIZE];

        int in=0;
        int out;
while(1){

      in = read(src_fd, buf, BUF_SIZE);
                if(in<=0)
                        break;
	out =  write (dst_fd, buf, in);
                if(out<=0)
                        break;

        }

	close(src_fd);

	close(dst_fd);


	return 0;

}





int main(int argc, char* argv[]) {
        if (argc != 3)
        {  // check correct usage
                fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
                exit(1);
        }



	char * src_filename;
        char * dst_filename;


        src_filename = argv[1];
        dst_filename = argv[2];


syscall_copy(src_filename, dst_filename);


//call the check copy times function
	return 0;
}
