#ifndef MES_H
#define MES_H

void send_message(int connection_id, const char* message);
void receive_message(int sender_fd);

#endif
