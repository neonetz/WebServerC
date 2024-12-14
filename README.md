# WebServerC

![WebServerC](https://img.shields.io/badge/Language-C-blue.svg) ![Status](https://img.shields.io/badge/Status-Development-yellow.svg) 

## Project Description
**WebServerC** is a simple project to build a web server using the C programming language. This server is designed to efficiently handle HTTP requests and provide appropriate responses. It is ideal for beginners who want to learn:

- 🌐 Network programming using sockets.
- 🧵 Process management with fork.
- 📡 Signal handling in operating systems.

## Key Features

- **HTTP Support:** Handles HTTP GET and POST requests.
- **Request Size Limit:** ❌ Rejects oversized requests with **413 Payload Too Large** status.
- **Route Management:** 📂 Directs requests to static files or HTML pages.
- **Parallel Processing:** ♻️ Utilizes child processes to serve multiple clients simultaneously.
- **Logging:** 📝 Logs requests and fork operations.

---

## Project Structure

```plaintext
WebServerC/
├── README.md
├── src/
│   ├── ClientHandler.c
│   ├── HTTP_Server.c
│   ├── Logger.c
│   ├── main.c
│   ├── Response.c
│   ├── Routes.c
│   ├── Server.c
│   └── SignalHandler.c
│
├── testing/
│   ├── test-buffer-limit.sh
│   └── test-buffer-overflow.sh
│
└── include/
    ├── ClientHandler.h
    ├── HTTP_Server.h
    ├── Logger.h
    ├── Response.h
    ├── Routes.h
    ├── Server.h
    └── SignalHandler.h
```

---

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/neonetz/WebServerC.git
cd WebServerC
```

### 2. Compile the Code

Use `make` to compile all source code:

```bash
make
```

### 3. Run the Server

After successful compilation, start the server with the following command:

```bash
./server.o
```

🚀 The server will run on **port 8080** by default.

---

## Usage

Once the server is running, you can send HTTP requests using tools like `curl` or a web browser. Example:

```bash
curl http://localhost:8080
```

### Testing Request Size Limit

To test the request size limit, execute the following script:

```bash
bash testing/test-buffer-limit.sh
```

### Testing Buffer Overflow

To test potential buffer overflow, use the following script:

```bash
bash testing/test-buffer-overflow.sh
```

---
### Testing HTTP Methods

To test different HTTP methods, use the following scripts:

- **GET Request:**
  ```bash
  bash testing/test-get.sh
  ```

- **POST Request:**
  ```bash
  bash testing/test-post.sh
  ```

- **PUT Request:**
  ```bash
  bash testing/test-put.sh
  ```

- **PATCH Request:**
  ```bash
  bash testing/test-patch.sh
  ```

- **DELETE Request:**
  ```bash
  bash testing/test-delete.sh
  ```

---

## Key Functions Explanation

### `start_server(int port)`
- ⚙️ Initializes the server and starts listening for requests on a specified port.
- 🛠️ Sets up routes and handles client connections.

### `handle_client(int client_socket, struct Route *route)`
- 📞 Handles communication with the client.
- ✅ Reads messages, validates requests, and sends appropriate responses.
- 🚫 Returns **413** status if the request size exceeds the limit.
- 🗂️ Handles different HTTP methods (GET, POST, PUT, DELETE, PATCH).

### `init_server(HTTP_Server *http_server, int port)`
- 🖧 Initializes the server socket, binds it to the port, and prepares the server to listen for incoming connections.

### `sigchld_handler(int signo)`
- 🧹 Handles the SIGCHLD signal to clean up finished child processes.
- 🛡️ Prevents zombie processes.

---

## Contributors

| Name                       | ID Number  |
|----------------------------|------------|
| Muhammad Adhyaksa Fadillah | 231524051  |
| Ais Laksana                | 231524035  |

---

## Contact

If you have any questions or suggestions, feel free to contact us via email or the development platform.

---

### **Happy Coding!** 🚀

