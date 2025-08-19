# What is Thread ?
+ A thread is the smallest unit of execution within a process in an operating system. Threads allow a program to perform multiple tasks concurrently, improving efficiency and responsiveness.

+ The transition from one thread to another is so fast that it is imperceptible. These threads run in parallel, so which one works first is determined by the scheduler.

    If one thread is blocked, the remaining threads will continue to operate normally. However, if one thread crashes or exits, all the remaining threads will be stopped.

+ Context switching including: process context switching and thread context switching. 

    Suppose, I have the following processes: A(10s), B(10s), C(10s)
    and running belike: A(2s) -> B(2s) -> C(2s) -> A(2s) -> ...

    For A to know exactly where A ended, it must save the information related to it at the time of suspension, including: address space, stack space, virtual address space, register set image,... This information is the context. After saving the context, context A will switch to B, and B also saves context B, then it will restore them, so they will know where to start.

+ Multithreading:
    + Running multiple threads simultaneously within a single process.
    + Enables better CPU utilization and faster execution for tasks like I/O operations, parallel computing, and real-time processing.
+ Thread States:
    + New: Created but not yet started.
    + Runnable: Ready to run, waiting for CPU time.
    + Running: Actively executing.
    + Blocked/Waiting: Paused (e.g., waiting for I/O or a lock).
    + Terminated: Execution completed.
+ Thread Types:
    + User-Level Threads: Managed by user-space libraries (lightweight but limited by OS scheduling).
    + Kernel-Level Threads: Managed directly by the OS (more efficient for multicore systems).
# Compare Process and Thread
```text
+---------------------+---------------------------------------------------+---------------------------------------------------+
|       Feature       |                     Process                       |                      Thread                       |
+---------------------+---------------------------------------------------+---------------------------------------------------+
| Definition          | Independent program in execution.                 | Lightweight subunit within a process.             |
| Resource Ownership  | Own memory, files, and resources.                 | Shares memory/resources with other threads.       |
| Creation Time       | Slower (requires OS intervention).                | Faster (lightweight, managed within process).     |
| Context Switching   | Heavy (OS involvement, more overhead).            | Light (less overhead, shared memory).             |
| Communication       | Needs IPC (pipes, sockets, shared memory).        | Direct sharing (variables/objects in process).    |
| Isolation           | High (one process crash doesn’t affect others).   | Low (buggy thread can crash whole process).       |
| Parallelism         | True parallelism on separate CPU cores.           | Concurrent, depends on process scheduling.        |
| Memory Usage        | High (separate address space).                    | Low (shared memory within process).               |
| Security            | More secure (isolated memory).                    | Less secure (shared data risks).                  |
| Examples            | Running multiple apps (Chrome, Word).             | Multiple tabs in a browser (shared process).      |
+---------------------+---------------------------------------------------+---------------------------------------------------+
```
And threads also need an identifier to distinguish each other, however this ID (TID) is not an integer like a process but a struct. Therefore, comparing thread IDs will be much more complicated because it is necessary to compare each element in the struct within it.
# Operation on Thread and Thread management
We declare as follows to be able to use functions that manipulate threads:
```c
#include <pthread.h>
```
+ Create new thread using pthread_create()

    Function Prototype: 
    ```c
    #include <pthread.h>

    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
    ```
    Parameters: 
    + thread (Output)
        + Pointer to a pthread_t variable where the thread ID will be stored
        + Used to reference the thread in other pthread functions
    + attr (Input)
        + Pointer to a thread attributes structure (pthread_attr_t)
        + If NULL, default attributes are used
        + Can specify stack size, scheduling policy, detach state, etc.
    + start_routine (Input)
        + Function pointer to the thread's entry point
        + Must have the signature: void *function_name(void *)
        + The thread begins execution at this function
    + arg (Input)
        + Argument to pass to the start routine
        + Can be any pointer type (cast to void*)
        + For multiple arguments, pass a pointer to a struct
    Return Value: 
    + Returns 0 on success
    + Returns an error number on failure (not in errno)
    Common Errors
    + EAGAIN: Insufficient resources to create another thread
    + EINVAL: Invalid settings in attr
    + EPERM: No permission to set scheduling policy/parameters
+ End of thread using pthread_exit()

    Function Prototype:
    ```c
    #include <pthread.h>

    void pthread_exit(void *retval);
    ```
    Parameters: retval is a pointer to the thread's return value

    + Can be any pointer type (cast to void*)
    + NULL is acceptable if no return value is needed
    + The value should not point to local thread variables (which will be invalid after thread exits)
    Return Value:
    + The retval becomes available to any thread that calls pthread_join on this thread
    + The memory pointed to by retval must remain valid after the thread exits
+ Specifies which thread terminates using pthread_cancel()

    Function Prototype:
    ```c
    #include <pthread.h>

    int pthread_cancel(pthread_t thread);
    ```
    Parameters: thread is the thread ID of the thread to be canceled (obtained from pthread_create or pthread_self)
    
    Return Value:
    + Returns 0 on success
    + Returns a non-zero error code on failure (ESRCH if the thread doesn't exist)
+ Wait for thread to end, avoid zombie thread using pthread_join()

    Blocks the calling thread until the target thread terminates. If the target thread has already terminated, returns immediately

    Function Prototype:
    ```c
    #include <pthread.h>

    int pthread_join(pthread_t thread, void **retval);
    ```
    Parameters: 
    + thread (Input)
        + The thread ID of the thread to wait for
        + Must be a joinable thread (not detached)
    + retval (Output)
        + Pointer to a location where the thread's exit status will be stored
        + If NULL, the exit status is ignored
        + Contains the value passed to pthread_exit() or the return value from the thread function

    Return Value: 
    + Returns 0 on success
    + Returns a non-zero error code on failure:
        + ESRCH: No thread with the specified ID could be found
        + EINVAL: Thread is not joinable (already detached or already joined)
        + EDEADLK: Deadlock detected (joining self)
+ Split thread without join() using pthread_detach()

    Its resources will be automatically reclaimed when it terminates, and it cannot be joined with pthread_join.

    Function Prototype: 
    ```c
    #include <pthread.h>

    int pthread_detach(pthread_t thread);
    ```
    Parameters: thread is the thread ID of the thread to be detached

    Return Value:
    + Returns 0 on success
    + Returns a non-zero error code on failure:
        + ESRCH: No thread with the specified ID could be found
        + EINVAL: Thread is already detached or has been joined
    Key Characteristics:
    + Automatic Cleanup:
        + Detached threads automatically release their resources when they terminate
        + No need to call pthread_join
    + No Joinability:
        + Once detached, a thread cannot be joined
        + Attempting to join a detached thread returns EINVAL
    + One-Time Operation:
        + A thread can only be detached once
        + Cannot "re-attach" a detached thread
+ Get current thread ID using pthread_self()

    The pthread_self function returns the thread ID of the calling thread. It's the pthreads equivalent of getpid() for processes, but for threads within the same process.

    Function Prototype:
    ```c
    #include <pthread.h>

    pthread_t pthread_self(void);
    ```
    Return Value:
    + Returns the thread ID of the calling thread
    + Never fails (no error return)
+ Compare 2 threads using pthread_equal()

    The pthread_equal function is used to compare two thread identifiers to check if they refer to the same thread. This is necessary because pthread_t may be a structure type on some systems, making direct comparison with == unreliable.

    Function Prototype:
    ```c
    #include <pthread.h>

    int pthread_equal(pthread_t t1, pthread_t t2);
    ```
    Parameters: 
    + t1: First thread identifier
    + t2: Second thread identifier
    Return Value: 
    + Returns non-zero (true) if the two thread IDs refer to the same thread
    + Returns 0 (false) if the thread IDs are different
    + No error return - always succeeds
+ Example using all thread function 
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// Global variables for demonstration
pthread_t main_thread_id;
pthread_t detached_thread_id;
volatile bool keep_running = true;

// Thread cleanup handler
void cleanup_handler(void* arg) {
    printf("Cleanup: %s (Thread %lu)\n", (char*)arg, (unsigned long)pthread_self());
}

// Function for joinable worker thread
void* worker_thread(void* arg) {
    int thread_num = *(int*)arg;
    printf("Worker Thread %d started (ID: %lu)\n", 
           thread_num, (unsigned long)pthread_self());
    
    // Push cleanup handlers
    pthread_cleanup_push(cleanup_handler, "Worker resources");
    
    // Simulate work
    for (int i = 0; i < 5; i++) {
        printf("Worker %d working... (%d/5)\n", thread_num, i + 1);
        sleep(1);
        
        // Check if we should exit early
        if (i == 2 && thread_num == 1) {
            printf("Worker %d exiting early via pthread_exit\n", thread_num);
            pthread_exit((void*)(long)(thread_num * 100));
        }
    }
    
    pthread_cleanup_pop(0); // Don't execute cleanup (normal exit)
    printf("Worker %d completed normally\n", thread_num);
    return (void*)(long)(thread_num * 10);
}

// Function for thread that might be cancelled
void* cancellable_thread(void* arg) {
    printf("Cancellable Thread started (ID: %lu)\n", 
           (unsigned long)pthread_self());
    
    pthread_cleanup_push(cleanup_handler, "Cancellable resources");
    
    while (keep_running) {
        printf("Cancellable thread running...\n");
        sleep(1);
        
        // Create a cancellation point
        pthread_testcancel();
    }
    
    pthread_cleanup_pop(0);
    return (void*)999;
}

// Function for detached thread
void* detached_worker(void* arg) {
    detached_thread_id = pthread_self();
    printf("Detached Thread started (ID: %lu)\n", 
           (unsigned long)detached_thread_id);
    
    // Demonstrate pthread_equal
    if (pthread_equal(main_thread_id, detached_thread_id)) {
        printf("ERROR: Detached thread has same ID as main thread!\n");
    } else {
        printf("Detached thread ID different from main thread ID\n");
    }
    
    // Do some work
    for (int i = 0; i < 3; i++) {
        printf("Detached thread working... (%d/3)\n", i + 1);
        sleep(2);
    }
    
    printf("Detached thread completing\n");
    return NULL;
}

// Function to demonstrate thread self-comparison
void* self_comparison_thread(void* arg) {
    pthread_t current_id = pthread_self();
    printf("Self-comparison Thread (ID: %lu)\n", (unsigned long)current_id);
    
    // Compare with itself (should always be equal)
    if (pthread_equal(current_id, current_id)) {
        printf("Thread ID equals itself (expected)\n");
    } else {
        printf("ERROR: Thread ID doesn't equal itself!\n");
    }
    
    // Compare with main thread
    if (pthread_equal(current_id, main_thread_id)) {
        printf("This is the main thread (unexpected!)\n");
    } else {
        printf("This is not the main thread (expected)\n");
    }
    
    return (void*)42;
}

int main() {
    printf("=== Comprehensive Pthread Example ===\n\n");
    
    main_thread_id = pthread_self();
    printf("Main Thread ID: %lu\n", (unsigned long)main_thread_id);
    
    pthread_t worker1, worker2, cancellable, self_compare;
    int thread1_num = 1, thread2_num = 2;
    void* thread_return;
    int rc;
    
    // 1. Create and join regular worker threads
    printf("\n--- Testing pthread_create and pthread_join ---\n");
    rc = pthread_create(&worker1, NULL, worker_thread, &thread1_num);
    if (rc) {
        fprintf(stderr, "Error creating worker1: %d\n", rc);
        return 1;
    }
    
    rc = pthread_create(&worker2, NULL, worker_thread, &thread2_num);
    if (rc) {
        fprintf(stderr, "Error creating worker2: %d\n", rc);
        return 1;
    }
    
    // Wait for worker threads
    pthread_join(worker1, &thread_return);
    printf("Worker 1 returned: %ld\n", (long)thread_return);
    
    pthread_join(worker2, &thread_return);
    printf("Worker 2 returned: %ld\n", (long)thread_return);
    
    // 2. Create and test cancellable thread
    printf("\n--- Testing pthread_cancel ---\n");
    rc = pthread_create(&cancellable, NULL, cancellable_thread, NULL);
    if (rc) {
        fprintf(stderr, "Error creating cancellable thread: %d\n", rc);
        return 1;
    }
    
    // Let it run for a bit, then cancel it
    sleep(2);
    printf("Main thread sending cancellation request...\n");
    pthread_cancel(cancellable);
    
    // Try to join (should work despite cancellation)
    rc = pthread_join(cancellable, &thread_return);
    if (rc == 0) {
        if (thread_return == PTHREAD_CANCELED) {
            printf("Thread was successfully cancelled\n");
        } else {
            printf("Thread returned: %ld (unexpected after cancel)\n", 
                   (long)thread_return);
        }
    } else {
        printf("pthread_join failed: %d\n", rc);
    }
    
    // 3. Create detached thread
    printf("\n--- Testing pthread_detach ---\n");
    pthread_t detached_thread;
    rc = pthread_create(&detached_thread, NULL, detached_worker, NULL);
    if (rc) {
        fprintf(stderr, "Error creating detached thread: %d\n", rc);
        return 1;
    }
    
    // Detach the thread
    rc = pthread_detach(detached_thread);
    if (rc) {
        fprintf(stderr, "Error detaching thread: %d\n", rc);
    } else {
        printf("Thread detached successfully\n");
    }
    
    // 4. Test pthread_self and pthread_equal
    printf("\n--- Testing pthread_self and pthread_equal ---\n");
    rc = pthread_create(&self_compare, NULL, self_comparison_thread, NULL);
    if (rc) {
        fprintf(stderr, "Error creating self-comparison thread: %d\n", rc);
        return 1;
    }
    
    pthread_join(self_compare, &thread_return);
    printf("Self-comparison thread returned: %ld\n", (long)thread_return);
    
    // 5. Demonstrate that we can't join detached thread
    printf("\n--- Testing join on detached thread (should fail) ---\n");
    sleep(1); // Give detached thread time to start
    
    // This should fail because the thread is detached
    rc = pthread_join(detached_thread, NULL);
    if (rc == EINVAL) {
        printf("Correctly failed to join detached thread (EINVAL)\n");
    } else if (rc == ESRCH) {
        printf("Thread not found (may have already finished)\n");
    } else if (rc == 0) {
        printf("Unexpected success joining detached thread\n");
    } else {
        printf("Unexpected error joining detached thread: %d\n", rc);
    }
    
    // 6. Let detached thread finish
    printf("\nWaiting for detached thread to complete...\n");
    sleep(7);
    
    printf("\n=== All tests completed ===\n");
    
    return 0;
}
```
# Thread Synchoronization
Related Concept:
+ Atomic/non-atomic
    + Atomic: Have shared resources but only one thread access
    + Non-atomic: Multiple threads access shared resources at the same time
+ Crictical Section: shared resources usage time
+ Shared resources: Resources shared between processes or threads
## Mutex Lock
### The Core Idea: The "Toilet" Analogy
Imagine a single-occupancy toilet (a critical resource). A mutex is the lock on the door.
+ Unlocked State: The door is open. A person (thread) can walk in, lock the door from the inside, and use the toilet.
+ Locked State: The door is closed. If another person arrives, they must wait outside until the current occupant is finished, unlocks the door, and leaves.
+ The Act of Locking: The person inside turning the lock is the crucial act that prevents others from entering and causing an... unpleasant situation (a race condition).
A Mutex (short for Mutual Exclusion) is a programming primitive that acts like that lock. It's used to ensure that only one thread of execution can access a shared resource (a variable, a data structure, a file, etc.) at a time.
### Why Do We Need It? The Problem of Race Conditions
In multi-threaded programming, threads often need to read and write to the same memory location.

Consider a simple shared variable counter = 0, and two threads that both try to increment it 1,000,000 times.

You might expect the final result to be 2,000,000. But without a mutex, it almost certainly won't be.
This happens because the operation counter++ is not atomic. It's actually three steps:
+ Read the current value of counter into a CPU register.
+ Increment the value in the register.
+ Write the new value back to counter.
Without a lock, the operating system can interrupt a thread at any point. A possible interleaving might be:
```text
+-------+---------------------------+---------------------------+----------------+
| Time  |         Thread 1          |         Thread 2          | Counter Value  |
+-------+---------------------------+---------------------------+----------------+
| t1    | Read counter (gets 0)     |                           | 0              |
| t2    |                           | Read counter (gets 0)     | 0              |
| t3    | Increment (register = 1)  |                           | 0              |
| t4    |                           | Increment (register = 1)  | 0              |
| t5    | Write 1 to counter        |                           | 1              |
| t6    |                           | Write 1 to counter        | 1              |
+-------+---------------------------+---------------------------+----------------+
```
Both threads read the value 0, incremented it to 1, and wrote back 1. The final result is 1, even though two increment operations occurred. This is a race condition.

A mutex lock prevents this by ensuring that once one thread starts the three-step process, no other thread can until the first one is completely finished.

### How to Use a Mutex: The Pattern
The usage pattern is always the same across most programming languages:
+ Create a mutex object.
+ Lock the mutex before entering the critical section (the code that accesses the shared resource).
+ Do the work on the shared resource.
+ Unlock the mutex immediately after leaving the critical section.
### Initialize, Lock and Unlock a mutex
+  Initialize a Mutex
    + Static Initialization (Compile-time)
        Use the macro PTHREAD_MUTEX_INITIALIZER for mutexes declared globally or with static storage class.
        ```c
        #include <pthread.h>

        pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER; // Static initialization
        ```
    + Dynamic Initialization (Run-time)
        Use the function pthread_mutex_init(). This is necessary for mutexes allocated on the heap (via malloc) or for when you need to specify non-default mutex attributes. And must destroy when using dynamic initialization.
        ```c
        #include <pthread.h>
        #include <stdio.h>

        int main() {
            pthread_mutex_t my_mutex; // Declare a mutex
            int init_result;

            // Initialize the mutex with NULL for default attributes
            init_result = pthread_mutex_init(&my_mutex, NULL);
            
            if (init_result != 0) {
                fprintf(stderr, "Failed to initialize mutex\n");
                return 1;
            }

            // ... Use the mutex ...

            pthread_mutex_destroy(&my_mutex); // Don't forget to destroy!
            return 0;
        }
        ```
+ Lock and Unlock a Mutex
    
    Return 0 if success and < 0 if failed.
    When lock a mutex, threads will wait until some thread unlocks the mutex it is holding.
    ```c
        #include <stdio.h>
        #include <pthread.h>

        // Shared counter variable
        int counter = 0;

        // Method 1: Static initialization (for global/static mutex)
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

        void* increment_counter(void* arg) {
            int iterations = *(int*)arg;
            
            for (int i = 0; i < iterations; i++) {
                // Lock the mutex before accessing shared resource
                pthread_mutex_lock(&mutex);
                
                // Critical section - only one thread can execute this at a time
                counter++;
                printf("Thread %lu: counter = %d\n", (unsigned long)pthread_self(), counter);
                
                // Unlock the mutex after finishing with shared resource
                pthread_mutex_unlock(&mutex);
            }
            
            return NULL;
        }

        int main() {
            pthread_t thread1, thread2;
            int iterations = 100000;
            
            printf("Main: Starting with counter = %d\n", counter);
            
            // Create two threads that will increment the counter
            if (pthread_create(&thread1, NULL, increment_counter, &iterations) != 0) {
                perror("Failed to create thread1");
                return 1;
            }
            
            if (pthread_create(&thread2, NULL, increment_counter, &iterations) != 0) {
                perror("Failed to create thread2");
                return 1;
            }
            
            // Wait for both threads to finish
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            
            printf("Main: Final counter value = %d (expected: %d)\n", counter, 2 * iterations);
            
            return 0;
        }
    ```
### Key Properties and Best Practices
Atomicity: The operations lock() and unlock() are themselves atomic. The hardware/OS guarantees that no two threads can be in the process of locking the same mutex at the exact same time.

Ownership: A mutex has a notion of ownership. The thread that locks it is the only one that should unlock it. Unlocking a mutex you don't own leads to undefined behavior.

Don't Hold Locks Longer Than Necessary: Lock right before the critical section and unlock immediately after. Holding a lock for a long time (e.g., during slow I/O operations) destroys performance by forcing other threads to wait needlessly.

Deadlock: A dangerous situation where two or more threads are waiting for locks that the others hold, and none can proceed.

+ Example: Thread 1 locks Mutex A and tries to lock Mutex B. At the same time, Thread 2 locks Mutex B and tries to lock Mutex A. Both threads are now stuck forever.

+ Solution: Always acquire locks in a consistent, predefined global order.
### Mutex Lock example
We can try a problem: sum an array of 1 million integers starting from 0, by dividing the array into 4 equal arrays and each thread will sum a part, using thread + mutex + malloc 
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

// Structure to pass data to threads
typedef struct {
    int* array;
    int start_index;
    int end_index;
    long long partial_sum;
} thread_data_t;

// Global variables
long long total_sum = 0;
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function to calculate partial sum
void* calculate_partial_sum(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    data->partial_sum = 0;
    
    // Calculate partial sum for this thread's segment
    for (int i = data->start_index; i < data->end_index; i++) {
        data->partial_sum += data->array[i];
    }
    
    printf("Thread: Partial sum from index %d to %d = %lld\n",
           data->start_index, data->end_index - 1, data->partial_sum);
    
    // Lock mutex before updating global total
    pthread_mutex_lock(&sum_mutex);
    total_sum += data->partial_sum;
    pthread_mutex_unlock(&sum_mutex);
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int* array = NULL;
    int segment_size = ARRAY_SIZE / NUM_THREADS;
    
    // Allocate memory for the array
    array = (int*)malloc(ARRAY_SIZE * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate memory for array\n");
        return 1;
    }
    
    // Initialize array with values 0 to ARRAY_SIZE-1
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }
    
    printf("Array initialized with values 0 to %d\n", ARRAY_SIZE - 1);
    printf("Segment size per thread: %d elements\n", segment_size);
    
    // Create thread data and launch threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = array;
        thread_data[i].start_index = i * segment_size;
        thread_data[i].end_index = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * segment_size;
        
        if (pthread_create(&threads[i], NULL, calculate_partial_sum, &thread_data[i]) != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
            free(array);
            return 1;
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Calculate expected sum using formula: n(n-1)/2
    long long expected_sum = (long long)(ARRAY_SIZE - 1) * ARRAY_SIZE / 2;
    
    printf("\nResults:\n");
    printf("Expected sum (formula n(n-1)/2): %lld\n", expected_sum);
    printf("Total sum calculated by threads: %lld\n", total_sum);
    printf("Calculation %s\n", (total_sum == expected_sum) ? "SUCCESSFUL!" : "FAILED!");
    
    // Print partial sums from each thread
    printf("\nPartial sums from each thread:\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d: %lld\n", i, thread_data[i].partial_sum);
    }
    
    // Free allocated memory
    free(array);
    
    return 0;
}
```
## Conditional Variable - Polling
###  The problem of producers and consumers
Imagine a busy coffee shop with a single counter:
+ The Shared Buffer: The counter space where finished drinks are placed. It can only hold, say, 5 drinks at a time.
+ The Producers: The baristas. Their job is to make coffee and place it on the counter.
+ The Consumers: The customers. Their job is to take a coffee from the counter.
The problem is coordinating the baristas and customers so that:
+ A barista doesn't try to add a 6th coffee to a full counter (Buffer Overflow).
+ A customer doesn't try to take a coffee from an empty counter (Buffer Underflow).
+ Two baristas don't try to put a coffee in the same spot at the same time (Race Condition).
+ Two customers don't try to grab the same coffee (Race Condition).
Method 1: The Polling (Busy-Waiting) Coffee Shop

In this poorly managed shop, there are no signals or communication.

+ A Barista's (Producer) Life: You finish making a latte. You walk to the counter. If it's full, you don't put the latte down. Instead, you...

    1. Put the latte down on your workstation.

    2. Walk back to the counter.

    3. Peer at it to see if there's a free spot.

    4. See it's still full.

    5. Walk back to your station.

    6. Wait a few seconds.

    7. Repeat steps 2-6 endlessly until a spot is free.

+ A Customer's (Consumer) Life: You want a coffee. You walk to the counter. If it's empty, you don't get a coffee. Instead, you...

    1. Sigh loudly.

    2. Lean on the counter and stare at it.

    3. Peer over the barista's station to see if any drinks are coming.

    4. Check your watch.

    5. Repeat steps 2-4 endlessly until a drink appears.

+ The Result: Extreme inefficiency.

    + The baristas are exhausted from constant, pointless walking and checking.

    + The customers are frustrated and wasting all their time staring at an empty counter.

    + The shop is filled with anxious energy and wasted effort. CPU cycles are being burned with no useful work being done.

Technical Translation: This is Polling. Threads (baristas/customers) are constantly using CPU time to check the state of a condition (is counter full? / is counter empty?) in a tight loop, instead of doing productive work.

Method 2: The Conditional Variable Coffee Shop
This is a well-managed, efficient coffee shop. They have a simple notification system: a small bell on the counter.

+ A Barista's (Producer) Life: You finish making a latte. You walk to the counter.

    1. If the counter is full, you simply stand there and wait. You don't move, you don't check again. You just... wait patiently. You are blocked.

    2. The moment a customer takes a coffee (making a space), they ring the bell once (pthread_cond_signal(&not_full)).

    3. This bell wakes you up. You see the free spot, place your latte down, and ring the other bell twice (pthread_cond_signal(&not_empty)) to let any waiting customers know a new drink is available.

+ A Customer's (Consumer) Life: You want a coffee. You walk to the counter.

    1. If the counter is empty, you don't stare at it. You simply find a chair and take a nap. You are blocked.

    2. The moment a barista adds a drink, they ring the bell twice (pthread_cond_signal(&not_empty)).

    3. This bell wakes you up. You go to the counter, take your coffee, and before you leave, you ring the first bell once (pthread_cond_signal(&not_full)) to let any waiting baristas know a spot is free.

+ The Result: Maximum efficiency and calm.

    + No wasted energy. Baristas only move to make coffee or to place one on the counter. They never make a pointless trip to "check."

    + No customer anxiety. Customers either immediately get a coffee or relax until one is ready. They never actively "wait."

    + The system is perfectly coordinated by the bells (conditional variables). The resource (CPU time) is only used when productive work can be done.

Technical Translation: This is using Conditional Variables. Threads voluntarily put themselves to sleep (pthread_cond_wait()) when they can't proceed, freeing the CPU for other tasks. They are only woken up by an explicit signal (pthread_cond_signal()) from another thread the moment the condition they were waiting for becomes true.
### Polling example 
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define PRODUCER_DELAY 1
#define CONSUMER_DELAY 2

// Shared buffer structure
typedef struct {
    int items[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
} buffer_t;

// Function prototypes
void* producer(void* arg);
void* consumer(void* arg);
void init_buffer(buffer_t* buf);
void print_buffer(buffer_t* buf);

int main() {
    pthread_t producer_thread, consumer_thread;
    buffer_t buffer;
    
    // Initialize the buffer
    init_buffer(&buffer);
    
    printf("Producer-Consumer Problem with Polling (Busy-Waiting)\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producer delay: %d sec, Consumer delay: %d sec\n\n", PRODUCER_DELAY, CONSUMER_DELAY);
    
    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, &buffer);
    pthread_create(&consumer_thread, NULL, consumer, &buffer);
    
    // Let the threads run for a while
    sleep(10);
    
    // Cancel threads (simplified termination for this example)
    pthread_cancel(producer_thread);
    pthread_cancel(consumer_thread);
    
    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    printf("\nSimulation completed.\n");
    
    return 0;
}

// Initialize the buffer
void init_buffer(buffer_t* buf) {
    buf->count = 0;
    buf->in = 0;
    buf->out = 0;
    pthread_mutex_init(&buf->mutex, NULL);
}

// Producer thread function
void* producer(void* arg) {
    buffer_t* buf = (buffer_t*)arg;
    int item = 1;
    
    while (1) {
        // Simulate some work before producing
        sleep(PRODUCER_DELAY);
        
        pthread_mutex_lock(&buf->mutex);
        
        // Polling (busy-waiting) if buffer is full
        while (buf->count == BUFFER_SIZE) {
            pthread_mutex_unlock(&buf->mutex);
            printf("Producer: Buffer full! Polling...\n");
            usleep(500000); // Sleep for 0.5 sec (polling interval)
            pthread_mutex_lock(&buf->mutex);
        }
        
        // Produce item
        buf->items[buf->in] = item;
        printf("Produced: %d (Buffer count: %d)\n", item, buf->count + 1);
        buf->in = (buf->in + 1) % BUFFER_SIZE;
        buf->count++;
        
        print_buffer(buf);
        
        pthread_mutex_unlock(&buf->mutex);
        
        item++;
    }
    
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    buffer_t* buf = (buffer_t*)arg;
    
    while (1) {
        // Simulate some work before consuming
        sleep(CONSUMER_DELAY);
        
        pthread_mutex_lock(&buf->mutex);
        
        // Polling (busy-waiting) if buffer is empty
        while (buf->count == 0) {
            pthread_mutex_unlock(&buf->mutex);
            printf("Consumer: Buffer empty! Polling...\n");
            usleep(500000); // Sleep for 0.5 sec (polling interval)
            pthread_mutex_lock(&buf->mutex);
        }
        
        // Consume item
        int item = buf->items[buf->out];
        printf("Consumed: %d (Buffer count: %d)\n", item, buf->count - 1);
        buf->out = (buf->out + 1) % BUFFER_SIZE;
        buf->count--;
        
        print_buffer(buf);
        
        pthread_mutex_unlock(&buf->mutex);
    }
    
    return NULL;
}

// Print the current state of the buffer
void print_buffer(buffer_t* buf) {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < buf->count) {
            int index = (buf->out + i) % BUFFER_SIZE;
            printf("%d", buf->items[index]);
        } else {
            printf("-");
        }
        
        if (i < BUFFER_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n\n");
}
```
### Conditional Variable example 
```c 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define PRODUCER_DELAY 1
#define CONSUMER_DELAY 2
#define SIMULATION_TIME 10

// Shared buffer structure with conditional variables
typedef struct {
    int items[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;  // Signal when buffer is not empty
    pthread_cond_t not_full;   // Signal when buffer is not full
} buffer_t;

// Function prototypes
void* producer(void* arg);
void* consumer(void* arg);
void init_buffer(buffer_t* buf);
void destroy_buffer(buffer_t* buf);
void print_buffer(buffer_t* buf);

int main() {
    pthread_t producer_thread, consumer_thread;
    buffer_t buffer;
    
    // Initialize the buffer with conditional variables
    init_buffer(&buffer);
    
    printf("Producer-Consumer Problem with Conditional Variables\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producer delay: %d sec, Consumer delay: %d sec\n", PRODUCER_DELAY, CONSUMER_DELAY);
    printf("Simulation will run for %d seconds\n\n", SIMULATION_TIME);
    
    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, &buffer);
    pthread_create(&consumer_thread, NULL, consumer, &buffer);
    
    // Let the threads run for a while
    sleep(SIMULATION_TIME);
    
    // Cancel threads (simplified termination for this example)
    pthread_cancel(producer_thread);
    pthread_cancel(consumer_thread);
    
    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Clean up resources
    destroy_buffer(&buffer);
    
    printf("\nSimulation completed.\n");
    
    return 0;
}

// Initialize the buffer with conditional variables
void init_buffer(buffer_t* buf) {
    buf->count = 0;
    buf->in = 0;
    buf->out = 0;
    pthread_mutex_init(&buf->mutex, NULL);
    pthread_cond_init(&buf->not_empty, NULL);
    pthread_cond_init(&buf->not_full, NULL);
}

// Clean up buffer resources
void destroy_buffer(buffer_t* buf) {
    pthread_mutex_destroy(&buf->mutex);
    pthread_cond_destroy(&buf->not_empty);
    pthread_cond_destroy(&buf->not_full);
}

// Producer thread function using conditional variables
void* producer(void* arg) {
    buffer_t* buf = (buffer_t*)arg;
    int item = 1;
    
    while (1) {
        // Simulate some work before producing
        sleep(PRODUCER_DELAY);
        
        pthread_mutex_lock(&buf->mutex);
        
        // Wait if buffer is full (efficient waiting with conditional variable)
        while (buf->count == BUFFER_SIZE) {
            printf("Producer: Buffer full! Waiting on not_full condition...\n");
            pthread_cond_wait(&buf->not_full, &buf->mutex);
            printf("Producer: Woken up! Buffer has space now.\n");
        }
        
        // Produce item
        buf->items[buf->in] = item;
        printf("Produced: %d (Buffer count: %d)\n", item, buf->count + 1);
        buf->in = (buf->in + 1) % BUFFER_SIZE;
        buf->count++;
        
        print_buffer(buf);
        
        // Signal consumer that buffer is not empty
        pthread_cond_signal(&buf->not_empty);
        
        pthread_mutex_unlock(&buf->mutex);
        
        item++;
    }
    
    return NULL;
}

// Consumer thread function using conditional variables
void* consumer(void* arg) {
    buffer_t* buf = (buffer_t*)arg;
    
    while (1) {
        // Simulate some work before consuming
        sleep(CONSUMER_DELAY);
        
        pthread_mutex_lock(&buf->mutex);
        
        // Wait if buffer is empty (efficient waiting with conditional variable)
        while (buf->count == 0) {
            printf("Consumer: Buffer empty! Waiting on not_empty condition...\n");
            pthread_cond_wait(&buf->not_empty, &buf->mutex);
            printf("Consumer: Woken up! Buffer has items now.\n");
        }
        
        // Consume item
        int item = buf->items[buf->out];
        printf("Consumed: %d (Buffer count: %d)\n", item, buf->count - 1);
        buf->out = (buf->out + 1) % BUFFER_SIZE;
        buf->count--;
        
        print_buffer(buf);
        
        // Signal producer that buffer is not full
        pthread_cond_signal(&buf->not_full);
        
        pthread_mutex_unlock(&buf->mutex);
    }
    
    return NULL;
}

// Print the current state of the buffer
void print_buffer(buffer_t* buf) {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < buf->count) {
            int index = (buf->out + i) % BUFFER_SIZE;
            printf("%d", buf->items[index]);
        } else {
            printf("-");
        }
        
        if (i < BUFFER_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n\n");
}
```
### Compare Polling and Conditional Variable table
```text
+-----------------------+-----------------------------------------------+-------------------------------------------------------------+
|        Aspect         |               Polling (Busy-Waiting)          |                   Conditional Variables                     |
+-----------------------+-----------------------------------------------+-------------------------------------------------------------+
| CPU Usage             | High - Wastes CPU cycles constantly checking  | Low - Threads sleep when waiting                            |
| Efficiency            | Poor - Inefficient use of resources           | Excellent - Optimal resource utilization                    |
| Responsiveness        | Delayed - Depends on polling interval         | Immediate - Instant wakeup on signal                        |
| Implementation Complexity | Simple - Easy to implement                | Moderate - Requires proper synchronization                  |
| Scalability           | Poor - Doesn't scale with many threads        | Excellent - Handles many threads efficiently                |
| Power Consumption     | High - Constant CPU activity                  | Low - CPU can enter low-power states                        |
| Latency               | Variable - Depends on polling frequency       | Minimal - Immediate response to changes                     |
| Resource Contention   | High - Frequent lock acquisition              | Low - Reduced lock competition                              |
| Use Case              | Simple scenarios, short waits,                | Complex synchronization, production code,                   |
|                       | embedded systems without OS support           | efficient waiting                                           |
| Error Prone           | Less prone to deadlock but wastes resources   | More complex but prevents issues if implemented correctly   |
| Code Readability      | Simple but messy with many conditions         | Clear intent with explicit waiting/signaling                |
| Performance Impact    | Negative - Steals CPU time from others        | Positive - Frees CPU for other tasks                        |
| Synchronization Pattern| Active checking                              | Passive waiting                                             |
| Memory Usage          | Lower - No extra synchronization objects      | Slightly higher - Needs condition variable storage          |
| Portability           | Highly portable - Works everywhere            | Requires pthreads or similar threading library              |
+-----------------------+-----------------------------------------------+-------------------------------------------------------------+
```
## Read-Write Lock
A Read-Write Lock (or shared-exclusive lock) is a synchronization primitive that allows concurrent access for read operations while ensuring exclusive access for write operations.

Its fundamental principle is:
+ Multiple threads can hold the read lock simultaneously to read data.
+ Only one thread can hold the write lock, and only when no threads are reading.

This makes it extremely efficient in scenarios where data is read frequently but written to only occasionally.

-> Increases parallelism by allowing multiple threads to read simultaneously, reducing unnecessary waiting when only reading data is needed. However, it will lead to resource starvation, the writing thread will have to wait a long time if there are many threads reading continuously, the reading thread can also be blocked if the write lock is continuous.

Initializer read-write lock
```c
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZED;
```
Locking for reading
```c
pthread_rwlock_rdlock (&rwlock);
```
Locking for writing
```c
pthread_rwlock_wrlock (&rwlock);
```
Open locking
```c
pthread_rwlock_unlock (&rwlock);
```
Destroy locking
```c
pthread_rwlock_destroy (&rwlock);
```
For a basic example
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep()

// Define our shared data and the lock that protects it
#define NUM_READERS 5
#define NUM_WRITERS 2
#define MAX_OPERATIONS 5

int shared_counter = 0;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// Function for reader threads
void* reader_func(void* arg) {
    int reader_id = *(int*)arg;
    free(arg); // Free the allocated memory for the argument

    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        // Simulate some work before reading
        usleep(100000); // 100 ms

        // Lock for reading
        pthread_rwlock_rdlock(&rwlock);
        // --- CRITICAL SECTION (READ) START ---
        printf("Reader %d: reads value %d\n", reader_id, shared_counter);
        // --- CRITICAL SECTION (READ) END ---
        pthread_rwlock_unlock(&rwlock); // Unlock after reading
    }
    return NULL;
}

// Function for writer threads
void* writer_func(void* arg) {
    int writer_id = *(int*)arg;
    free(arg); // Free the allocated memory for the argument

    for (int i = 0; i < MAX_OPERATIONS; ++i) {
        // Simulate some work before writing
        usleep(250000); // 250 ms

        // Lock for writing
        pthread_rwlock_wrlock(&rwlock);
        // --- CRITICAL SECTION (WRITE) START ---
        shared_counter++;
        printf("Writer %d: updated value to %d\n", writer_id, shared_counter);
        // --- CRITICAL SECTION (WRITE) END ---
        pthread_rwlock_unlock(&rwlock); // Unlock after writing
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

    printf("Starting with shared_counter = %d\n", shared_counter);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        int *arg = malloc(sizeof(*arg)); // Allocate memory for thread id
        if (arg == NULL) {
            perror("malloc for reader id");
            exit(EXIT_FAILURE);
        }
        *arg = i + 1;
        if (pthread_create(&readers[i], NULL, reader_func, arg) != 0) {
            perror("Failed to create reader thread");
            exit(EXIT_FAILURE);
        }
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        int *arg = malloc(sizeof(*arg)); // Allocate memory for thread id
        if (arg == NULL) {
            perror("malloc for writer id");
            exit(EXIT_FAILURE);
        }
        *arg = i + 1;
        if (pthread_create(&writers[i], NULL, writer_func, arg) != 0) {
            perror("Failed to create writer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all reader threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    // Wait for all writer threads to finish
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy the read-write lock
    pthread_rwlock_destroy(&rwlock);

    printf("Final value of shared_counter = %d\n", shared_counter);
    printf("All threads finished.\n");

    return 0;
}
```
# Linux Thread exam
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
