# proj3
CMSC421 Project 3
Names: Jonah Ukaegbu and Jessie Maes
Professor: Dr. Maya Larson
Section: 01

Project Implementation

When first approaching this project there were many things to look at before we dived into the thick of things.
We first had to understand the concept of the producer and the consumer. The producer will be using 
the enqueue function to insert nodes into our circular buffer. The consumer will access the dequeue
function and will comprise a single thread or process that will retrieve sequential blocks of characters from
the buffer and then displays them on the screen. In our ouserpace, when creating our circular buffer, we had to intialize
it and setting it up to hold 20 nodes. We would then call our enqueue and dequeue function which in turn would call 
producer and consumer. After that we would print out each node and its data and delete our buffer with no memory leaks or errors.
