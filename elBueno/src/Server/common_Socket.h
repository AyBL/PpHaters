#ifndef SOCKET_H_
#define SOCKET_H_

#include <netdb.h>
#include <sys/socket.h>
#include <iostream>

class Socket {
private:
	int socket;
	struct addrinfo hints = { 0 };

public:
	Socket();

	~Socket();

	void bindAndListen(const char *ip,const char *port);

	Socket accept();

	void connect(const char *host_name, const char *port);

	int send(const char *buffer, int length);

	int recv(char *buffer, int length);

	void shutdown();

	bool isValid();

	Socket& operator=(Socket&& other);

	Socket(Socket&& other);

private:
	explicit Socket(int s);

	Socket(const Socket&) = delete;

	Socket& operator=(const Socket&) = delete;
};

#endif /* SOCKET_H_ */
