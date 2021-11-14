#include "buffer.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>

//Global variable
ring_buffer_421_t myBuffer;

SYSCALL_DEFINE0(init_buffer_421){
	int i = 1;
	node_421_t *curr = (node_421_t*)kmalloc(sizeof(node_421_t), GFP_KERNEL);
	curr->data = 0;
	myBuffer.length = 1;
	myBuffer.read = curr;
	if(myBuffer.read == NULL){
		return -1;
	}
	while(i < SIZE_OF_BUFFER){
		i++;
		curr->next = (node_421_t*)kmalloc(sizeof(node_421_t), GFP_KERNEL);
		curr = curr->next;
		curr->data = 0;
		myBuffer.length++;
	}
	myBuffer.write = curr;
	curr->next = myBuffer.read;
	return 0;
}

SYSCALL_DEFINE1(insert_buffer_421, int, i){
	node_421_t *curr = myBuffer.read;
	int count = 0;
	if(myBuffer.read == NULL){
		printk("Buffer uninitialized\n");
		return -1;
	}
	while(curr != myBuffer.write && curr->data != 0){
		curr = curr->next;
		count++;
	}
	if(count >= SIZE_OF_BUFFER){
		printk("Buffer Full\n");
		return -1;
	}
	curr->data = i;
	return 0;
}

SYSCALL_DEFINE0(print_buffer_421){
	node_421_t *curr = myBuffer.read;
	int i = 0;
	if(myBuffer.read == NULL){
		printk("Buffer uninitialized\n");
		return -1;
	}
	while(i < myBuffer.length){
		printk("%i -->", curr->data);
		curr = curr->next;
		i++;
	}
	return 0;
}

SYSCALL_DEFINE0(delete_buffer_421){
	node_421_t *curr = myBuffer.read;
	node_421_t *prev = curr;
	int i = 0;
	if(myBuffer.read == NULL){
		printk("Buffer does not exist");
		return -1;
	}
	while(i < SIZE_OF_BUFFER){
		prev = curr;
		curr = curr->next;
		kfree(prev);
		prev = NULL;
		i++;
	}
	myBuffer.read = NULL;
	myBuffer.write = NULL;
	return 0;
}
