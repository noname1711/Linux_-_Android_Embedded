#ifndef MES_H
#define MES_H

typedef enum {
    RECV_OK,          // Received a message successfully
    RECV_DISCONNECT,  // Peer disconnected
    RECV_EXIT         // Peer send EXIT
} RecvStatus;

RecvStatus receive_message(int sender_fd, char* output_buffer, int buffer_size);
void send_message(int connection_id, const char* message);

#endif
