#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


char *handle_http_req(const char *src){
	// Check if the request method is GET (for simplicity, we're only handling GET requests)
	if(strncmp(src, "GET", 3) == 0) {
	// Prepare the HTTP response 
		char *response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 46\r\n"
			"\r\n"
			"<html><body><h1>Hello, World!</h1></body></html>"; 
	// Allocate memory for the response (ensure to free this in the calling function to avoid memory leaks)
		char *resp = malloc(strlen(response) + 1); // +1 for the null terminator
		if (resp != NULL) {
			strcpy(resp, response);
		}
		return resp;
	}
	else {
	// For non-get requests
		char *response =
            		"HTTP/1.1 405 Method Not Allowed\r\n"
            		"Content-Type: text/html\r\n"
            		"Content-Length: 40\r\n"
            		"\r\n"
            		"<html><body><h1>Method Not Allowed</h1></body></html>";
        	char *resp = malloc(strlen(response) + 1); // +1 for the null terminator
        	if (resp != NULL) {
            		strcpy(resp, response);
        	}
        	return resp;
    	}
}

int main () {

	WSADATA wsaData;
	int iResult;
	// initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0){
		printf("WSAStartup failed: %d\n", iResult);
	}
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

	//struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
	    printf("getaddrinfo failed: %d\n", iResult);
	    WSACleanup();
	    return 1;
	}
	// Create a SOCKET for the server to listen for client connections. 
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET){
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	else {
		printf("socket created successfully\n");
	}
	// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else {
    	printf("Server listening on port %s\n", DEFAULT_PORT);
    }

    while (1) {
	    // Accept a client socket 
	    ClientSocket = accept(ListenSocket, NULL, NULL);
	    if(ClientSocket == INVALID_SOCKET) {
	    	printf("accept failed with error: %d\n", WSAGetLastError());
	    	closesocket(ListenSocket);
	    	WSACleanup();
	    	return 1;
	    }
	    // Receive until the peer shuts down the connection
        iResult = recv(ClientSocket, recvbuf, recvbuflen - 1, 0);

        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            // Null-terminate the received data
    		recvbuf[iResult] = '\0';  // Make sure recvbuflen has space for this
    		//Now, recvbuf contains a null-terminated string 
    		printf("Received message: %s\n", recvbuf);
		
			// Handle the HTTP request
    		char *response = handle_http_req(recvbuf); // This should parse the request and generate the appropriate response

        	// Echo the buffer back to the sender
            iSendResult = send( ClientSocket, response, strlen(response), 0 );
            if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			} else {
			printf("Response sent: %d bytes\n", iSendResult);
			}
			// Free the response memory if it's no longer needed
			free(response);
		}
	}
		return 0;
}
