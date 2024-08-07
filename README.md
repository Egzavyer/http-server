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
├── wwwroot/         # Static HTML files
├── CMakeLists.txt   # CMake configuration
└── README.md        # Project description
```

## License

This project is licensed under the MIT License.
