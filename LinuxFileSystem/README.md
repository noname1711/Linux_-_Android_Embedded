# File in Linux

Everything in Linux is a File.

Types of File on Linux:
+ regular file: normal file such as text file, executable file (-)
+ directories file: file contain a list of other file (d)
+ character device file: special file (c)
+ block device file: special file (b)
+ link file: represent another file (l)
+ socket file: represent for a socket (s)
+ pipe file: represent for a pipe (p)

See everything about files:
```bash
ls -l
```
So we will get information similar to the following:
```bash
drwxrwxr-x 4 hungle hungle 4096 May 20 10:53  LinuxFileSystem
```
with:
+ "d":  type of file (directories)
+ "rwxrwxr-x": file permissions
+ "4": number of hardlink
+ "hungle": name of owner
+ "hungle": name of group
+ "4096": size of file (byte)
+ "May 20 10:53": last modified time
+ "LinuxFileSystem": name of file 

File permission:
+ "r": read
+ "w": write
+ "x": excute
The owners of the file are: user(owner)     group       other

Permission can be represented in bit form.

For example:    
```text
Permission | Binary | Decimal
-----------|--------|--------
   rwx     |  111   |   7
   rw-     |  110   |   6
   r--     |  100   |   4
   -w-     |  010   |   2
```

Conversion of permission, example:
```bash
chmod 6446 test.txt
```
The user with the highest authority is root, to access root:
```bash
sudo su
```
or change the permissions under root, example:
```bash
sudo chmod o+w test.txt
```

# Operations on File

Read/Write to file: kernel provides a basic set of system calls to perform reading, writing and manipulate files, including:
+ open(): open
+ read(): read
+ write(): write
+ lseek(): move mouse pointer
+ close(): close
  
To use these systemcalls, declare:
```c
#include<fcntl.h>
#include<unistd.h>
```
System calls are the communication gateway between user space and kernel space. When working with files, networks, and memory, the kernel is asking for help. Normally, if an app fails, the app will just crash, but if the kernel space fails, the operating system will stop.

+ open(): Opens a file or creates a new file if it does not exist and returns a file descriptor (fd) that is an integer representing the file or -1 if the open fails.
    ```c
    int open(const char *pathname, int flags, mode_t mode);
    ```
    with: 

        + pathname: path to file
        + flags: Specify file open mode:
            + O_APPEND: add to end of file
            + O_RDONLY: only read
            + O_WRONLY: only write
            + O_RDWR: read and write
            + O_CREAT: create a new file if it does not exist
            + O_TRUNC: if the file already exists, delete the content
        + mode( only use with O_CREAT): file permissions
    For example:
    ```c
    int fd = open ("test.txt", O_CREAT | O_WRONLY, 0644);
    ```

+ read(): read data from file into buffer
    ```c
    ssize_t read(int fd, void *buf, size_t count);
    ```
    with:

        + fd: file descriptor which return from open()
        + buf: pointer to buffer to hold readable data
        + count: number of bytes want to read
    Returns the number of bytes read, 0 if end of file (EOF), -1 if error.

    Example:
    ```c
    char buffer[100];
    int n = read(fd, buffer, sizeof(buffer));
    ```

+ write(): write data from buffer to file
    ```c
    ssize_t write(int fd, const void *buf, size_t count);
    ```
    with: 

        + fd: file descriptor
        + buf: source data to be write
        + count: number of bytes want to write
    Returns the actual number of bytes written or -1 if fail

    Example:
    ```c
    const char *msg = "Hello world!\n";
    write (fd, msg, strlen(msg));
    ```

+ lseek(): move the read/write pointer in the file to another location
    ```c
    off_t lseek(int fd, off_t offset, int whence);
    ```
    with: 

        + fd: file descriptor 
        + offset: number of bytes to move
        + whence: starting position of movement
            + SEEK_SET: from the begin of file
            + SEEK_CUR: from the current location
            + SEEK_END: from the last of file

    Example: move mouse pointer to 5th byte from beginning of file
    ```c
    lseek (fd, 5, SEEK_SET);
    ```

+ close(): close file, freeing up unnecessary resources
    ```c
    int close(int fd);
    ```
    return 0 if success or -1 if fail

    For example:
    ```c
    close(fd);
    ```

# File management

## Page cache

+ Page cache:
    ```text
    read()   write()
    v        ^
        cache
        v ^
    Disk/Flask
    ```

        + read():
            + 1. Kernel determines which page to read
            + 2. Kernel reads from page cache
            + 3. If page is in page cache, information is read
            + 4. If page is not in page cache, read from physical memory into page cache then read out to user space
        + write():
            + 1. Kernel writes page contents to page cache
            + 2. Page cache will be written to physical memory periodically or when using sync(), fsync() commands

+ Bottle neck:

    For example: we have a RAM drive with a speed of 100000 bytes/s and HDD drive with 1000 bytes/s

    RAM -> HDD (spent 1s) but HDD need 100s to done!

    To fix this, we use intermediate cache memory:
    ```text
    CPU
    v
    Cache
    v
    RAM
    v
    Cache
    v
    HDD
    ```
    RAM spends 1s writing data to cache, cache will send it to HDD in turn, then return execution rights to RAM.

    Data from cache will sent to HDD by 1 of 2 following ways:

        + Call sync()
        + when cache is full

    sync(): use to synchronization and asynchronization

    For example: we have 2 process: A: "hello" and B: "linux programing"

    and hope the result is "hello linux programing" but scheduling algorithm (such as: round robin, time sharing, shortest job first), the result maybe: "hellinuxprogrlloamming"
    -> need to locking 

## File locking and file control

+ Race condition

    For example: we have 4 process, and only processes with key can run. But when that process releases the key, the remaining 3 processes compete for the key. And we can fix it by flock() or fcntl(), declare as follows to use
    ```c
    #include<fcntl.h>
    #include<unistd.h>
    ```

+ flock(): file lock

    Lock the entire file and the lock will be automatically released when the file is closed or the process is terminated. Now, systems have more memory so flock() is often  used than fcntl()
    ```c
    int flock (int fd, int operation);
    ```
    with:
  
        + fd: file descriptor
        + operation: 
            + LOCK_SH: shared lock (multiple process can read)
            + LOCK_EX: exclusive lock (only 1 process can write)
            + LOCK_UN: unlock
            + add LOCK_NB: non-blocking (no wait if locked)
    For example:
    
    ```c
    int fd = open("test.txt", O_RDWR);  
    if( flock(fd, LOCK_EX) == -1){
        perror("flock");
        exit(1);
    }
    flock(fd, LOCK_UN);  //release the lock
    close(fd);
    ```
+ fcntl(): file control

    More complex but more powerful than flock, supports partial file locking, not bound by file descriptor - process can set many different types of locks. Process must respect locks voluntarily
    ```c
    int fcntl(int fd, int cmd, struct flock *lock);
    ```
    with: 

        + cmd:
            + F_SETLK: lock non-blocking
            + F_SETLKW: lock blocking( wait until can lock)
            + F_GETLK: check if lock can be set
        + struct flock {
            short l_type;  //F_RDLCK(multiple processes reading at the same time, none writing), F_WRLCK(only 1 process can write, no simultaneous reading/writing), F_UNLCK(unlock, free up memory)
            short l_whence; //SEEK_SET, SEEK_CUR, SEEK_END
            off_t l_start;  //starting position
            off_t l_len;    //length of the part to be locked
            pid_t l_pid;    //PID of the locking process (used with F_GETLK)
        };
        
    for example:
    ```c
    int fd = open("test.txt", O_RDWR);
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;     //lock till end of file
    if( fcntl(fd, F_SETLKW, &lock) == -1){
        perror ("fcntl");
        exit(1);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    close(fd);
    ```

## struct stat 
is a structure in C to store information about files in linux. Using:
```c
#include<sys/stat.h>
```

Some components in struct stat:
+ st_mode: Contains information about file types and access rights
+ st_size: Size of file (bytes)
+ st_mtime: Last file modification time
+ st_ctime: Last metadata change time
+ st_atime: Last file access time
+ st_uid/st_gid: ID of the user/group that owns the file
+ st_ino: Number of inode (each file has its own inode)
+ st_nlink: Number of links to the file

File type checking macros:
+ S_ISREG(info.st_mode): regular file
+ S_ISDIR(info.st_mode): directories
+ S_ISLNK(info.st_mode): symbolic link
+ S_ISCHR(info.st_mode): character device
+ S_ISBLK(info.st_mode): block device
+ S_ISFIFO(info.st_mode): fifo
+ S_ISSOCK(info.st_mode): socket

# Assignment of Linux File System

Hw1: Write a program that opens any file and uses the O_APPEND flag, then seeks to the beginning of the file and writes some data to the file. Where in the file will the data appear and why?

Look at the text before you run this program:
```bash
make beforehw1
``` 
Run program with this command:
```bash
make allhw1
```
The text which inserting always fall down to the last of older. When opening a file with the O_APPEND flag, any write() calls will automatically be redirected to write to the last file, regardless of where the file pointer is.

Hw3:
Follow some below step:
Want to write data to a file, command:
```bash
make writehw3 FILE=yourfile.txt NUM=yournumber DATA=yourdata
```
Example:
```bash
make writehw3 FILE=demo.txt NUM=5 DATA=quynchana
```
If file isn't exist, the program will create it!
If you command without full arguments:
```bash
make writehw3
```
it will be run with file is "./text/data.txt", num =5 and data is "hello" (const)
Want to read a file (not empty data file or not exist), command:
```bash
make readhw3 FILE=yourfile.txt NUM=yournumber
```
Example:
```bash
make readhw3 FILE=demo.txt NUM=3
```
if you only command this:
```bash
make readhw3
```
this program will read 5 data in "./text/data.txt"

Hw4: Create any file and write a piece of data to that file. Write the following information to the screen: File type, file name, last file modification time, size. - Use struct stat

This program will make "./text/checkvar.txt" and plain text is "Goodluck hungle".
We can see some info including: name, size, last edited and style by this command:
```bash
make allhw4
```

After run all hw, we should clean that:
```bash
make clean
```


