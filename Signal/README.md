Hw1: 
By default, when Ctrl+C is pressed in the terminal, the system sends a SIGINT signal to the running process, asking it to terminate.

If you ignore this signal, the program will disable the default behavior, which is not to terminate when receiving the SIGINT signal.

Result: Pressing Ctrl+C has no effect, the program continues to run normally as if nothing happened.

In this program, follow the below command to see exit program after Ctrl+C 3 times instead of just one:
```bash
make allhw1
```
Hw2:
If alarm(1) is not called again in the handler, the program will only receive the SIGALRM signal exactly 1 time.

Because alarm(1) only sets a timer once. When the time expires (after 1 second), SIGALRM is sent to the process.

If alarm(1) is not called again in the handler, the program does not set the next alarm, and therefore no more SIGALRM signals will be sent.

Result: the program will print "Timer: 1 seconds" and stop counting forever.

In this program, exiting after 10 seconds, command to see it:
```bash
make allhw2
```
Hw3:
Although the function name is kill(), it is actually used to send any signal (not just to "kill" a process) to a process via PID (Process ID). 

Without using kill(), the parent process has no direct way to signal the child process.

In this program, we will see parent process send SIGUSR1 signal to child and stopping program after 5 times, using command:
```bash
make allhw3
```
Hw4:
If the SIGTSTP signal is not handled, when the user presses Ctrl+Z, the process will be suspended and moved to the background. This means the process is suspended and no longer running.
The shell (bash) will display a message like:
```bash
[9]+  Stopped
```
In this program,we ignore SIGTSTP (Ctrl + Z). First, build all file:
```bash
make allhw4
```
And run:
```bash
./bin/hw4
```
And Ctrl + C to interrupt this program

Hw5:
In this program, we can get signal and allow to enter from keyboard. Command to start:
```bash
make allhw5
```
Now, we can fill it but Ctrl + C(SIGINT) can't interrupt program. We can use SIGTERM to exit. Attention on PID, for example, your PID is 
```bash
PID: 18422
``` 
So we can exit by command in different terminal:
```bash
kill 18422
```
This command is mean that SIGTERM.

After both of them, we should be clean all:
```bash
make clean
```
