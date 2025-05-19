# What is Makefile ?

Makefile is a script that contains the flowing information:
+ Structure of a project (file, dependency)
+ Command lines are used to create and destroy file
  
The make program will read the contents of the Makefile and execute it.

Makefile's structure (rules): 
```text
Target : dependences
    Action
```
Structure of Makefile basic:
```Makefile
# target in PHONY is a virtual target, containing dependencies that help make command to always run those dependencies, even if the directory name is duplicated.
.PHONY: all clean setup create link install 

# directory variables help organize directories more clearly and easily maintain them.
CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

# compiled variable
CC := gcc
CFLAGS := -g -Wall -Wextra -I$(INC_DIR)

# main target
setup:
    mkdir neccessary directory
create_obj: setup
    command to create object file
create_lib: setup
    command to create libarary
link_lib: create_lib
    command link
all: link_lib
clean:
    rm -rf all dir which create in setup target
check:
    some check including testcase, memory leak...
```

Create Makefile:
```bash
touch Makefile
```
Command "touch" will create file with empty information

Run: 
```bash
make target
```

Commonly used targets are usually named according to the following standard:
+ all: produce the final output
+ info: information of program
+ clean: cancel
+ install: install necessary tools
+ tar: compress files
+ test: test the task

Note:
+ Unused resources must be deleted:
```bash
rm -rf file
```
+ run C/C++ program:
    + create C program executable file:
  ```bash
  gcc test.c -o test
  ```
    + create Cpp program executable file:
  ```bash
  g++ test.cpp -o test
  ```
    + run executable file:
  ```bash
  ./test
  ```
+ run Python/Java program:
    + Python:
  ```bash
  python3 test.py
  ```
    + Java:
        + First compile:
        ```bash
        javac test.java
        ```
        + Run:
        ```bash
        java test
        ```

# What is Vim ?

Vim is a highly configurable text editor built to make creating and changing any kind of text very efficient. 

Open file with Vim:
```bash
vim file
```
Basic commands in Vim:
+ Insert mode: Press "i" to start
+ Command mode: Press "esc" to return to command line
+ Save and exit: Type ":wq" and press "enter"
+ Undo: Press "u" to undo the last change
+ Search: Type "/word" to search for a "word"

# What is Visual Studio Code ?

Visual Studio Code (VS Code) has become one of the most popular code editors for embedded Linux development due to its versatility, extensive extension ecosystem, and excellent cross-platform support. 

Open file with VSCode:
```bash
code file
```

# What is header file ?

Header files (.h or .hpp) serve as interfaces for source code files, containing declarations that can be shared across multiple files. Here's their typical structure:
+ Header Guard (esential):
```c
#ifndef FILENAME_H  // Typically uppercase with _H suffix
#define FILENAME_H
// ... contents ...
#endif // FILENAME_H
```
+ Includes (dependencies):
```c
#include <standard_library_headers>  // Angle brackets for system headers
#include "local_headers.h"          // Quotes for project headers
```
+ Constants:
```c
#define MAX_SIZE 100       // Macro constants
const int BUFFER_SIZE = 256; // C++ style constants
```
+ Type definitions:
```c
typedef struct {
    int x, y;
} Point;  // C style

// C++ style
enum class Color { RED, GREEN, BLUE }; 
```
+ Function declarations:
```c
// C style
int calculate_sum(int a, int b);

// C++ may include default arguments
void print_message(const char* msg, bool newline = true);
```
+ Class/Struct Definitions (C++)
```cpp
class MyClass {
public:
    MyClass();              // Constructor
    ~MyClass();             // Destructor
    void publicMethod();     
    
private:
    int privateVar;
};
```
+ Templates (C++)
```cpp
template <typename T>
T max(T a, T b);
```
+ Inline Functions (When Appropriate):
```cpp
inline int square(int x) { return x * x; }
```
+ Namespace (C++):
```cpp
namespace MyProject {
    // Declarations here
}
```
+ Documentation:
```cpp
/**
 * @brief Brief description
 * @param param1 Description of first parameter
 * @return Description of return value
 */
int documented_function(int param1);
```

# Stages of compiling a C program

+ Pre-processing: 
    + remove comments
    + expand macros
    + expand include files
    + compiling conditional statements
    + the rusult obtained after this step is an ".i" file
+ Compilation: satge of translating high level language to asm. At the stage, the source code will continue to perform compilation from the ".i" file obtained in the previous step into a ".s" (assembly) file
+ Assembly:
    + the file ".s" in the previous stage continues to be used for this stage by using assembler
    + the output we get is a file ".o". This is a file containing machine language level instructions
+ Linking: 
    + each ".o" file obtained at the assembly stage is part of the program 
    + at the linking stage will link them to get a complete executable file

# Static Library and Shared Library

A library is a collection of precompiled code that can be reused in a program.

Divided into 2 types:
+ Static lib (.a)
+ Shared lib (.so)

Comparison: 
```text
| Characteristic       | Static Library                  | Shared Library                |
|----------------------|---------------------------------|-------------------------------|
| **Linking Time**     | Compile time                    | Runtime                       |
| **File Size**        | Larger executable               | Smaller executable            |
| **Memory Usage**     | Each program has its own copy   | Shared between programs       |
| **Updates**          | Requires recompile              | Can replace .so file          |
| **Distribution**     | Self-contained                  | Requires library install      |
| **Loading Speed**    | Faster startup                  | Slightly slower startup       |
| **Versioning**       | No versioning                   | Supports versioning           |
```
When to use each:
+ use static lib:
    + You're building a standalone application
    + You don't want deployment dependencies
    + Performance is critical
    + You're working on embedded systems with limited resources
+ use shared lib:
    + Multiple applications will use the same code
    + You want to save disk and memory space
    + You need to update libraries without recompiling
    + You're developing system-level software

# The structure of basic project

+ inc: including header file (.h)
+ src: including source code
+ obj: including object file (.o)
+ bin: including executable file
+ lib: including static lib or shared lib
+ Makefile
+ README.md

# Assignment: Build static library (.a) and shared library (.so) and using Makefile to build all steps

Build everything by this command:
```bash
make all
```
And if you want to use static lib, follow this command:
```bash
bin/use-static-library
```
or you want to use shared lib, follow this command:
```bash
bin/use-shared-library
```
Both of them, this program can help you to calculate the sqrt of a number (double).
After all action, you should be clean that:
```bash
make/clean
```
