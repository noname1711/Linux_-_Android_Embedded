Hw1:
Create 2 threads and print thread's ID with pthread_create() and pthread_self(). Using pthread_join() to make sure that all thread are completed before this program ended. Follow the below command:
```bash
make allhw1
```
Hw2:
Create 3 threads and counter starting at 0. Each thread increases the value of counter by 1, 1,000,000 times. Using mutex lock to make sure synchronize access to the global variable "counter" between multiple threads, to avoid race conditions. Follow this command:
```bash
make allhw2
```
Without mutex lock, the final value counter (maybe 13409473) is often lower than expected (33000000).

Hw3:
Producer make a random number and send to consumer, using pthread_cond_wait and pthread_cond_signal to synchronization, consumer just can read data if producer send done. That program is repeated 10 times. Want to see it, please command: 
```bash
make allhw3
```
Hw4:
Using thread to count of even numbers and odd numbers in random array including 100 numbers. Command:
```bash
make allhw4
```
Hw5:
Using read-write lock with 5 read-threads and 2 write-threads, the starting shared data is 0 and write-thread will update it 1 time. Just command:
```bash
make allhw5
```
Hw6: 

Divide an array of 1 million elements into 4 arrays of equal number of elements. Calculate the sum of the elements in each array and give the sum of the original array. Command:
```bash
make allhw6
```
After both of them, should be clean that:
```bash
make clean
```