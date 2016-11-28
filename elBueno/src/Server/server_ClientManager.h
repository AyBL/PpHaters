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
	bool keepListening = true;
	std::vector<VirtualMachine> &availableVMs;
public:
	ClientManager(ProxyCliente proxy, VirtualMachine &mv, std::vector<VirtualMachine> &availableVMs );
	virtual ~ClientManager();
//	char recvCommand();
	void run();
	void stopListening();
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
	void commandD();
	void sendAll();
};

#endif /* SERVER_CLIENTMANAGER_H_ */
