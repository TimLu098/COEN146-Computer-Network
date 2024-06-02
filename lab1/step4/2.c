//Name: Yen-Jung Lu 
//Date: 04/06/2022 Wednesday(the day you have lab) 
//Title: COEN146 Lab1 – step 3 
//Description: This program computes a copy of the texts with the system call copy

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
	int src_fd = open(src_filename, O_RDONLY); // opens a file for reading

	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);

		close(src_fd);
		exit(2);


	}


// O_WRONLY - write only
// // O_CREAT - create file if it doesn't exist
// // O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)




// opens a file for writing; erases old file/creates a new file
	int dst_fd = creat(dst_filename,MODE);  // O_WRONLY | O_CREAT | O_TRUNC
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(3);
	}

	char  buf[BUF_SIZE];
	int in=0;
	int out;
	// read/ write loop
	while(1){

		// reads up to BUF_SIZE bytes from src_filename
		in = read(src_fd, buf, BUF_SIZE);
		if(in<=0)
			break;
		// writes bytes_read to dst_filename
		out =  write (dst_fd, buf, in);
		if(out<=0)
			break;

	}
	//closes src_fd file descriptor
	close(src_fd);
	//closes dst_fd file descriptor
	close(dst_fd);

	//frees memory used for buf
	

	return 0;

}



int main(int argc, char* argv[]) {
	if (argc != 3) 
	{  // check correct usage
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
	//get the source and destination files from the command line arguments
	char * src_filename;
        char * dst_filename;

        
        src_filename = argv[1];
        dst_filename = argv[2];

    
        //get the source and destination files from the command line arguments
        syscall_copy(src_filename, dst_filename);
       

	//call the check copy times function
	return 0;
}
