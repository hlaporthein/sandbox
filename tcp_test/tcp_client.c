

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
#include <errno.h>
#include <unistd.h> // for write and closing socket

#define SERVER_HOSTNAME "127.0.0.1"
#define SERVER_PORT 22222

#define MESSAGE "Hello From Client!"

////////////// error checking macro

#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

#define TRY(condition, ...) \
	if (condition) { \
		char my_buffer[BUFFER_SIZE] = ""; \
		snprintf(my_buffer, BUFFER_SIZE, __VA_ARGS__); \
		fprintf(stderr, my_buffer); \
		result = 1; \
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
	int result = 0;
	
	int client_socket = 0;
	TRY((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0, "ERROR opening socket: %d (%s)", errno, strerror(errno));

	// retrieve host by name
	struct hostent *server;
	TRY((server = gethostbyname(SERVER_HOSTNAME)) < 0, "ERROR, no such host as %s: %d\n", SERVER_HOSTNAME, h_errno);

	struct    sockaddr_in servaddr;
	// set all the fields to 0
	memset(&servaddr, 0, sizeof(servaddr));
	// set some fields to the desired values
    servaddr.sin_family      = AF_INET;
	memmove((void *)&servaddr.sin_addr.s_addr, (void *)server->h_addr, server->h_length);
    servaddr.sin_port        = htons(SERVER_PORT);

	TRY(connect(client_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0, "ERROR connecting socket: %d (%s)", errno, strerror(errno));

    int ret = 0;
	ret = write(client_socket, MESSAGE, strlen(MESSAGE));
	TRY(ret < 0, "Error while writing the message on the socket file descriptor");

	while (TRUE) {
		int size = BUFFER_SIZE;
		int qty_read = 0;
		char buffer[BUFFER_SIZE] = "";
		TRY((qty_read = read(client_socket, buffer, size - 1)) < 0, "Error while reading the socket content.\n");
		buffer[BUFFER_SIZE - 1] = '\0';
		printf("Socket content: %s\n", buffer);
		if (qty_read != size - 1) {
			// we suppose it's because the end of stream
			break;
		}
	}

	TRY(close(client_socket) < 0, "Error while closing the socket");
cleanup:
	return result;
}
