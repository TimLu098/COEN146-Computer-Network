//Name: Yen-Jung Lu 
//Date: 04/06/2022 Wednesday(the day you have lab) 
//Title: COEN146 Lab1 – step 3 
// Description: This program computes a copy of the texts with the function copy 

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>

#define BUF_SIZE 2048

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
int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);		
	}
	
	char* src_filename;
	char* dst_filename;
	
	src_filename = argv[1];
	dst_filename = argv[2];

	//get the source and destination files from the command line arguments
	func_copy(src_filename, dst_filename);
	
	
	return 0;


} 


