# vRedisClone

vRedisClone is a minimal Redis-like key-value store, written completely in C. With a design philosophy centered on simplicity and performance, this project reimplements Redis concepts like custom hash tables and RESP (Redis Serialization Protocol) parsing to build foundational database operations and a client-server architecture from scratch.

## Features

### Custom Hash Table Implementation
- Dynamically resizable hash tables with a load factor threshold for efficient memory utilization.
- Supports basic CRUD (Create, Read, Update, Delete) operations on key-value pairs.
- Collision handling via linked lists within hash table slots.
- Implements both string and integer key support with on-the-fly type checks.

### Redis Serialization Protocol (RESP)
- Fully capable of parsing Redis requests encoded in the RESP protocol format.
- Implements error handling for invalid or malformed RESP inputs.
- Provides utilities to encode client-provided Redis commands into RESP format for communication.

### Command Lexicon
The server includes a built-in "lexicon" or dictionary of commands mimicking Redis:
- **SET**: Stores a value corresponding to a given key.
- **GET**: Retrieves the stored value for a key.
- Command parsing and execution are tightly integrated into the RESP parsing pipeline for efficiency.

### Client-Server Model
- The server and client communicate via TCP sockets.
- The client allows users to send RESP-formatted commands (`SET`, `GET`) to the server for processing.
- Server uses `epoll` for non-blocking I/O and efficient handling of concurrent client connections.

### Epoll-Based Event Loop
- Uses the `epoll` syscall to handle concurrent connections efficiently.
- Event-driven, non-blocking architecture ensures scalability for multiple simultaneous connections.

### Lightweight Design
- Entire project built from scratch, avoiding external dependencies.
- Modular and extensible header files for maintaining distinct functionalities:
  - `HTable.h` for hash table operations.
  - `Cmdlexicon.h` for managing the command lexicon.
  - `Resp.h` for RESP parsing and error handling.

### Socket Based Redis Client (`Respcli`)
- Enables users to directly interact with the server via the client program.
- Tokenizes user commands into RESP format and manages the complete interaction lifecycle.
- Provides responses from the server and displays error messages when applicable.

### Built with C
By leveraging C for this implementation, the project achieves:
- Fine-grained control over memory management.
- Resource optimization suitable for constrained environments.
- Pedagogical exploration of the core Redis mechanisms.

## Installation

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/tslime/vRedisClone.git
cd vRedisClone
```

Build the client and server applications using the provided shell script:

```bash
bash compile_file.sh
```

This will generate two executables:
- `Server`: The server application.
- `Respcli`: The Redis-like client application.

## Usage

### Start the Server
To start the server, simply execute the `Server` binary:

```bash
./Server
```

The server will bind to port `6379` (default Redis port) and wait for client requests.

### Connect Through the Client
Run the `Respcli` binary to interact with the server:

```bash
./Respcli
```

Supported commands for the client:
- **SET key value**: Store a value under the specified key.
- **GET key**: Retrieve the value associated with the key.

Example session:
```plaintext
Give me your command
SET myKey myValue
OK

Give me your command
GET myKey
myValue
```

## Supported Commands
- `SET`: Save data with a key and associated value.
- `GET`: Retrieve value stored with a specific key.
- Error reporting for malformed commands or unavailable keys.

---

## Disclaimer

Redis is a trademark of Redis Ltd. `vRedisClone` is not affiliated with, nor endorsed by, Redis Ltd.
