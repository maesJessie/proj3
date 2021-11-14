#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

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

//void producer (void *thread){
//	int i = 0;
//	char value = '0';
//	int count = 0;
//	while(i < SIZE_OF_BUFFER){
//		if(count > 9)
//			count = 0;
//		value = (char)count;
//		usleep_range(0, 1);
//		sys_enqueue_buffer_421(&value);
//		i++;
//		count++;
//	}
//}

//void consumer(void *thread){
//	int i = 0;
//	char value = '0';
//	while(i < SIZE_OF_BUFFER){
//		usleep_range(0, 1);
//		printk("%ld ", sys_dequeue_buffer_421(&value));
//		i++;
//	}
//}

int main(int argc, char *argv[]) {
	long rv;
	char *data = "1";
	//INIT
	rv = init_buffer_421();
		
	if(rv < 0) {
		perror("Init syscall failed");
	}
	else {
		printf("Init syscall ran successfully, check dmesg output\n");
	}
	//ENQUEUE
	rv = enqueue_buffer_421(data);

	if(rv < 0) {
		perror("Insert syscall failed");
	}
	else {
		printf("Insert syscall ran successfully, check dmesg output\n");
	}
	//DEQUEUE
	rv = dequeue_buffer_421(data);
		
	if(rv < 0) {
		perror("Print syscall failed");
	}
	else {
		printf("Print syscall ran successfully, check dmesg output\n");
	}
	//Delete
	rv = delete_buffer_421();
		
	if(rv < 0) {
		perror("Delete syscall failed");
	}
	else {
		printf("Delete syscall ran successfully, check dmesg output\n");
	}
	return 0;
}
