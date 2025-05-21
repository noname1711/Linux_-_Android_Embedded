We will build POSIX and systemV shared memory 

Firstly, build POSIX and sent message from writer to reader:

+ Step 1: open new terminal and we will make writer first:
    ```bash
    make posix-writer
    ```

+ Step 2: open a new terminal and we will make reader here:
    ```bash
    make posix-reader
    ```

Secondary, build systemV and sent message from witer to reader:

+ Step 1: open new terminal and we will make writer first:
    ```bash
    make systemv-writer
    ```

+ Step 2: open new terminal and we will make reader here:
    ```bash
    make systemv-reader
    ```

After all, should be clean up:
```bash
make clean
```