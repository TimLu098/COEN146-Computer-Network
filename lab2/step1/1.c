#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()



#define BUF_SIZE 2048 //buffer size
// make two src file, two dst files

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char *src_filename;
	char *dst_filename; 
};
// // copies a files from src_filename to dst_filename using functions fopen(), 
// fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	
	FILE *src, *dst;
	src = fopen(src_filename, "r"); // opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	dst = fopen(dst_filename, "w"); // opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n",dst_filename, errno);
	exit(0);
}
	void *buf = malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
		

	if(buf==NULL)
	{
		fprintf(stderr, "unable to allocate memory: %i\n", errno);
		exit(0);
	}
	
	int size;


	while((size = fread(buf, 1 ,BUF_SIZE, src))>0)// reads content of file is loop iterations until end of file
	{
                fwrite(buf, 1 , size, dst);

	}
	// writes bytes_read to dst_filename 
	fclose(src);//closes src file pointer
	fclose(dst);// closes dst file pointer
	free(buf);//frees memory used for buf
	return 0;
}


// thread function to copy one file
void* copy_thread(void* arg) {
	
	struct copy_struct params = *(struct copy_struct*)arg;  // cast/dereference void* to copy_struct

	printf("thread[%i] - copying %s to %s\n", params.thread_id, params.src_filename, params.dst_filename);
	func_copy(params.src_filename, params.dst_filename);//call file copy function
	
	pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
	// check correct usage of arguments in command line
	if (argc < 3) {  
		fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
	
	int pairs= (argc-1)/2;

        int desStart = pairs +1;


	 int num_threads= pairs; 

	char* src_filename;
	char* dst_filename;
	char* src_filenames[num_threads]; // array of source files
	char* dst_filenames[num_threads]; // array of desintation files	

	int j; 

	for(j =1; j<= pairs ; j++)
	{
		src_filenames[j-1] = argv[j];
		dst_filenames[j-1] = argv[j-1+ desStart];		

	}

	 
	pthread_t threads[num_threads]; //initialize threads
	
	struct copy_struct thread_params[num_threads]; // structure for each thread
	int i;
	for (i = 0; i < num_threads; i++) {
	//initialize thread parameters
		thread_params[i].thread_id =i;
		thread_params[i].dst_filename = dst_filenames[i];
		thread_params[i].src_filename = src_filenames[i];
	
		
		pthread_create(&threads[i], NULL, copy_thread,(void *)&thread_params[i]);	
	// create each copy thread use pthread_create(.....);
	//int pthread_create(pthread_t *threadid, pthread_attr_t *attr, void *(*start_routine) (void *arg), void *arg);
	
			

	}
 // wait for all threads to finish
	for (i = 0; i < num_threads; i++) {
	pthread_join(threads[i], NULL);
  	}
	pthread_exit(NULL);
}





