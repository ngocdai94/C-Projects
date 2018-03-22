all: pa4_client pa4_server

pa4_client: hw4_client.cpp
	g++ hw4_client.cpp -std=c++11 -o pa4_client

pa4_server: hw4_server.cpp
	g++ hw4_server.cpp -std=c++11 -lpthread -o pa4_server

clean:
	rm -f pa4_client pa4_server
