

#ifdef __MINGW32__
#include <winsock.h>
#include <windows.h>
#else
#include <sys/socket.h> // for socket(), ...
#include <netinet/in.h> // for sockaddr_in
#include <netdb.h> // for gethostbyname(), ...
#endif
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for write and closing socket

#define SERVER_HOSTNAME "127.0.0.1"
#define SERVER_PORT 22222

#define MESSAGE "Hello From Client!"

////////////// error checking macro

#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

static int s_return_code = 0;

#define CHECK_ERROR(condition, ...) \
	if (condition) { \
		char my_buffer[BUFFER_SIZE] = ""; \
		snprintf(my_buffer, BUFFER_SIZE, __VA_ARGS__); \
		fprintf(stderr, my_buffer); \
		s_return_code = 1; \
		goto cleanup; \
	} \


int main() {
#ifdef __MINGW32__
	WORD versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);
	printf("Windows version.\n");
#else
	printf("Posix version.\n");
#endif
	int client_socket = 0;
	CHECK_ERROR((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0, "ERROR opening socket");

	// retrieve host by name
	char *hostname = SERVER_HOSTNAME;
	struct hostent *server;
	CHECK_ERROR((server = gethostbyname(hostname)) < 0, "ERROR, no such host as %s\n", hostname);

	struct    sockaddr_in servaddr;
	// set all the fields to 0
	memset(&servaddr, 0, sizeof(servaddr));
	// set some fields to the desired values
    servaddr.sin_family      = AF_INET;
	memmove((void *)&servaddr.sin_addr.s_addr, (void *)server->h_addr, server->h_length);
    servaddr.sin_port        = htons(SERVER_PORT);

	CHECK_ERROR(connect(client_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0, "ERROR connecting");

    int ret = 0;
	ret = write(client_socket, MESSAGE, strlen(MESSAGE));
	CHECK_ERROR(ret < 0, "Error while writing the message on the socket file descriptor");

	while (TRUE) {
		int size = BUFFER_SIZE;
		int qty_read = 0;
		char buffer[BUFFER_SIZE] = "";
		CHECK_ERROR((qty_read = read(client_socket, buffer, size - 1)) < 0, "Error while reading the socket content.\n");
		buffer[BUFFER_SIZE - 1] = '\0';
		printf("Socket content: %s\n", buffer);
		if (qty_read != size - 1) {
			// we suppose it's because the end of stream
			break;
		}
	}

	CHECK_ERROR(close(client_socket) < 0, "Error while closing the socket");
cleanup:
	return s_return_code;
}
