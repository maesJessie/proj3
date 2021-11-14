#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_init_buffer_421 442
#define __NR_insert_buffer_421 443
#define __NR_print_buffer_421 444
#define __NR_delete_buffer_421 445

long init_buffer_421(void) {
	return syscall(__NR_init_buffer_421);
}

long insert_buffer_421(void) {
	return syscall(__NR_insert_buffer_421, 1);
}

long print_buffer_421(void) {
	return syscall(__NR_print_buffer_421);
}

long delete_buffer_421(void) {
	return syscall(__NR_delete_buffer_421);
}

int main(int argc, char *argv[]) {
	long rv;
	//INIT
	rv = init_buffer_421();
		
	if(rv < 0) {
		perror("Init syscall failed");
	}
	else {
		printf("Init syscall ran successfully, check dmesg output\n");
	}
	//INSERT
	rv = insert_buffer_421();
		
	if(rv < 0) {
		perror("Insert syscall failed");
	}
	else {
		printf("Insert syscall ran successfully, check dmesg output\n");
	}
	//PRINT
	rv = print_buffer_421();
		
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
