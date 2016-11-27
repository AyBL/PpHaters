#include "common_Socket.h"
#include <cstring>
#include <unistd.h>
#include <string>
//#include <exception>

#define INVALID_FILE_DESCRIPTOR -1

Socket::Socket() {
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;

	socket = ::socket(hints.ai_family, hints.ai_socktype, hints.ai_flags);
	if (socket == -1) {
		throw std::string("ERROR no se pudo crear el socket");
	}
}
Socket::Socket(int s) :
		socket(s) {
}

Socket& Socket::operator=(Socket&& other) {
	socket = std::move(other.socket);
	other.socket=-1;
	return *this;
}

Socket::Socket(Socket&& other):socket(std::move(other.socket)) {
	other.socket=-1;
}

bool Socket::isValid() {
	if (socket == -1) {
		return false;
	}
	return true;
}

Socket::~Socket() {
	if (socket != INVALID_FILE_DESCRIPTOR)
		close(socket);
}

void Socket::bindAndListen(const char *ip,const char *port) {
	int s;
	struct addrinfo *ptr;
	s = getaddrinfo(ip, port, &hints, &ptr);
	if (s != 0) {
		throw std::string("Error in getaddrinfo: %s\n", gai_strerror(s));
	}
	s = bind(socket, ptr->ai_addr, ptr->ai_addrlen);
	if (s == -1) {
		freeaddrinfo(ptr);
		throw std::string("ERROR No se pudo hacer el bind");
	}
	freeaddrinfo(ptr);
	s = listen(socket, 10);
	if (s == -1) {
		throw std::string("ERROR No se pudo hacer el listen");
	}
}

// luego de hacer el accept hay que ver que sea valido
// invocando a isValid()
Socket Socket::accept() {
	int s;
	s = ::accept(socket, NULL, NULL);
	Socket skt(s);
	return skt;
}

void Socket::connect(const char *host_name, const char *port) {
	bool conectado = false;
	struct addrinfo *ptr, *result;

	int s = getaddrinfo(host_name, port, &hints, &result);
	if (s != 0) {
		throw std::string("Error in getaddrinfo: %s\n", gai_strerror(s));
	}
	for (ptr = result; (ptr != NULL && !conectado); ptr = ptr->ai_next) {
		if (s != 0) {
			freeaddrinfo(result);
		}
		s = ::connect(socket, result->ai_addr, result->ai_addrlen);
		if (s == -1) {
			freeaddrinfo(result);
			throw std::string("ERROR en connect");
		}
		conectado = true;
	}
	freeaddrinfo(result);
}

int Socket::send(const char *buffer, int length) {
	int enviado = 0;
	bool SinError = true;
	int s = 0;
//	memset(&buffer[length], 0, 15);
	while ((enviado < length) && (SinError)) {
		s = ::send(socket, &buffer[enviado], length - enviado, MSG_NOSIGNAL);
		if (s < 0) {
			SinError = false;
		} else if (s == 0) {
			SinError = false;
		} else {
			enviado += s;
		}
	}
	if (SinError) {
//		std::cout<<"+++++++++enviado: "<< buffer<<std::endl;
		return enviado;
	}
//	std::cout<<"++++++++++++++enviado: "<< buffer<<std::endl;
	return -1;
}

int Socket::recv(char *buffer, int length) {
	int recibido = 0;
	int s = 0;
	bool SinError = true;
	memset(buffer, 0, length);
	while ((recibido < length) && (SinError)) {
		s = ::recv(socket, &buffer[recibido], length - recibido, MSG_NOSIGNAL);
		if (s < 0) {
			SinError = false;
		}
		if (s == 0) {
			SinError = false;
		}
		recibido += s;
	}
	if (SinError) {
//		std::cout<<"++++++++recibido: "<< buffer<<std::endl;
		return recibido;
	}
//	std::cout<<"++++++++++++++recibido: "<< buffer<<std::endl;
	return -1;
}

void Socket::shutdown() {
	::shutdown(socket, SHUT_RDWR);
}

