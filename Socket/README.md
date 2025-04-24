Run this program, starting by command:
```bash
make all PROTO=your_proto TYPE=your_type
```
with your_proto may be one of the following options: ipv4, ipv6 or unix, and your type may be datagram or stream. Example:
```bash
make all PROTO=ipv4 TYPE=stream
```
Create your server with command:
```bash
make server PROTO=your_proto TYPE=your_type PORT=your_port
```
Your_port can be anything, suggest 8080 but port number must be between 1024 and 65535. Example:
```bash
make server PROTO=ipv4 TYPE=stream PORT=8080
```

Open a new bash, create your client to listen and print message from server by this command:
```bash
make client PROTO=your_proto TYPE=your_type PORT=your_port  HOST=your_host
```
Your host can be anything, recommend 127.0.0.1. Example:
```bash
make client PROTO=ipv4 TYPE=stream PORT=8080  HOST=127.0.0.1
```
Note: client parameters must match server.

If the parameters: proto, type, port, host are not assigned, the system will default to assigning the following values: PROTO is ipv4, TYPE is stream, PORT is 8080, HOST is 127.0.0.1.

But if you using Unix(PROTO), you don't care about port and host, so you can command without PORT and HOST.

After all, should be clean that by command:
```bash
make clean
```

