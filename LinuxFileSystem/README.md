Hw1: 
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

Hw4:
This program will make "./text/checkvar.txt" and plain text is "Goodluck hungle".
We can see some info including: name, size, last edited and style by this command:
```bash
make allhw4
```

After run all hw, we should clean that:
```bash
make clean
```
