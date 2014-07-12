

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

<<<<<<< HEAD
#define TRY(statement, condition, ...) \
	statement; \
	if (condition) { \
		fprintf(stderr, __VA_ARGS__); \
		result = 1; \
		errno = 0; \
=======
#define TRY(condition, ...) \
	if (condition) { \
		char my_buffer[BUFFER_SIZE] = ""; \
		snprintf(my_buffer, BUFFER_SIZE, __VA_ARGS__); \
		fprintf(stderr, my_buffer); \
		result = 1; \
>>>>>>> 9dc42fcc64a4578aebee9e3c1c80a5892809b734
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
<<<<<<< HEAD

	int client_socket = TRY(socket(AF_INET, SOCK_STREAM, 0),
		client_socket < 0,
		"ERROR opening socket (%d: %s)", errno, strerror(errno));

	// retrieve host by name
	char *hostname = SERVER_HOSTNAME;
	struct hostent *server = TRY(gethostbyname(hostname),
		server < 0,
		"ERROR while getting hostname %s. (%d: %s)\n", hostname, errno, strerror(errno));
=======
	
	int client_socket = 0;
	TRY((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0, "ERROR opening socket: %d (%s)", errno, strerror(errno));

	// retrieve host by name
	struct hostent *server;
	TRY((server = gethostbyname(SERVER_HOSTNAME)) < 0, "ERROR, no such host as %s: %d\n", SERVER_HOSTNAME, h_errno);
>>>>>>> 9dc42fcc64a4578aebee9e3c1c80a5892809b734

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

<<<<<<< HEAD
	printf("result=%d\n", result);
	printf("client_socket=%d\n", client_socket);

	result = TRY(write(client_socket, MESSAGE, strlen(MESSAGE) + 1),
		result < 0,
		"Error while writing the message on the socket file descriptor: %d (%s)", errno, strerror(errno));
=======
	TRY(connect(client_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0, "ERROR connecting socket: %d (%s)", errno, strerror(errno));

    int ret = 0;
	ret = write(client_socket, MESSAGE, strlen(MESSAGE));
	TRY(ret < 0, "Error while writing the message on the socket file descriptor");
>>>>>>> 9dc42fcc64a4578aebee9e3c1c80a5892809b734

	while (TRUE) {
		int size = BUFFER_SIZE;
		int qty_read = 0;
		char buffer[BUFFER_SIZE] = "";
<<<<<<< HEAD
		qty_read = TRY(read(client_socket, buffer, size - 1),
			qty_read < 0,
			"Error while reading the socket content.\n");
=======
		TRY((qty_read = read(client_socket, buffer, size - 1)) < 0, "Error while reading the socket content.\n");
>>>>>>> 9dc42fcc64a4578aebee9e3c1c80a5892809b734
		buffer[BUFFER_SIZE - 1] = '\0';
		printf("Socket content: %s\n", buffer);
		if (qty_read != size - 1) {
			// we suppose it's because the end of stream
			break;
		}
	}

<<<<<<< HEAD

cleanup:
	if (client_socket) {
		close(client_socket);
	}
=======
	TRY(close(client_socket) < 0, "Error while closing the socket");
cleanup:
>>>>>>> 9dc42fcc64a4578aebee9e3c1c80a5892809b734
	return result;
}
