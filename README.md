# IRC Server

### Overview
This project involves creating your own IRC (Internet Relay Chat) server. The server allows real-time messaging between clients, enabling both private direct messages and public group channel interactions. You will use an actual IRC client to connect to your server and test its functionality.

### What is IRC?
Internet Relay Chat (IRC) is a text-based communication protocol on the Internet that facilitates real-time messaging. It allows users to send direct messages or join public or private group channels. 

### How it Works
- IRC clients connect to IRC servers to join channels and exchange messages.
- Multiple IRC servers can be connected together to form an IRC network.
- Each server operates according to standardized protocols, allowing for seamless communication across the network.

### Features
- **Real-time messaging**: Send direct messages or join group channels.
- **Public and private communication**: Users can join public channels or send private messages to other users.

### To compile the program:
- `make` To start the Server

```./ircserv <port> <password>```
port: The port number on which your IRC server will be listening to for incoming IRC connections.
password: The connection password. It will be needed by any IRC client that tries to connect to your server. To connect to the server, you can use:
```nc <IP ADDRESS> <PORT>```
IP ADDRESS: Host IP address.
PORT: The PORT that the server listening on.

### commands specific to channel
operators:
∗ KICK - Eject a client from the channel
∗ INVITE - Invite a client to a channel
∗ TOPIC - Change or view the channel topic
∗ MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel
operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel
