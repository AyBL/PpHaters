#include "server_Server.h"
#include "server_ClientManager.h"
#include "server_ProxyCliente.h"
#include <vector>
#include <mutex>

Server::Server(const char* script) :
		scriptFile(script) {
//	std::fstream arch;
//	arch.open(argv[1]);
//	VirtualMachine vm("lobby");
//	//		vm.JsonLoad("salida1.txt");
//	try {
//		Parser p(&arch, vm);
//		bool pudoParsear = p.script();
//		arch.close();
//	} catch (...) {
//		std::cout << "SYNTAX ERROR" << std::endl;
//	}
//	vm.JsonSave("salida1.txt");
}

Server::Server(const char* ip, const char* port) :
		ip(ip), port(port) {
}

Server::~Server() {
	sktPasivo.shutdown();
}

void Server::run() {
	std::vector<Thread*> clientes;
	std::vector<VirtualMachine> MVs;
//	std::map<std::string, std::vector<ProxyCliente*>> clients;
	sktPasivo.bindAndListen(ip, port);
	while (!terminar) { // cambiuar por el bloqueo del accept o algo asi
		Socket sktActivo(sktPasivo.accept());
		if (sktActivo.isValid()) {
			std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
			ProxyCliente proxy(std::move(sktActivo));
			uint8_t indexMV;
			proxy.sendLobbiesMenu(MVs);
			char command = proxy.recvMessage();
			if (command == 'L') {
				indexMV = proxy.selectedLobby();
				std::cout << "Server : index ingresado  "<<indexMV << std::endl;
				if (indexMV == 255) {
					std::string newMV(proxy.getName());
					MVs.push_back(std::move(VirtualMachine(newMV)));
					indexMV = MVs.size() - 1;}
//				}else {indexMV = 0; }
//			}
				std::cout << "SERVER ANTES DE PUSHEAR NUEVO CLIENTE" << std::endl;
				std::cout << "A MV: "<<  MVs[(unsigned)indexMV].getName() << std::endl;
				clientes.push_back(
						new ClientManager(std::move(proxy), MVs[indexMV]));

				clientes[clientes.size() - 1]->start();
			}
		}
	}
	for (unsigned i = 0; i < clientes.size(); ++i) {
		clientes[i]->join();
		delete clientes[i];
	}
}

void Server::stop() {
	terminar = true;
	sktPasivo.shutdown();
}
