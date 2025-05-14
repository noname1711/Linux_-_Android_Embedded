# The structure of project:
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
|-- src/                    # source code
    |-- lib/                # library
        |-- cli.c
        |-- connection.c
        |-- mySocket.c
        |-- mes.c
        |-- myLog.c
    |-- peer/               # peers
        |-- peer.c
|
|-- obj/                    # including object file (.o)                 
|-- bin/                    # including executable file
|-- Makefile                
|-- README.md
```

The project uses IPv4 stream socket in peer-to-peer model.

Peer-to-peer (P2P) is a decentralized network model where devices (referred to as "peers") communicate and share resources directly with each other without relying on a centralized server or authority. Each peer functions both as a client and a server, meaning it can request and provide resources.

Here, the maximum number of connections is 10 connections.

In cli.c, this program can show menu, that will show auto when command 
```bash
make run PORT=your_port
```
and show help including the list of command by command 
```bash
help
```

And connection.c, the structure will be:
```text
|-- add_connection(int socket_fd, struct sockaddr_in addr, int listening_port):
    this function will insert new connection to array connections. Rule:
        Check if the number of connections has reached the MAX_CONNECTIONS limit,log an error and return.
        Assign a new ID that does not match the ID of existing connections.
        Store the peer's socket information, address and listening port.
        Increase conn_count.
        Log messages.
|
|-- get_peer_listen_port(int socket_fd): returns the port the peer is listening on from a known socket_fd connection or returns -1 if not found.
|
|-- remove_connection(int id): Remove the connection with the corresponding id from the connections array. 
    Find the connection by ID.
    Move the elements after it up to overwrite the deleted element.
    Decrease conn_count.
    Log a message or error if not found.
|
|-- list_connections(): Print out the list of current connections in a table format including:ID, IP of peer, port to connect to (assigned by kernel), port that peer is listening to (sent to peer)
|
|-- get_socket_by_id(int id): Get socket_fd from connection id or returns -1 if no connection matches.
|
|-- get_id_by_socket(int socket fd): Get the connection id from socket_fd.
|
|-- cleanup_connections(): Close all connections and reset conn_count to 0.
|
|-- get_connection_index_by_id(int id): Returns the index in the connections array corresponding to the id or returns -1 if not found.
```

In mes.c, the structure will be:
```text
|-- send_message(int connection_id, const char* message): Send a message from the current machine to the peer with the corresponding connection_id.
    Steps:
    1. Get socket from connection_id using get_socket_by_id.
    2. Check for validity (socket exists).
    3. Get peer's IP address using getpeername.
    4. Use get_peer_listen_port to get the port the peer is listening on.
    5. Check message length does not exceed MAX_MESSAGE_LENGTH.
    6. Send message using send().
    7. Log message sending.
|
|-- RecvStatus receive_message(int sender_fd, char* output_buffer, int buffer_size): Receive messages from a peer via sender_fd (sender's socket).
    Steps:
    1.Receive data from sender_fd using recv().
    If len <= 0:
        + len == 0: Peer has closed the connection → return RECV_DISCONNECT.
        + len < 0: There was an error while receiving → also return RECV_DISCONNECT.
    2. End the string with null (\0) to be safe.
    3. If "EXIT" is received, return RECV_EXIT.
    4. Get the peer's IP and port using getpeername and get_peer_listen_port.
    5. If the message is longer than MAX_MESSAGE_LENGTH: truncate and log the error.
    6. Log the message content from the peer.
    7. Return RECV_OK.
```

In mySocket.c, the structure will be:
```text
|-- setup_listener_socket(int port):
    Create a server socket, attached to port.
    Allow other peers to connect to you.
    Returns the socket used to listen for connections, or -1 if there is an error.
|
|-- get_my_ip():
    Gets your machine's real IP address (to display to others).
    Returns an IP string in the form "192.168.x.x", because 192.168.x.x is part of the private IP address range, used within a LAN (home, school, company...).
|
|-- create_connection(const char* ip, int port):
    Connects to another peer via the IP and port they provide.
    Returns the socket used to send/receive data, or -1 if there is an error.
```

In myLog.c, the structure will be:
```text
|-- log_info(const char* fmt, ...): Print info with the prefix [INFO].
|
|-- log_error(const char* fmt, ...): Print error with the prefix [ERROR].
```

And we will use all library to main program (peer.c):

Allows a peer to:

    + Listen for incoming connections from other peers.
    + Send/receive messages to/from other peers.
    + Manually connect to another peer.
    + Manage the connection list (list, terminate).
    + Exit the application safely.

Overall Structure:

    + Initialization

        Receive the port from the command line.
        Open a socket to listen for incoming connections.
        Display the command-line interface (CLI) menu.
        Set up select() to monitor: STDIN, the listener socket, and peer sockets.

    + Main Loop (while (1))
        Process events from:

        a. User Input (STDIN)

            help → Show instructions.
            myip / myport → Display IP or port.
            connect <ip> <port> → Connect to another peer.
            list → List current connections.
            terminate <id> → Close a connection by ID.
            send <id> <msg> → Send a message to a peer by ID.
            exit → Send “EXIT” to all peers and exit the program.

        b. New Incoming Connection (Listener Socket)

            Accept a connection from another peer.
            Receive the listening port information of the other peer.
            Add it to the connection list.

        c. Messages from Other Peers

            Receive a message (or an EXIT command).
            Display it or handle disconnection if the peer exits.

And finish all program, we should clean that:
```bash
make clean
```