#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"

static ring_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;

long init_buffer_421(void) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printf("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node_421_t *node;
	node = (node_421_t *) kmalloc(sizeof(node_421_t));
	// Create the rest of the nodes, linking them all together.
	node_421_t *current;
	int i;
	current = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		current->next = (node_421_t *) kmalloc(sizeof(node_421_t));
		current = current->next;
	}
	// Complete the chain.
	current->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	sem_init(&mutex, 0, 1);
	sem_init(&fill_count, 0, SIZE_OF_BUFFER);
	sem_init(&empty_count, 0, 0);

	return 0;
}

long enqueue_buffer_421(char * data) {
	// NOTE: You have to modify this function to use semaphores.
	if (!buffer.write) {
		printf("write_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	//Lock the mutex and decrease the empty_count
	sem_wait(&mutex);
	sem_wait(&empty_count);
	
	//critical section
	memcpy(buffer.write->data, data, DATA_LENGTH);
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;
	
	//Add to the fill_count and release the mutex
	sem_post(&fill_count);
	sem_post(&mutex);

	return 0;
}

long dequeue_buffer_421(char * data) {
	// NOTE: Implement this function.
	
	if(!buffer.read){
		printf("read_buffer_421(): The buffer does not exist. Aborting.\n")
		return -1;
	}

	//Here we lock the mutex and decrease the full count
	sem_wait(&mutex);
	sem_wait(&full_count);

	
	//Here we will Copy 1024 bytes from the read node 
        //into the provided buffer data.
	memcpy(buffer.read->data, data, DATA_LENGTH);

	//Advance the pointer
	buffer.read = buffer.read->next;
	buffer.legnth--;

	//Add to the empty_count and release the mutex
	sem_post(&empty_count);
	sem_post(&mutex);
	return 0;
}

long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printf("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	node_421_t *temp;
	node_421_t *current = buffer.read->next;
	while (current != buffer.read) {
		temp = current->next;
		kfree(current);
		current = temp;
	}
	// Free the final node.
	kfree(current);
	current = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	return 0;
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int value;
	sem_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	return;
}

void producer(void *thread){
	int i = 0;
	char value = '0';
	int count = 0;
	while(i < SIZE_OF_BUFFER){
		if(count > 9)
			count = 0;
		value = count + '0';
		sleep(rand() % 0 + 1);
		enqueue_buffer_421(value);
		i++;
		count++;
		}
	}
}

void consumer(void *thread){
	int i = 0;
	char value = '0';
	while(i < SIZE_OF_BUFFER){
		sleep(rand() % 0 + 1);
		printf(dequeue_buffer_421(value), " ");
		i++;
		}
	}
}

int main(){
	init_buffer_421();
	pthread_t thread[DATA_LENGTH];
	return 0;
}
