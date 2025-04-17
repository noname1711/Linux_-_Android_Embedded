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
change myapp by your name, see pts/0, it mean success. But we only check this in 10s after child process sleeping
After both of them, should be clean that:
```bash
make clean
```
