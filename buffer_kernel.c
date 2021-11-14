//Kernel implimentation of buffer_user.c
#include "buffer.h"
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

static ring_buffer_421_t buffer;
static struct semaphore mutex;
static struct semaphore fill_count;
static struct semaphore empty_count;

SYSCALL_DEFINE0(init_buffer_421) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	node_421_t *curr;
	node_421_t *node;
	int i;
	if (buffer.read || buffer.write) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
	// Create the rest of the nodes, linking them all together.
	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t *) kmalloc(sizeof(node_421_t), GFP_KERNEL);
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	sema_init(&mutex, 1);
	sema_init(&fill_count, SIZE_OF_BUFFER);
	sema_init(&empty_count, SIZE_OF_BUFFER);

	return 0;
}

SYSCALL_DEFINE1(enqueue_buffer_421, const char __user *, data) {
	// NOTE: You have to modify this function to use semaphores.
	if (!buffer.write) {
		printk("write_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	//If the buffer is full, do nothing
	if(buffer.length >= SIZE_OF_BUFFER){
		printk("write_buffer_421(): The buffer is full. Aborting.\n");
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
	up(&mutex);
	up(&fill_count);

	return 0;
}

SYSCALL_DEFINE1(dequeue_buffer_421, const char __user *, data) {
	// NOTE: Implement this function.

	if(!buffer.read){
		printk("read_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	//If the buffer is empty, do nothing
	if(buffer.length <= 0){
		printk("write_buffer_421(): The buffer is empty. Aborting.\n");
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
	up(&mutex);
	up(&empty_count);
	return 0;
}


SYSCALL_DEFINE0(delete_buffer_421) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	node_421_t *temp;
	node_421_t *curr = buffer.read->next;
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	while (curr != buffer.read) {
		temp = curr->next;
		kfree(curr);
		curr = temp;
	}
	// Free the final node.
	kfree(curr);
	curr = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	return 0;
}

