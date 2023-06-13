This program is a small example to show how threads work.
Written using Linux's pthreads to implement the threads
It solves two problems that require the use of manipulating the same data structure and using mutex lock and unlock to prevent race conditions among the other issues that arise when using threads

Must be run in a Linux or Unix enviornment as it uses Linux's pthreads

When compiling it may be required to link the pthread library using the -pthread flag 
EX: g++ -o lab3 lab3.cpp -pthread