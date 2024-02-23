# Simple HTTP Server in C

This project contains a basic implementation of an HTTP server written in C using Winsock API, designed to run on Windows environments. For simlicity, It only handles GET requests and responds with a simple "Hello, World!" HTML page, for simplicity.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.
Prerequisites

To run this server, you will need:

- A Windows machine since the project uses Winsock API.
- A C compiler (like GCC or Clang) that supports C89 or later.
- Make sure your system has the Winsock library installed.

## Installing

Follow these steps to get your development environment running:

1. Clone the repository to your local machine.
```
git clone https://github.com/aterzgar/SimpleWebServer.git
```
2. Navigate to the project directory.
```
cd SimpleWebServer
```
3. Compile the code using your C compiler. 
```
gcc server.c -o httpserver -lws2_32
```
4. Run the server:
```
./server
```

The server will start listening on port 27015. You can test it by navigating to `http://localhost:27015` in your web browser. you can also set another port if you like.

## Usage

Once the server is running, it will listen for HTTP GET requests. It only responds to GET requests with a simple HTML page containing "Hello, World!" for now. Any other types of requests will receive a "405 Method Not Allowed" response.

To modify the response, edit the handle_http_req function in the server.c file.