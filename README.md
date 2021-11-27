# proj3
CMSC421 Project 3
Names: Jonah Ukaegbu and Jessie Maes
Professor: Dr. Maya Larson
Section: 01

Project Implementation

When first approaching this project there were many things to look at before we dived into the thick of things.
We first had to understand the concept of the producer and the consumer within the user space. The producer will be using 
the enqueue function to insert nodes into our circular buffer. The consumer will access the dequeue
function and will comprise a single thread or process that will retrieve sequential blocks of characters from
the buffer and then displays them on the screen. In our ouserpace, when creating our circular buffer, we had to intialize
it and setting it up to hold 20 nodes. We would then call our enqueue and dequeue function which in turn would call 
producer and consumer. After that we would print out each node and its data and delete our buffer with no memory leaks or errors.

We then moved into the kernel space. The commands for initializing, incrementing, and decrementing a semaphore are different 
here than in the user space. Rather than sem_init(), sem_wait(), and sem_post(), we used sema_init(), down(), and up() respectively. 
The rest of the code remained much the same, with a few exceptions such as changing printf() to printk() and free() to kfree().
Ultimately, the biggest problem we ran into was that we had to change the order of incrementing/decrementing the mutex and semaphores from
their previous user space order (semaphore then mutex) to mutex then semaphore.

In order to use the test files provided, simply compile and run them as standard c executables. Various tests should run on the code, testing its
limits and ability to function.
