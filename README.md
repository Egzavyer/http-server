# HTTP Server

A simple HTTP server and client implementation in C++.

## Features
- Basic HTTP server
- Client-server communication
- Serves static HTML content

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Egzavyer/http-server.git
   ```
2. **Build the project:**
   ```bash
   cd http-server
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   mingw32-make
   ```

## Usage

- **Start the Server:**
  ```bash
  ServerExecutable.exe
  ```
  or
  
  ```bash
  ./ServerExecutable
  ```
  *Go to http://localhost:8080/*
- **Run the Client:**
  ```bash
  ClientExecutable.exe
  ```
  or
  
  ```bash
  ./ClientExecutable
  ```

## Project Structure

```
http-server/
├── client/          # Client-side code
├── common/          # Common utilities
├── server/          # Server-side code
├── public/         # Static HTML files
├── CMakeLists.txt   # CMake configuration
└── README.md        # Project description
```

## TODO
- ~~Put methods that can be made static in a util.cpp file~~
- ~~Put frequently reused headers in the common.h file or delete the common class~~


- Handle multiple connections using threads (connection.cpp)
- Make the parsing of POST request body more reusable and efficient (util.cpp)
- Handle response Content-Type header more efficiently (httpHandler.cpp)
- Handle bigger or incomplete requests (httpParser.cpp)
- Handle chunked Transfer-Encoding (httpParser.cpp)
- Handle GET request for favicon.ico (httpParser.cpp)


- Refactor for best practices
- Optimize
- Comment code
- Figure out what license project falls under and add it
- Testing
