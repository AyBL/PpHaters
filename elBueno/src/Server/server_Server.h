#ifndef ACCEPTCLIENT_H_
#define ACCEPTCLIENT_H_

#include "common_Socket.h"
#include "server_Thread.h"

class Server: public Thread{
private:
	const char* ip;
	const char* port;
	const char* scriptFile;
	Socket sktPasivo;
	bool terminar = false;
public:
	/* inicializa los atributos */
	Server(const char* script);
	Server(const char* ip,const char* porta);

	virtual ~Server();

	/* escucha conexiones y acepta clientes mientras
	 * !terminar */
	void run();

	/* setea terminar en true y silencia al socket*/
	void stop();
};

#endif /* ACCEPTCLIENT_H_ */
