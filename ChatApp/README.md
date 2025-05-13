The structure of project 1:
```text
Chat Application/
|
|-- inc/                    # including header file
    |-- cli.h               # CLI command handling functions
    |-- connection.h        # Connection management functions
    |-- mySocket.h          # Utility functions
    |-- mes.h               # Message send/receive interface
    |-- myLog.h             # Logging utilities
|
|-- src/
    |-- lib/
        |-- cli.c
        |-- connection.c
        |-- mySocket.c
        |-- mes.c
        |-- myLog.c
    |-- client/
        |-- client.c
    |-- server/
        |-- server.c
|
|-- obj/
|-- lib/
|-- bin/
|-- Makefile
|-- README.md
```


