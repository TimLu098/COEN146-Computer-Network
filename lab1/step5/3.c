//Name: Yen-Jung Lu 
////Date: 04/06/2022 Wednesday(the day you have lab) 
////Title: COEN146 Lab1 – step 3 
//// Description: This program computes a copy of the texts, and also calculate the time for the copy to be processed.

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<time.h>

#define BUF_SIZE 2048
#define MODE 0666
int func_copy(char *src_filename, char *dst_filename);


int syscall_copy(char* src_filename, char* dst_filename);
 
void check_copy_times(char* src_filename, char* dst_filename) {


	clock_t func_start, func_end, syscall_start, syscall_end;
// capture runtime of func_copy() using start clock, call the copy, end the clock

	func_copy(src_filename, dst_filename);
	
	double func_time;


	func_start = clock();
	
	func_end = clock();

	func_time = ((double) (func_end - func_start)) / CLOCKS_PER_SEC;
	
// // capture runtime of syscall_copy() using start clock, call the copy, end the clock
	

	syscall_copy(src_filename, dst_filename);

	double syscall_time;


        syscall_start = clock();

        syscall_end = clock();

        syscall_time = ((double) (syscall_end - syscall_start)) / CLOCKS_PER_SEC;
	
	

	printf("time to copy using functions: %.7f\n", func_time);
	printf("time to copy using syscalls: %.7f\n", syscall_time);
}


int main(int argc, char* argv[]) {
	if (argc != 3) {  // check correct usage
		printf("usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
// get the source and destination files from the command line arguments
	char* src_filename;
        char* dst_filename;

        src_filename = argv[1];
        dst_filename = argv[2];

        func_copy(src_filename, dst_filename);
	syscall_copy(src_filename, dst_filename);

// call the check copy times function
	check_copy_times(src_filename, dst_filename);	

	return 0;


}




int func_copy(char *src_filename, char *dst_filename)
{
        FILE *src, *dst;

        src= fopen(src_filename, "r");
        if(src==NULL)
        {
                fprintf(stderr, "unable to open/create %s for writing: %i\n","test_file", errno);
                exit(0);
        }

        dst= fopen(dst_filename, "w");

        if(dst==NULL)
        {
                fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
                exit(0);
        }

        void *buf= malloc((size_t)BUF_SIZE);


         int size=0;

        while(!feof(src))
        {
                size= fread(buf, 1 ,BUF_SIZE, src);
                        
                fwrite(buf, 1 , size, dst);
        }


        fclose(src);
        fclose(dst);
        return 0;

}



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

