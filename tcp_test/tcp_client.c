

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

#define TRY(statement, condition, ...) \
	statement; \
	if (condition) { \
		fprintf(stderr, __VA_ARGS__); \
		result = 1; \
		errno = 0; \
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

	int client_socket = TRY(socket(AF_INET, SOCK_STREAM, 0),
		client_socket < 0,
		"ERROR opening socket (%d: %s)", errno, strerror(errno));

	// retrieve host by name
	char *hostname = SERVER_HOSTNAME;
	struct hostent *server = TRY(gethostbyname(hostname),
		server < 0,
		"ERROR while getting hostname %s. (%d: %s)\n", hostname, errno, strerror(errno));

	struct    sockaddr_in servaddr;
	// set all the fields to 0
	memset(&servaddr, 0, sizeof(servaddr));
	// set some fields to the desired values
	servaddr.sin_family      = AF_INET;
	memmove((void *)&servaddr.sin_addr.s_addr, (void *)server->h_addr, server->h_length);
	servaddr.sin_port        = htons(SERVER_PORT);

	result = TRY(connect(client_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)),
		result < 0,
		"ERROR connecting. (%d: %s)", errno, strerror(errno));

	printf("result=%d\n", result);
	printf("client_socket=%d\n", client_socket);

	result = TRY(write(client_socket, MESSAGE, strlen(MESSAGE) + 1),
		result < 0,
		"Error while writing the message on the socket file descriptor: %d (%s)", errno, strerror(errno));

	while (TRUE) {
		int size = BUFFER_SIZE;
		int qty_read = 0;
		char buffer[BUFFER_SIZE] = "";
		qty_read = TRY(read(client_socket, buffer, size - 1),
			qty_read < 0,
			"Error while reading the socket content.\n");
		buffer[BUFFER_SIZE - 1] = '\0';
		printf("Socket content: %s\n", buffer);
		if (qty_read != size - 1) {
			// we suppose it's because the end of stream
			break;
		}
	}


cleanup:
	if (client_socket) {
		close(client_socket);
	}
	return result;
}
