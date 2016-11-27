#include "clientsocket.h"

#define MAXLISTEN 20

Socket::Socket(int type){
   this->skt = 0;
  memset(&this->hints, 0, sizeof(struct addrinfo));
 	this->hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
 	this->hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
 	this->hints.ai_flags = type;     /* None (or AI_PASSIVE for server) */
}

Socket::Socket(int s,int type):skt(s){
  memset(&this->hints, 0, sizeof(struct addrinfo));
 	this->hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
 	this->hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
 	this->hints.ai_flags = type;     /* None (or AI_PASSIVE for server) */
}

int Socket::BindAndListen(const char* host_name,char* port){
   int s;

   struct addrinfo *ptr;

   s = getaddrinfo(host_name, port, &(this->hints), &ptr);

   this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

   s = bind(this->skt, ptr->ai_addr, ptr->ai_addrlen);
   if (s == -1) {
      close(this->skt);
      freeaddrinfo(ptr);
      return 1;
   }

   freeaddrinfo(ptr);

   s = listen(this->skt, MAXLISTEN);
   if (s == -1) {
      close(this->skt);
      return 1;
   }
   return 0;
}

int Socket::Connect(const char* host_name,const char* port){
   int s;
   bool are_we_connected;

   struct addrinfo *result, *ptr;

   are_we_connected = false;

   s = getaddrinfo(host_name, port, &(this->hints), &result);

   for (ptr = result; ptr != NULL && !are_we_connected; ptr = ptr->ai_next) {
      this->skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

      if (this->skt == -1) {
         return 1;
      }else{
         s = connect(this->skt, ptr->ai_addr, ptr->ai_addrlen);
         if (s == -1) {
            close(this->skt);
            return 1;
         }
         are_we_connected = (s != -1);
      }
   }
   freeaddrinfo(result);
   return 0;
}

Socket Socket::Accept(){
    int s = accept(this->skt, NULL, NULL);
    Socket accepted_socket(s,AI_PASSIVE);
    return accepted_socket;
}

int Socket::Send(const char* buffer, size_t length){
   int s;
   unsigned int bytes_sent;
   bool socket_error, remote_socket_closed;

   socket_error = false;
   remote_socket_closed = false;

   bytes_sent = 0;

   while (bytes_sent < length && !socket_error && !remote_socket_closed) {
      s = send(this->skt, &buffer[bytes_sent],
          length - bytes_sent, MSG_NOSIGNAL);

      if (s < 0) {
         socket_error = true;
      }else if (s == 0) {
         remote_socket_closed = true;
      }else {
         bytes_sent += s;
      }
   }

   if (!remote_socket_closed && !socket_error) {
      return bytes_sent;
   }else {
      return -1;
   }
}

int Socket::Receive(char* buffer, size_t length){
  return recv(this->skt,buffer, length, MSG_NOSIGNAL);
}

bool Socket::ValidSocket() {
    return (this->skt != -1);
}

Socket::Socket(Socket&& other) : skt(other.skt) {
    other.uninit();
}

Socket& Socket::operator=(Socket&& other){
    this->skt = other.skt;
    other.uninit();
    return *this;
}

void Socket::uninit() {
    this->skt = -1;
}

void Socket::Shutdown() {
    if (this->skt != -1){
        shutdown(this->skt, SHUT_RDWR);
        close(this->skt);
        uninit();
    }
}

Socket::~Socket() {
    Shutdown();
}
