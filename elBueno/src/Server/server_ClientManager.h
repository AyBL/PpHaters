#ifndef SERVER_CLIENTMANAGER_H_
#define SERVER_CLIENTMANAGER_H_

#include "server_Thread.h"
#include "server_ProxyCliente.h"
#include "server_VirtualMachine.h"
#include "common_Socket.h"

#include <vector>

class ClientManager: public Thread{
private:
	ProxyCliente proxy;
	VirtualMachine &vm;
public:
	ClientManager(ProxyCliente proxy, VirtualMachine &mv);
	virtual ~ClientManager();
//	char recvCommand();
	void run();
private:
	//metodos para cada comando
//	void commandL();
	void commandO();
	void commandS();
	void commandP();
	void commandM();
	void commandC();
	void commandE();
	void commandR();

	void sendAll();
};

#endif /* SERVER_CLIENTMANAGER_H_ */
