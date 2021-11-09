//Kernel implimentation of buffer_user.c
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

static ring_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;

SYSCALL_DEFINE0(init_buffer_421) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node_421_t *node;
	node = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
	// Create the rest of the nodes, linking them all together.
	node_421_t *current;
	int i;
	current = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		current->next = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
		current = current->next;
	}
	// Complete the chain.
	current->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	init_MUTEX(&mutex);
	sem_init(&fill_count, 0, SIZE_OF_BUFFER);
	sem_init(&empty_count, 0, 0);

	return 0;
}

SYSCALL_DEFINE1(enqueue_buffer_421, char *, data) {
	// NOTE: You have to modify this function to use semaphores.
	if (!buffer.write) {
		printk("write_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	//Lock the mutex and decrease the empty_count
	down(&mutex);
	down(&empty_count);
	
	//critical section
	memcpy(buffer.write->data, data, DATA_LENGTH);
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;
	
	//Add to the fill_count and release the mutex
	up(&fill_count);
	up(&mutex);

	return 0;
}

SYSCALL_DEFINE1(dequeue_buffer_421, char *, data) {
	// NOTE: Implement this function.
	
	if(!buffer.read){
		printk("read_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}

	//Here we lock the mutex and decrease the fill count
	down(&mutex);
	down(&fill_count);

	
	//Here we will Copy 1024 bytes from the read node 
        //into the provided buffer data.
	memcpy(buffer.read->data, data, DATA_LENGTH);

	//Advance the pointer
	buffer.read = buffer.read->next;
	buffer.length--;

	//Add to the empty_count and release the mutex
	up(&empty_count);
	up(&mutex);
	return 0;
}

SYSCALL_DEFINE1(producer, void *,thread){
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

SYSCALL_DEFINE1(consumer, void *,thread){
	int i = 0;
	char value = '0';
	while(i < SIZE_OF_BUFFER){
		sleep(rand() % 2);
		printk("%d ", dequeue_buffer_421(&value));
		i++;
	}
}


SYSCALL_DEFINE0(delete_buffer_421) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
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

