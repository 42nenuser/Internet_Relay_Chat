# ft_irc

## Introduction
ft_irc is a project focused on creating an IRC (Internet Relay Chat) server in C++ 98. This server will allow real-time text-based communication between clients over the internet using standard IRC protocols. The goal of this project is to gain a deeper understanding of network programming, socket communication, and the principles of client-server architecture.

## Features
ft_irc includes the following functionalities:
- **Client Connections**:
  - Users can connect to the server using an IRC client.
  - Supports user authentication with a password-protected connection.
- **Channel Management**:
  - Users can create and join channels.
  - Channel operators have administrative privileges.
- **Messaging System**:
  - Users can send and receive messages in channels or privately.
  - Messages are forwarded to all clients in the channel.
- **Operator Commands**:
  - `KICK`: Eject a client from a channel.
  - `INVITE`: Invite a client to a private channel.
  - `TOPIC`: Change or view the channel topic.
  - `MODE`: Set channel modes (e.g., invite-only, topic restrictions, user limits).
- **Non-blocking I/O**:
  - The server handles multiple clients using a single `poll()` or an equivalent system call.
  - Ensures efficient network communication.
- **Protocol Compliance**:
  - Follows standard IRC behavior for authentication, messaging, and channel management.

## Bonus Features
If the mandatory part is fully implemented, additional features can be added:
- **File Transfer**: Enable users to send and receive files.
- **Bot Integration**: Add a bot to automate common tasks or provide responses.

## Requirements
- The project must be written in **C++ 98**.
- Use only standard C++ libraries and system calls (Boost and external libraries are forbidden).
- Handle multiple clients efficiently without forking.
- All I/O operations must be **non-blocking**.
- The server must use **TCP/IP** communication.
- Provide a Makefile for compilation with the flags `-Wall -Wextra -Werror`.

## Compilation
To compile ft_irc, use the provided Makefile:
```sh
make
```
To clean up object files:
```sh
make clean
```
To remove all compiled files:
```sh
make fclean
```
To recompile:
```sh
make re
```

## Usage
Run the server using:
```sh
./ircserv <port> <password>
```
- `<port>`: The port number on which the server will listen for connections.
- `<password>`: The password required for clients to connect.

To connect, use an IRC client and provide the server address and password.

## Learning Outcomes
By completing this project, you will gain experience in:
- Network programming using **sockets** and **TCP/IP**.
- Handling **multiple client connections** using polling mechanisms.
- Implementing **client-server communication protocols**.
- Managing **authentication and user roles** in a real-time messaging system.

## License
This project is for educational purposes and follows the requirements outlined in the ft_irc project guidelines.

---



