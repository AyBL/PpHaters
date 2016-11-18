#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

class Socket{
    public:
          explicit Socket(int type);//Constructor
          /*Seteamos en que puerto queremos escuchar y cuantos clientes
          podemos mantener en espera antes de aceptar a alguno*/
          int BindAndListen(const char* host_name, char* port);
          //Elije la direccion que mejor funcione para poder conectarse
          int Connect(const char* host_name,const char* port);
          //Acepta un cliente y devuelve un nuevo socket
          Socket Accept();
          //Envia el buffer a traves de la red
          int Send(const char* buffer, size_t length);
          //Recibe informacion a traves de la red y lo guarda en el buffer
          int Receive(char* buffer, size_t length);
          //Devuelve si el socket es valido
          bool ValidSocket();

          Socket(const Socket&) = delete;
          Socket& operator=(const Socket&) = delete;
          Socket(Socket&& other);
          Socket& operator=(Socket&& other);
          //Cierra la conexion del socket
          void Shutdown();
          //Destruye los elementos de la estructura
          virtual ~Socket();

    private:
          Socket(int s,int type);
          void uninit();
          int skt;
          struct addrinfo hints;
};

#endif
