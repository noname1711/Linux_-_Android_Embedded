#ifndef MES_H
#define MES_H

typedef enum {
    RECV_OK,          // Nhận tin nhắn thành công
    RECV_DISCONNECT,  // Peer ngắt kết nối
    RECV_EXIT         // Peer gửi lệnh EXIT
} RecvStatus;

RecvStatus receive_message(int sender_fd, char* output_buffer, int buffer_size);
void send_message(int connection_id, const char* message);
//void receive_message(int sender_fd);

#endif
