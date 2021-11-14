#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define SIZE_OF_BUFFER 20
#define DATA_LENGTH 1024

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

long init_buffer_421(void) {
	return syscall(__NR_init_buffer_421);
}

long enqueue_buffer_421(char *data) {
	return syscall(__NR_enqueue_buffer_421, data);
}

long dequeue_buffer_421(char *data) {
	return syscall(__NR_dequeue_buffer_421, data);
}

long delete_buffer_421(void) {
	return syscall(__NR_delete_buffer_421);
}

void producer(void *thread){
	int i = 0;
	char value = '0';
	int count = 0;
	while(i < SIZE_OF_BUFFER){
		if(count > 9)
			count = 0;
		value = (char)count;
		sleep(rand() % 2);
		enqueue_buffer_421(&value);
		i++;
		count++;
	}
}

void consumer(void *thread){
	int i = 0;
	char value = '0';
	while(i < SIZE_OF_BUFFER){
		sleep(rand() % 2);
		dequeue_buffer_421(&value);
		i++;
	}
}


int main(int argc, char *argv[]) {
	long rv;

        printf("Here we test to see if our main program works.\n");

        rv = init_buffer_421();

        
        pthread_t thread[DATA_LENGTH];


        producer(thread);
        
        consumer(thread);

        delete_buffer_421();
	
	{//here we will try to reinitialize the buffer after we initialize the buffer
                long rv;

                init_buffer_421();

                rv = init_buffer_421();

                if(rv == -1)
                        printf("The test was a success.\n\n");

                else
                        printf("The test was a failure.\n\n");

                delete_buffer_421();

        }//end of test case

	{
                //Here we test to see if the dequeue function will return
                //-1 on an empty buffer.
                long rv;

                
                char value = '0';

                printf("\nHere we are testing the dequeue function on an empty >");
                init_buffer_421();
                pthread_t thread[DATA_LENGTH];
                rv = dequeue_buffer_421(&value);
                if(rv == -1)
                        printf("The test was a success.\n\n");
		       
		else
                        printf("The test was a failure.\n\n");
                delete_buffer_421();
        }//end of test case
		   
	{//Here we will test the dequeue function on an empty buffer
                long rv;
                char value = '0';
                printf("Here we are testing the dequeue function on an unintial>");
        
                pthread_t thread[DATA_LENGTH];

                rv = dequeue_buffer_421(&value);

                if(rv == -1)
                        printf("The test was a success.\n\n");

                else
                        printf("The test was a failure.\n\n");


        }//end of test case
		   
	{//Here we will test our enqueue function with a full buffer
                long rv;

                char value = '0';

                printf("Here we test to the enqueue function on a full buffer.>");
                init_buffer_421();
        
                pthread_t thread[DATA_LENGTH];
                producer(thread);
                //Calling enqueue again
                rv = enqueue_buffer_421(&value);
 		if(rv == -1)
                        printf("The test was a success.\n\n");
                else
                        printf("The test was a failure.\n\n");
                delete_buffer_421();
        }//end of test case
		
	{//Here we will test our enqueue function with an uninitialized buffer
                long rv;
                char value = '0';
                printf("Here we test to the enqueue function on an uninitialize>");



                rv = enqueue_buffer_421(&value);

                if(rv == -1)
                        printf("The test was a success.\n\n");

                else
                        printf("The test was a failure.\n\n");

        }//end of test case
		       
	{//Testing delete on an uninitialized buffer
                printf("Here we will test to our delete function on an uninitial");
                long rv;
                rv = delete_buffer_421();
                if(rv == -1)
                        printf("The test was a success.\n\n");
                else
                        printf("The test was a failure.\n\n");
        }//end of test case
		       
	return 0;
}
