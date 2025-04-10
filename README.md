# KoolServer

## Server Side (Test):
1. Binds and Listens to a socket
2. Receives a string from client and reverses it
3. Returns the string to the client after the value is reversed
4. Uses Error handling for easy debug and troubleshooting
5. Run Command: ./<executable> <port number>

## Client Side (Test):
1. Creates, connects, and sends a string over a socket
2. Receives a string over the socket
3. Prints out the received string
4. Uses Error handling for easy debug and troubelshooting
5. Run Command: ./<executable> <ip address> <port number>
6. Enter String that needs to be reversed

## HTTP WebServer:
1. Accepts the TCP connection
2. Parses HTTP GET requests
3. Sends an "HELLO WORLD" HTTP response file (to be expanded)
4. Handles Single Client per iteration (to be expanded w/ RTOS functionalities)
5. Run Command: cmake .. -> cmake -> --build . ->./build/webserver to run executable software
6. Run http://localhost:8080/hello.html
