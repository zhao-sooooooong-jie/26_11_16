.PHONY:all
all:client_tcp server_tcp
server_tcp:EchoServerMain.cc
	g++ -o $@ $^ -std=c++17
client_tcp:EchoClient.cc
	g++ -o $@ $^ -std=c++17
.PHONY:clean
clean:
	rm -f client_tcp server_tcp
