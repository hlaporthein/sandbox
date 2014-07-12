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
#include <errno.h>

#define PORT 22222
#define LISTEN_QUEUE_SIZE 5

#define TRUE 1
#define FALSE 0

////////////// error checking macro

#define BUFFER_SIZE 1024

#define TRY(statement, condition, ...) \
	statement; \
	if (condition) { \
		fprintf(stderr, __VA_ARGS__); \
		result = 1; \
		errno = 0; \
		goto cleanup; \
	} \

int handle_connection(int socket) {
	int result = 0;
	// here a socket is like a stream referenced by an integer
	// print on the screen the stream...

	while (TRUE) {
		int size = 16;
		int qty_read = 0;
		char buffer[16] = "";
		qty_read = TRY(recv(socket, buffer, size - 1, 0),
			qty_read < 0,
			"Error while reading the socket content.\n");
		buffer[15] = '\0';
		printf("Socket content: %s\n", buffer);
		if (qty_read != size - 1) {
			// we suppose it's because the end of stream
			break;
		}
	}

	// write on the socket file descriptor an answer.
	char *answer = "Message received!";
	int ret = TRY(send(socket, answer, strlen(answer) + 1, 0), ret < 0, "Error while answering");
	printf("ret=%d\n", ret);
cleanup:
	return result;
}

int main() {
#ifdef __MINGW32__
	WORD versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);
	printf("Windows version.\n");
#else
	printf("Posix version.\n");
#endif
	printf("Starting TCP Server on Port %d.\n", PORT);


	int result = 0;
	// start the server
	// create a socket pointer
	// int socket(int domain, int type, int protocol);
	// domain : AF_UNIX (File system pathnames.) or AF_INET (Internet address.)
	// type : SOCK_STREAM, etc. (always SOCK_STREAM for a bidirectional socket (to be investigated later)
	int server_socket = 0;
	server_socket = TRY(socket(AF_INET, SOCK_STREAM, 0),
		server_socket < 0,
		"ERROR opening socket (%d: %s)", errno, strerror(errno));

	struct    sockaddr_in servaddr;
	// set all the fields to 0
	memset(&servaddr, 0, sizeof(servaddr));
	// set some fields to the desired values
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(PORT);

	result = TRY(bind(server_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)), result < 0, "ECHOSERV: Error calling bind()\n");

	result = TRY(listen(server_socket, LISTEN_QUEUE_SIZE), result < 0, "ECHOSERV: Error calling listen()\n");

	while (TRUE) {

		// Wait for a connection, then accept() it
		int client_socket = 0;
		TRY((client_socket = accept(server_socket, NULL, NULL)),
			client_socket < 0,
			"ECHOSERV: Error calling accept()\n");

		result = TRY(handle_connection(client_socket),
			result != 0,
			"Error while handling a connection\n");

		// Close the connected socket
#ifdef __MINGW32__
#define close closesocket
#endif
		result = TRY(close(client_socket),
			result < 0, "ECHOSERV: Error calling close(): %d %s\n", errno, strerror(errno));

	}


cleanup:
	return result;
}

