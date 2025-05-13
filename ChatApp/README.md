The structure of project:
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
những vấn đề cần giải quyết:
7. send <connection id> <message>: Send a message to a peer
Mục tiêu:
Gửi tin nhắn văn bản đến một kết nối cụ thể.

Các yêu cầu:
Tin nhắn không quá 100 ký tự.
Hiển thị thông báo thành công khi gửi tin nhắn.
Khi nhận tin nhắn:
Hiển thị IP và cổng của người gửi.
Hiển thị nội dung tin nhắn.


