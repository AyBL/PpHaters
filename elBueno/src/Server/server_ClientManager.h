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
	void run();
	void stopListening();
private:
	/* creacion de un nuevo objeto en el lobby */
	void commandO();
	/* creacion o cambio de valor de slot en un objeto */
	void commandS();
	/* cambio de posiscion de un objeto */
	void commandP();
	/* mensajes */
	void commandM();
	/* borrado de objeto/slot */
	void commandE();
	/* renombrar objeto/slot */
	void commandR();
	/* mover objeto de un lobby a otro */
	void commandD(); //NO PROBADO FALTA GRAFICA
	/* envia todo el contenido del lobby  */
	void sendAll();
	/* retorna el nombre de su parent,
	 * de no poseer, string vacio*/
	std::string getParentName(ObjectMasCapo *obj);
};

#endif /* SERVER_CLIENTMANAGER_H_ */
