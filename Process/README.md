# Intro about Process

Program: executable file built from source

Process: is a program that is allowed to use system resources such as RAM, CPU, etc...

On the system there can be many different processes running at the same time. To distinguish them in general, we use PID (process ID) and each process will have only 1 unique PID.

List all process are running:
```bash
ps -aux
```


# Command-line arguments 

Command-line arguments are inputs passed to a program when it is executed from the command line (terminal/shell). These arguments allow users to customize the program's behavior without modifying its source code.

How they work:
+ When you run a program, you can provide additional parameters after the program name.
+ These arguments are stored as strings in an array (often called argv or args).
+ argc gives the number of arguments.
+ argv is the content.
+ The first argument (argv[0]) is typically the program's name itself.

For example named program.c:
```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Program Name: %s\n", argv[0]);
    printf("Number of Arguments: %d\n", argc - 1);
    
    for (int i = 1; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}
```

Compile and run:
```bash
gcc program.c -o program
./program arg1 arg2 123
```

So we have output:
```text
Program Name: ./program
Number of Arguments: 3
Argument 1: arg1
Argument 2: arg2
Argument 3: 123
```

Common uses:
+ Specifying input/output files.
+ Setting flags.
+ Passing configuration values.


# Memory Layout

There are many segment of memory
```text
                 Virtual memory address (hexadecimal)
                 ^  (increasing addresses)
                 |
+-----------------------------+ 
|       Kernel space          |
| (mapped but not accessible) |
+-----------------------------+ 0xC0DDD000
|         argv, env           |
+-----------------------------+
|           Stack             |
|     (grows downwards)       |
+-----------------------------+ <-- Top of stack
|      Unallocated memory     |
+-----------------------------+ <-- Program break
|            Heap             |
|     (grows upwards)         |
+-----------------------------+ <-- end
|    Uninitialized data (bss) |  
+-----------------------------+ <-- edata
|    Initialized data         |  
+-----------------------------+ <-- etext
|       Text (code)           |  
+-----------------------------+ 0x08048000
|       Unused (NULL)         |
+-----------------------------+ 0x00000000
```
5 segments we really need to care about: Stack, Heap, Uninitialized data, Initialized data and Text

## Text segment
Also known as the code segment, is a section of a program's memory layout in computing that contains the executable instructions of a program. It is a part of the process memory and is typically read-only to prevent accidental or malicious modification of the program's code during execution:
+ Contains Executable Code: Stores the machine instructions that the CPU executes. Includes functions, control structures, and program logic.
+ Read-Only Memory: Usually marked as read-only to prevent runtime modifications. Helps in security (prevents code injection attacks) and ensures program stability.
+ Fixed Size at Compile Time: The size is determined when the program is compiled and linked. Unlike the heap or stack, it does not grow or shrink during runtime.
+ Shared Among Processes: In multi-process environments (e.g., multiple instances of the same program), the text segment may be shared to save memory.

## Initialized data segment and Uninitialized data segment

Initialized Data Segment (data): 
+ Stores explicitly initialized global and static variables (variables with an initial value).    
+ Occupies space in the executable file because initial values must be stored.
+ Read-write memory (can be modified at runtime).
+ Example:
     ```c
    int global_var = 10;         // Initialized global → data segment
    static int static_var = 20;   // Initialized static → data segment

    int main() {
        return 0;
    }
    ```
Uninitialized Data Segment (bss):
+ Stores zero-initialized or uninitialized global/static variables.
+ Does not occupy space in the executable file (just a placeholder for size).
+ Zeroed out by the OS at runtime (all variables start as 0 or NULL).
+ Read-write memory (can be modified later).
+ Example:
    ```c
    int global_uninit;           // Uninitialized global → BSS (auto-zeroed)
    static int static_uninit;    // Uninitialized static → BSS (auto-zeroed)

    int main() {
        static int local_uninit; // Uninitialized static local → BSS (auto-zeroed)
        return 0;
    }
    ```
Compare table:
```text
+-------------------------+------------------------------+-------------------------------+
|        Feature          |    Initialized Data Segment  |   Uninitialized Data Segment  |
+-------------------------+------------------------------+-------------------------------+
| Also known as           | .data                        | .bss                          |
+-------------------------+------------------------------+-------------------------------+
| Stores                  | Global & static variables    | Global & static variables     |
|                         | with explicit initialization | with no explicit init         |
+-------------------------+------------------------------+-------------------------------+
| Initial values          | Defined by programmer        | Automatically set to zero     |
+-------------------------+------------------------------+-------------------------------+
| Memory allocation       | At program load time         | At program load time          |
+-------------------------+------------------------------+-------------------------------+
| Example                 | static int x = 5;            | static int y;                 |
+-------------------------+------------------------------+-------------------------------+
 ```

## Heap segment
Heap segment is a region of a process's memory that is used for dynamic memory allocation during runtime. Unlike the stack (which is automatic and limited) or the data/BSS segments (which are static), the heap allows programs to request and release memory as needed, making it essential for flexible data structures like linked lists, trees, and buffers. It is read and write memory.
+ Dynamic Memory Allocation: Managed manually via
    + malloc(), calloc(), realloc() → allocation.
    + free() → deallocation.
+ Grows Towards Higher Addresses:
    + Expands as more memory is allocated (unlike the stack, which grows downward).
    + Limited by system RAM and process quotas.
+ Fragmentation Issues:
    + External fragmentation: Free memory is scattered, making large allocations harder.
    + Internal fragmentation: Allocated blocks may have unused padding.
+ Slower than Stack Allocation:
    + Requires system calls (brk, sbrk, or mmap in Linux) to resize.
    + No automatic cleanup (must manually free() in C/C++).

## Stack segment
The stack segment is a region of memory used for automatic, short-lived storage of local variables, function parameters, and control data (like return addresses). It operates in a Last-In-First-Out (LIFO) manner and is managed directly by the CPU, making it extremely fast but limited in size.Any local function or variable we create, called a "stack frame", is located in this area. It is read and write memory.
+ Automatic Allocation/Deallocation:
    + Memory is reserved when a function is called and freed when it returns.
    + No manual management (unlike the heap).
+ Stores Local Variables & Function Data:
    + Local variables (e.g., int x inside a function).
    + Function arguments.
    + Return addresses (where to go after a function finishes).
+ Fixed Size (Can Overflow):
    + Default stack size is small.
    + Exceeding it causes a stack overflow
+ Fast Access (CPU-Managed):
    + Uses push/pop instructions (hardware-optimized).
    + No fragmentation (unlike the heap).
+ Grows Downward (Higher → Lower Addresses): The stack pointer moves downward as data is pushed.
--> Compare table:
```text 
+-------------------------+------------------------------+-------------------------------+
|        Feature          |          Heap Segment        |          Stack Segment        |
+-------------------------+------------------------------+-------------------------------+
| Allocation              | Manual (malloc/free, new/del)| Automatic (function calls)    |
+-------------------------+------------------------------+-------------------------------+
| Growth direction        | Grows upwards                | Grows downwards               |
+-------------------------+------------------------------+-------------------------------+
| Lifetime                | Until explicitly freed       | Until function returns        |
+-------------------------+------------------------------+-------------------------------+
| Speed                   | Slower (fragmentation, mgmt) | Faster (simple push/pop)      |
+-------------------------+------------------------------+-------------------------------+
| Typical usage           | Dynamic data structures      | Local variables, call frames  |
+-------------------------+------------------------------+-------------------------------+
| Managed by              | Programmer / runtime libs    | Compiler / CPU                |
+-------------------------+------------------------------+-------------------------------+
```
+ Note: Program break and Top of stack must not exceed each other.

# "Memory Leak" and Debugging tool "valgrind"

A memory leak occurs when a computer program incorrectly manages memory allocations in a way that memory which is no longer needed is not released. This leads to:
+ Gradual reduction of available memory
+ Potential program slowdown or crash
+ System performance degradation in severe cases

Common causes:
+ Forgetting to free allocated memory
+ Losing pointers to allocated memory
+ Exception paths that bypass deallocation code
+ Circular references (in garbage-collected languages)

Valgrind is a powerful open-source tool for memory debugging, memory leak detection, and profiling. Its most used tool is Memcheck, which can detect:
+ Memory leaks
+ Use of uninitialized memory
+ Reading/writing memory after it's freed
+ Reading/writing past allocated blocks
+ Memory corruption
+ Bad frees (double frees, mismatched alloc/free)
Basic Valgrind usage:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./your_program
```
Valgrind Output Interpretation:
+ Definitely lost: Memory that was allocated but definitely not freed
+ Indirectly lost: Memory lost due to losing pointers to a structure
+ Possibly lost: Memory that might still be reachable but looks suspicious
+ Still reachable: Memory that wasn't freed but still has pointers to it
For example of valgrind finding a leak:
```c
// leaky.c
#include <stdlib.h>

void leak() {
    malloc(100);  // Allocated but never freed
}

int main() {
    leak();
    return 0;
}
```
Running Valgrind would report:
```bash
==12345== 100 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x483B7F3: malloc (in /usr/lib/valgrind/vgpreload_memcheck.so)
==12345==    by 0x109146: leak (leaky.c:4)
==12345==    by 0x109156: main (leaky.c:8)
```

# Operations on Process 

## System call fork()
fork() is a fundamental system call in Unix-like operating systems that creates a new process by duplicating the calling process.

Key Characteristics
+ Creates a child process: The new process is an exact copy of the parent process
+ Returns twice:
    + Returns 0 to the child process
    + Returns the child's PID (Process ID) to the parent process
    + Returns -1 if the fork failed
How It Works ?
+ The OS creates a new process (child) that's identical to the parent
+ Both processes continue execution from the point after fork()
+ They have:
    + Same code
    + Same open file descriptors
    + Same memory contents (initially)
    + Different process IDs
```text
                   +-------------------+
                   |   Parent Process  |
                   |   (PID = 1000)    |
                   +-------------------+
                            |
                            |  fork()
                            v
        +-------------------------------------------+
        |   Execution splits into two processes:    |
        +-------------------------------------------+
             |                              |
             v                              v
 +-------------------+            +-------------------+
 |   Parent Process  |            |   Child Process   |
 |   (PID = 1000)    |            |   (PID = 1001)    |
 | return value > 0  |            | return value = 0  |
 +-------------------+            +-------------------+
             |                              |
             |   continues execution         |   continues execution
             |   after fork()                |   after fork()
             v                              v
        [ same code / different memory spaces ]
```
Important Notes
+ Copy-on-Write: Modern systems use this optimization where the child shares the parent's memory until either modifies it
+ Resource Inheritance: The child inherits:
    + Open file descriptors
    + Signal handlers
    + Current working directory
    + User/group IDs
+ Differences:
    + The child gets a unique PID
    + Parent PID (PPID) differs
    + Resource utilization statistics are reset
    + Pending signals are cleared
Common Use Cases:
+ Creating parallel processes
+ Implementing shells (command execution)
+ Server processes handling multiple clients
+ Process isolation
Potential Pitfalls:
+ Zombie processes: Children that exit but aren't waited for by parents
+ Resource exhaustion: Forking too many processes
+ Shared file descriptors: Both processes share the same open files initially
For example:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d)\n", getpid());
    }
    else {
        // Parent process
        printf("Parent process (PID: %d), Child PID: %d\n", getpid(), pid);
    }

    return 0;
}
```
## exec family
The exec family of functions replaces the current process image with a new program while keeping the same process ID. Unlike fork(), exec doesn't create a new process - it transforms the existing one.

Key Characteristics
+ Replaces the current process: Loads a new program into the current process space
+ PID remains the same: The process continues with the same ID but runs different code
+ Never returns on success: The new program takes over execution
+ Only returns on failure: Returns -1 if the execution fails
The Six Variants of exec family:
+ execl()
    + Syntax:
    ```c
    int execl(const char *path, const char *arg0, ..., NULL);
    ```    
    + Usage:
        + Takes a full path to the executable.
        + Arguments are passed as a list (comma-separated).
        + Must end with NULL.
    + Example:
    ```c
    execl("/bin/ls", "ls", "-l", "/home", NULL);
    ```
+ execv()
    + Syntax:
    ```c
    int execv(const char *path, char *const argv[]);
    ```
    + Usage:
        + Takes a full path to the executable.
        + Arguments are passed as an array (vector) of strings.
        + The array must end with NULL.
    + Example:
    ```c
    char *args[] = {"ls", "-l", "/home", NULL};
    execv("/bin/ls", args);
    ```
+ execlp()
    + Syntax:
    ```c
    int execlp(const char *file, const char *arg0, ..., NULL);
    ```
    + Usage:
        + Searches for the executable in the PATH environment variable.
        + Arguments are passed as a list.
        + Must end with NULL.
    + Example:
    ```c
    execlp("ls", "ls", "-l", "/home", NULL);
    ```
+ execvp()
    + Syntax:
    ```c
    int execvp(const char *file, char *const argv[]);
    ```
    + Usage:
        + Searches for the executable in PATH.
        + Arguments are passed as an array.
        + The array must end with NULL.
    + Example:
    ```c
    char *args[] = {"ls", "-l", "/home", NULL};
    execvp("ls", args);
    ```
+ execle()
    + Syntax:
    ```c
    int execle(const char *path, const char *arg0, ..., NULL, char *const envp[]);
    ```
    + Usage:
        + Takes a full path to the executable.
        + Arguments are passed as a list.
        + Allows specifying a custom environment (envp).
        + Must end with NULL before envp.
    + Example:
    ```c
    char *env[] = {"USER=test", "PATH=/bin", NULL};
    execle("/bin/ls", "ls", "-l", NULL, env);
    ```
+ execve() (The actual system call)
    + Syntax:
    ```c
    int execve(const char *path, char *const argv[], char *const envp[]);
    ```
    + Usage:
        + Takes a full path to the executable.
        + Arguments are passed as an array.
        + Allows specifying a custom environment (envp).
        + The array must end with NULL.
    + Example:
    ```c
    char *args[] = {"ls", "-l", "/home", NULL};
    char *env[] = {"USER=test", "PATH=/bin", NULL};
    execve("/bin/ls", args, env);
    ```
Example
```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        char *args[] = {"ls", "-l", "/home", NULL};
        execvp("ls", args);  // Searches PATH for "ls"
        perror("execvp failed");  // Only reached if exec fails
        return 1;
    } else {
        // Parent process
        wait(NULL);  // Wait for child to finish
        printf("Child process completed.\n");
    }

    return 0;
}
```
## Process Termination
When allocating or creating a new process, it is possible to terminate a process
+ Normal termination: when the program runs and calls return or system call exit()
+ Abnormal termination: terminated by kill
For example: if there is a program with PID 29553, then when the command
```bash
kill -9 29553
```
And then that program will terminate immediately.
# Process Management

## "Do one thing and do it well"
Each system call should only do one thing and do it well.

Question: why after fork is there a parent-child relationship and not simply this process-that process?

When the child finishes, it must send its status to the parent and the parent will get that status (this status is return 0, return 1,... sent through the system call exit()) and the parent will get the information through the system call wait().
## System call wait() and exit()
System call wait()
+ Purpose:
    + Suspends the parent process until any of its child processes terminates.
    + Returns the PID of the terminated child and its exit status.
+ Syntax:
    ```c
    #include <sys/types.h>   // for pid_t
    #include <sys/wait.h>    // for wait
    pid_t wait(int *status);
    ```
    And status is a pointer to an integer where the child's exit information is stored.
+ Macros to Decode status:
```text
+---------------------+-------------------------------------------------------+---------------------------------------------+
|        Macro        |                     Description                       |                  Usage Example              |
+---------------------+-------------------------------------------------------+---------------------------------------------+
| WIFEXITED(status)   | True if child exited normally (exit() or return).     | if (WIFEXITED(status)) { ... }              |
| WEXITSTATUS(status) | If WIFEXITED is true, returns the exit code (0-255).  | int code = WEXITSTATUS(status);             |
| WIFSIGNALED(status) | True if child was killed by a signal.                 | if (WIFSIGNALED(status)) { ... }            |
| WTERMSIG(status)    | If WIFSIGNALED is true, returns signal number.        | int sig = WTERMSIG(status);                 |
| WCOREDUMP(status)   | True if child produced a core dump.                   | if (WCOREDUMP(status)) { ... }              |
| WIFSTOPPED(status)  | True if child was stopped (e.g., by SIGSTOP).         | if (WIFSTOPPED(status)) { ... }             |
| WSTOPSIG(status)    | If WIFSTOPPED is true, returns the stopping signal.   | int sig = WSTOPSIG(status);                 |
| WIFCONTINUED(status)| True if child was resumed (e.g., by SIGCONT).         | if (WIFCONTINUED(status)) { ... }           |
+---------------------+-------------------------------------------------------+---------------------------------------------+
```
System call waitpid()
+ Problem: The wait() system call has some limitations:
    + If the parent process creates multiple children, it cannot use wait() to track a specific child process.
    + If the child process does not terminate, wait() always blocks.
-> So we use waitpid() to wait for the specific child process to terminate according to PID.
+ Syntax:
    ```c
    #include <sys/types.h>
    #include <sys/wait.h>

    pid_t waitpid(pid_t pid, int *status, int options);
    ```
    + Parameters
        + pid: Specifies which child process(es) to wait for:
            + \> 0: Wait for the specific child with this PID
            + == -1: Wait for any child (like wait())
            + == 0: Wait for any child in the same process group
            + \< -1: Wait for any child in the process group equal to the absolute value of pid
        + status: Pointer to store termination status (same as wait())
            + Can be NULL if status isn't needed
            + Use macros like WIFEXITED, WEXITSTATUS to examine
        + options: Bitmask of options
            + WNOHANG: Return immediately if no child has exited
            + WUNTRACED: Also return if child is stopped
            + WCONTINUED: Also return if stopped child has resumed
    + Return Value:
        + On success: PID of the child whose state changed
        + 0: If WNOHANG was used and no child was available
        + -1: On error (sets errno)
+ Key Features
    + Targeted Waiting: Can wait for a specific child process
    + Non-blocking Option: WNOHANG allows polling without blocking
    + State Monitoring: Can detect stopped/continued states
    + Process Group Control: Can wait for children in specific groups
System call exit() 
+ A system call called exit() is used to terminate a program. In environments with multiple threads, this call indicates that the thread execution is finished. After using the exit() system function, the operating system recovers the resources used by the process.
+ Example:
    ```c
    #include <stdlib.h>

    int main() {
        if (error_condition) {
            exit(EXIT_FAILURE);  // Program exits with failure status
        }
        exit(EXIT_SUCCESS);     // Program exits with success status
    }
    ```
Example:
+ Basic exit() and wait()
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/wait.h>
    #include <unistd.h>

    int main() {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Child process
            printf("Child process (PID: %d) running\n", getpid());
            sleep(2);  // Simulate work
            printf("Child process exiting\n");
            exit(42);  // Child exits with status 42
        }
        else {
            // Parent process
            printf("Parent process (PID: %d) waiting for child\n", getpid());
            
            int status;
            pid_t child_pid = wait(&status);  // Wait for any child
            
            if (child_pid == -1) {
                perror("wait failed");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                printf("Child %d exited with status: %d\n", 
                    child_pid, WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status)) {
                printf("Child %d killed by signal: %d\n", 
                    child_pid, WTERMSIG(status));
            }
        }

        return EXIT_SUCCESS;
    }
    ```
+ Multiple Children with wait()
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/wait.h>
    #include <unistd.h>

    int main() {
        for (int i = 0; i < 3; i++) {
            pid_t pid = fork();
            
            if (pid == 0) {
                // Child process
                printf("Child %d (PID: %d) started\n", i, getpid());
                sleep(i + 1);  // Each child sleeps different duration
                exit(i);       // Exit with different status
            }
        }

        // Parent waits for all children
        int status;
        pid_t child_pid;
        
        while ((child_pid = wait(&status)) != -1) {
            if (WIFEXITED(status)) {
                printf("Child %d exited with status: %d\n",
                    child_pid, WEXITSTATUS(status));
            }
        }

        printf("All children finished\n");
        return EXIT_SUCCESS;
    }
    ```
+ waitpid() with Specific Child
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/wait.h>
    #include <unistd.h>

    int main() {
        pid_t child1 = fork();
        
        if (child1 == 0) {
            // First child
            sleep(3);
            exit(10);
        }

        pid_t child2 = fork();
        
        if (child2 == 0) {
            // Second child
            sleep(1);
            exit(20);
        }

        // Parent waits specifically for child2 first
        int status;
        pid_t terminated = waitpid(child2, &status, 0);
        
        if (terminated == child2) {
            printf("Child2 (%d) exited first with status %d\n", 
                child2, WEXITSTATUS(status));
        }

        // Then wait for remaining child
        terminated = waitpid(child1, &status, 0);
        
        if (terminated == child1) {
            printf("Child1 (%d) exited next with status %d\n", 
                child1, WEXITSTATUS(status));
        }

        return EXIT_SUCCESS;
    }
    ```
+ Non-blocking waitpid()
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/wait.h>
    #include <unistd.h>

    int main() {
        pid_t child = fork();
        
        if (child == 0) {
            // Child runs for 5 seconds
            sleep(5);
            exit(99);
        }

        // Parent checks periodically
        int status;
        pid_t result;
        
        while (1) {
            result = waitpid(child, &status, WNOHANG);
            
            if (result == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            else if (result == 0) {
                printf("Child still running... checking again in 1 second\n");
                sleep(1);
            }
            else {
                printf("Child exited with status %d\n", WEXITSTATUS(status));
                break;
            }
        }

        return EXIT_SUCCESS;
    }
    ```
## Orphane and zombie process
Orphane Process
+ An orphan process is a child process that continues to run after its parent process has terminated or exited.
+ How Orphan Processes Are Created ?
    + Parent process terminates before its child process(es)
    + The child process becomes orphaned
    + The init process (PID 1) becomes its new parent
+ Key Characteristics:
    + Not harmful to the system
    + Re-parented to init process
    + init automatically calls wait() to clean them up when they terminate
    + Common in server applications and daemons
+ Example:
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    int main() {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            printf("Child PID: %d\n", getpid());
            sleep(10);  // Child sleeps longer than parent
            printf("Child now has parent PID: %d\n", getppid());  // Will show 1 (init)
            exit(0);
        } else {
            // Parent process
            printf("Parent PID: %d\n", getpid());
            sleep(2);  // Parent exits before child
            printf("Parent exiting\n");
            exit(0);
        }
    }
    ```
Zombie Process
+ A zombie process (or defunct process) is a process that has completed execution but still has an entry in the process table.
+ How Zombie Processes Are Created ?
    + Child process terminates
    + Parent hasn't yet called wait() or waitpid()
    + The process entry remains in the process table
+ Key Characteristics:
    + Process is dead but process descriptor remains
    + Consumes minimal system resources (just PID and exit status)
    + Removed when parent calls wait()
    + If parent dies without waiting, init will clean them up
    + Too many zombies can fill the process table
+ Example:
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    int main() {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            printf("Child PID: %d\n", getpid());
            exit(0);  // Child exits immediately
        } else {
            // Parent process
            printf("Parent PID: %d\n", getpid());
            sleep(20);  // Parent doesn't call wait()
            // Check process list during sleep - child will be zombie
            printf("Parent exiting\n");
            exit(0);
        }
    }
    ```
Comparison Table between Orphane Process and Zombie Process
```text
+---------------------+---------------------------+-------------------------------+
|    Characteristic   |       Orphane Process     |        Zombie Process         |
+---------------------+---------------------------+-------------------------------+
| State               | Still running             | Already terminated            |
| Parent              | Re-parented to init       | Original parent (unless dead) |
| Process Table Entry | Active                    | Minimal (just PID & status)   |
| Resource Usage      | Normal (running process)  | Almost none                   |
| Cleanup             | By init when terminates   | By parent's wait() or init    |
| Creation            | Parent dies before child  | Child dies before parent wait |
| Potential Issues    | Generally harmless        | Can fill process table        |
+---------------------+---------------------------+-------------------------------+
```
Prevention and Handling
+ Preventing Orphane Process
    + Design parent process to outlive children
    + Use process groups and sessions for clean termination
    + In servers, implement proper signal handling
+ Preventing Zombie Process
    + Call wait() or waitpid() in parent
    + Use signal handler for SIGCHLD:
        ```c
        #include <signal.h>
        #include <sys/wait.h>

        void sigchld_handler(int sig) {
            while (waitpid(-1, NULL, WNOHANG) > 0);
        }

        int main() {
            signal(SIGCHLD, sigchld_handler);
            // ... rest of program
        }
        ```
    + Explicitly ignore SIGCHLD:
        ```c
        signal(SIGCHLD, SIG_IGN);
        ```
Real-World Implications
+ Orphane Process
    + Common in daemons that double-fork to detach from terminal
    + Used by servers to handle client connections
+ Zombie Process
    + Temporary state during normal process cleanup
    + Problematic when parents don't properly wait for children
    + Can be observed in ps/top with status 'Z'
        ```bash
        ps -axu | grep defunct
        ```
# Prepare Linux Process exam 
Hw1:
Using system call fork() to create child process and see PID's child, command:
```bash
make allhw1
```
Hw2:
If you want to change running process by child process "ls -l", follow this command:
```bash
make allhw2-1
```
or change to "date", command:
```bash
make allhw2-2
```
when using exec family, we change the present process to new process (in this program, using execvp).

Hw3:
Create 2 process: child and parent. Parent process will send signal SIGUSR1 to child process, I define my signal is "Child process get SIGUSR1 signal + id":
```bash
make allhw3
```
Hw4:
Parent process will wait till the end of child process and print this output. Using WIFEXITED() return true if child process ended by exit and WEXITSTATUS() return exitcode (in this program i define it is 0), see that by command:
```bash
make allhw4
```
Hw5:
Create zombie process and orphan process:
If you want to create zombie process, command:
```bash
make allhw5-z
```
and we can see zombie's status (Z+) by open new terminal, command:
```bash
ps -aux | grep defunct
```
Or if you want to create orphan process, command:
```bash
make allhw5-o
```
and we can check it by command:
```bash
ps -ef | grep myapp
```
change myapp by your name, see pts/0, it mean success. But we only check this in 10s after child process sleeping.

After both of them, should be clean that:
```bash
make clean
```
