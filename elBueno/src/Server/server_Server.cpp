#include "server_Server.h"
#include "server_ClientManager.h"
#include "server_ProxyCliente.h"
#include <vector>
#include <mutex>

Server::Server(const char* ip, const char* port) :
		ip(ip), port(port) {
}

Server::~Server() {
	sktPasivo.shutdown();
}

void Server::run() {
	std::vector<ClientManager*> clientes;
	std::vector<VirtualMachine> MVs;
	sktPasivo.bindAndListen(ip, port);
	while (!terminar) {
		Socket sktActivo(sktPasivo.accept());
		if (sktActivo.isValid()) {
			std::cout
					<< "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
			ProxyCliente proxy(std::move(sktActivo));
			uint8_t indexMV;
			std::string vmName;
			proxy.sendLobbiesMenu(MVs);
			if (proxy.recvCommandL(indexMV,vmName)) {
				if (indexMV == 255) {
					MVs.push_back(std::move(VirtualMachine(vmName)));
					indexMV = MVs.size() - 1;
				}
				std::cout << "SERVER ANTES DE PUSHEAR NUEVO CLIENTE"
						<< std::endl;
				std::cout << "A MV: " << MVs[indexMV].getName() << std::endl;
				clientes.push_back(
						new ClientManager(std::move(proxy), MVs[indexMV], MVs));
				clientes[clientes.size() - 1]->start();
			}
		}
	}
	for (unsigned i = 0; i < clientes.size(); ++i) {
		clientes[i]->stopListening();
		clientes[i]->join();
		delete clientes[i];
	}
}

void Server::stop() {
	terminar = true;
	sktPasivo.shutdown();
}
