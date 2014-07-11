all:
	gcc -E -Wall -Werror -o tcp_client.i tcp_client.c -lws2_32
	gcc -Wall -Werror -o tcp_client tcp_client.c -lws2_32
	#gcc -Wall -Werror -o tcp_server tcp_server.c -lws2_32

clean:
	rm -rf *.exe
