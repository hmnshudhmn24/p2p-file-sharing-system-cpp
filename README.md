# P2P File Sharing System

A simple decentralized peer-to-peer (P2P) file sharing system using C++ and socket programming.

## Features
- Peer-to-peer file sharing without a centralized server
- Supports file requests and downloads
- Multi-threaded server to handle multiple connections
- Simple command-line interface

## Requirements
- A Linux-based system (or WSL on Windows)
- g++ compiler

## Installation & Usage

### 1. Compile the program
```sh
 g++ p2p_file_sharing.cpp -o p2p -pthread
```

### 2. Running as Server
```sh
 ./p2p
```
- Enter a file name and path to share.
- The server listens for incoming file requests on `PORT 8080`.

### 3. Running as Client
```sh
 ./p2p
```
- Choose the option to request a file.
- Enter the server's IP and the filename.
- The file is downloaded and saved as `downloaded_filename`.

## How It Works
- **Server Mode**: Listens for incoming connections, shares files.
- **Client Mode**: Connects to a peer server and requests a file.

## Code Overview
- **Server Mode**
  - Accepts connections
  - Responds to file requests
  - Serves files if available
- **Client Mode**
  - Sends a GET request for a file
  - Receives and saves the file

## Future Enhancements
- Implement better peer discovery
- Add secure file transfer with encryption
- Improve file integrity checks
