# Example of unnamed and named semaphore 

+ Unnamed Semaphore: 

Write a program that uses an unnamed semaphore to synchronize two threads in the same process. One thread acts as a producer, the other as a consumer. Use a semaphore to ensure that the consumer only gets the data after the producer has finished creating it.
```bash
make run_unnamed
```

+ Named Semaphore:

Write two independent programs (Program A and Program B) that use named semaphore to synchronize access to a shared resource (e.g. file). Program A writes data to the file, Program B reads data from the file. Make sure Program B reads only after Program A has finished writing.
```bash
make run_named
```

After all, should be clean that:
```bash
make clean
```