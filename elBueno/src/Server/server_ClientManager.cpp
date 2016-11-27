#include "server_ClientManager.h"
#include "Parser.h"
#include <sstream>
#include <unistd.h>

ClientManager::ClientManager(ProxyCliente proxy, VirtualMachine &mv) :
		proxy(std::move(proxy)), vm(mv) {
	std::cout << "------client manager constructor------" << std::endl;
	std::cout << "      nueva direccion de proxy: " << &this->proxy
			<< std::endl;
	vm.connectNewClient(&this->proxy);
	sendAll();
}

ClientManager::~ClientManager() {
}

void ClientManager::run() {
	std::cout << "    entro a run client manager\n";
	bool j = true;
	while (j) {
		std::cout << "  recibe el proxy:  " << &this->proxy << std::endl;
		char command = proxy.recvMessage();
		std::cout << "  nombreMV  " << this->vm.getName() << std::endl;
		std::cout << "     clienteManager: nuevo comando ingresado: " << command
				<< "\n";
		switch (command) {
		//command L se hace afuera
		case 'O': {
			commandO();
			break;
		}
		case 'S': {
			commandA();
			break;
		}
		case 'P': {
			commandP();
			break;
		}
		case 'M': {
			commandM();
			break;
		}
		case 'E': {
			commandE();
			break;
		}
		case 'R': {
			commandR();
			break;
		}
		default: {
			std::cout << "ERROR: comando no existente\n";
			j = false;
			break;
		}
		}
	}
}

void ClientManager::commandO() {
	std::string newObjectName;
	uint32_t positionX, positionY;
	proxy.commandO(newObjectName, positionX, positionY);
	// falta el chequeo de si el objecto se crea correctamente
	/*Seccion critica*/
	CustomObject *newObject = vm.createObject(newObjectName);
	//aca puede salir mal por nombre repetido
	vm.getLobby()->addSlot(newObjectName, newObject, 'M');
	newObject->setPosition(positionX, positionY); // chequear
	// if  todoo ok se lo tengo que mandar a todos
	std::cout << "\\\\\\\\\\ClientManager::commandO()\\\\\\\\\n";
	std::cout << "antes del for" << std::endl;
	std::cout << vm.numberOfUsers() << std::endl;
	for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
		std::cout << "iteracion de los proxy:  " << i << std::endl;
		std::cout << vm.getClient(i) << std::endl;
		std::cout << " " << &proxy << std::endl;
		vm.getClient(i)->sendCommandO(newObjectName, positionX, positionY);
		std::cout << "" << std::endl;
	}
//	proxy.sendCommandO(newObjectName, positionX, positionY);
	//proxy.notifyAllCommandO(sarasa)
	/*Fin seccion critica*/
	std::cout << "           fin de commando O" << std::endl;
	std::cout << "\\\\\\\\\\                   \\\\\\\\\n";
}

void ClientManager::commandA() {
	std::cout << "CLienteManager: comando A" << std::endl;
	std::string object, stringParse;
	proxy.commandA(object, stringParse);
	std::cout << "CLienteManager: string to parse " << stringParse << std::endl;
	std::stringstream code(stringParse);
	Parser parserCode(&code, this->vm);
	// aca rompe todoo con lo del nombre repetido
	ObjectMasCapo* slot = parserCode.script();
//	ObjectMasCapo* slot = (static_cast<CustomObject*>(obj))->getSlot();
	proxy.sendCommandA(object, slot->getName(), slot->getValue(),
			slot->getFlag(), "parent");
	std::cout << "CLienteManager: fin de comando A " << stringParse
			<< std::endl;
}

void ClientManager::commandP() {
	std::string objectToMove;
	uint32_t positionX, positionY;
	proxy.commandP(objectToMove, positionX, positionY);
	CustomObject* obj = static_cast<CustomObject*>(vm.lookup(objectToMove));
	obj->setPosition(positionX, positionY);
	proxy.sendCommandP(objectToMove, positionX, positionY);
}

void ClientManager::commandM() {
	//Enviar	M (Message)	tam	nomobj	tam	nomslot	x	y
	//Recibir	M	tam	nomobj	tam	string	x	y
	std::cout << "CLienteManager: entra comando M " << std::endl;
	std::string receiverName, message;
	uint32_t positionX, positionY;
	std::map<std::string, ObjectMasCapo*> argument;
	char type = proxy.commandM(receiverName, message, positionX, positionY);
	ObjectMasCapo* receiver = vm.lookup(receiverName);
	std::cout << "CLienteManager: receiver " << receiver->getName()
			<< std::endl;
	std::cout << "CLienteManager: message " << message << std::endl;
	ObjectMasCapo* result = vm.message(receiver, message, argument);
	receiverName = "";
	if (type == '1') {
		receiverName = receiver->getName();
	}
	proxy.sendCommandM(receiverName, result->getValue(), positionX, positionY);
	std::cout << "CLienteManager: fin comando M " << std::endl;
}

void ClientManager::commandE() {
//	Enviar 	E (Erase Object)	tam	nomobj
//	Reecibir	E	tam	nomobj
	std::string objectName, slotName;
	proxy.commandE(objectName, slotName);
	std::cout << "antes de borrar: "
			<< vm.lookup(objectName)->lookup(slotName)->getName() << std::endl;
	static_cast<CustomObject*>(vm.lookup(objectName))->removeSlot(slotName);
	std::cout << "antes de borrar " << std::endl;
	//falta chequeo
	proxy.sendCommandE(objectName, slotName);
}

void ClientManager::commandR() {
//	Enviar	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
//	Recibir	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
	std::string objectName, slotName, newSlotName;
	proxy.commandR(objectName, slotName, newSlotName);
	std::cout << "antes de cambiar nombre: "
			<< vm.lookup(objectName)->lookup(slotName)->getName() << std::endl;
	vm.lookup(objectName)->lookup(slotName)->rename(newSlotName);
	std::cout << "despues de cambiar nombre: "
			<< vm.lookup(objectName)->lookup(slotName)->getName() << std::endl;
	//chequeo si pudo cambiar nombre
	proxy.sendCommandR(objectName, slotName, newSlotName);
}

void ClientManager::sendAll() {
	CustomObject *lobby = vm.getLobby();
	std::vector<std::string> slotList, objectList = lobby->getIndex();
	CustomObject *object, *slot;
	std::cout << "sendAll antes del for " << std::endl;
	//ME ESTARIA FALLANDO ACA NUNCA ENTRA
	for (unsigned i = 0; i < objectList.size(); ++i) {
		std::cout << "sendALL OBJECT it:" << i << std::endl;
		object = static_cast<CustomObject*>(vm.lookup(objectList[i]));
		usleep(250000);
		proxy.sendCommandO(object->getName(), object->getPositionX(),
				object->getPositionY());
		slotList = object->getIndex();
		for (unsigned i = 0; i < slotList.size(); ++i) {
			std::cout << "sendALL SLOTS it:" << i << std::endl;
			slot = static_cast<CustomObject*>(object->lookup(slotList[i]));
			usleep(250000);
			proxy.sendCommandA(object->getName(), slot->getName(),
					slot->getValue(), slot->getFlag(), "parent");
		}
		std::cout << "fin sendALL:" << std::endl;
	}
}
