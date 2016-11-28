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
			commandS();
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

	// vm.lock();
	CustomObject *newObject = static_cast<CustomObject*>(vm.getLobby()->addSlot(
			newObjectName, vm.createObject(newObjectName), 'I'));
	if (newObject == NULL) {
		proxy.sendError("ERROR: Ya existe un objeto con ese nombre. ");
	} else {
		newObject->setPosition(positionX, positionY);
		std::cout << "\\\\\\\\\\ClientManager::commandO()\\\\\\\\\n";
		std::cout << "antes del for" << std::endl;
		std::cout << vm.numberOfUsers() << std::endl;
		// vm.unlock();

		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			std::cout << "iteracion de los proxy:  " << i << std::endl;
			std::cout << vm.getClient(i) << std::endl;
			std::cout << " " << &proxy << std::endl;
			vm.getClient(i)->sendCommandO(newObjectName, positionX, positionY);
			std::cout << "" << std::endl;
		}
		std::cout << "           fin de commando O" << std::endl;
	}
	//v,.unlock();
}

void ClientManager::commandS() {
//	A (Add Slot)	tam	nombreobj	tam	nombreslot	tam	value	a/i/m
//	A	tam	nombreobj	tam	nombreslot	tam	value	p	tam	nombre
//	A	tam	string
	std::cout << "CLienteManager: comando A" << std::endl;
	std::string object, stringParse;
	char type = proxy.commandS(object, stringParse);

	// vm.lock();
	std::cout << "CLienteManager: string to parse " << stringParse << std::endl;
	std::stringstream code(stringParse);
	Parser parserCode(&code, this->vm);
	ObjectMasCapo* slot = parserCode.script();

	if (slot == NULL) {
		if (type == '1') {
			proxy.sendError("ERROR: Slot inexistente. ");
		} else {
			proxy.sendError("ERROR: Ya existe un slot con ese nombre. ");
		}
	} else {
		std::string parentName = "";
		if (slot->isParent())
			parentName = static_cast<CustomObject*>(slot)->getElementInIndexAt(
					0);
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandS(object, slot->getName(),
					slot->getValue(), slot->getFlag(), parentName);
		}
		std::cout << "CLienteManager: fin de comando A " << stringParse
				<< std::endl;
	}
	//vm.unlock();
}

void ClientManager::commandP() {
	std::string objectToMove;
	uint32_t positionX, positionY;
	proxy.commandP(objectToMove, positionX, positionY);
	//vm.lock();
	CustomObject* obj = static_cast<CustomObject*>(vm.lookup(objectToMove));
	if (obj == NULL) {
		proxy.sendError("ERROR: Objeto inexistente. ");
	} else {
		obj->setPosition(positionX, positionY);
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandP(objectToMove, positionX, positionY);
		}
	}
//	vm.unlock();
}

void ClientManager::commandM() {
	//Enviar	M (Message)	tam	nomobj	tam	nomslot	x	y
	//Recibir	M	tam	nomobj	tam	string	x	y
	std::cout << "CLienteManager: entra comando M " << std::endl;
	std::string receiverName, message;
	uint32_t positionX, positionY;
	std::map<std::string, ObjectMasCapo*> argument;
	char type = proxy.commandM(receiverName, message, positionX, positionY);
	//vm.lock();
	ObjectMasCapo* receiver = vm.lookup(receiverName);
	if (receiver == NULL) {
		proxy.sendError("ERROR: Objeto inexistente. ");
	} else {
		std::cout << "CLienteManager: receiver " << receiver->getName()
				<< std::endl;
		std::cout << "CLienteManager: message " << message << std::endl;
		ObjectMasCapo* result = vm.message(receiver, message, argument);
		if (receiver == NULL)
			proxy.sendError("ERROR: Slot inexistente. ");
		else {
			receiverName = "";
			if (type == '1') {
				receiverName = receiver->getName();
			}
			for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
				vm.getClient(i)->sendCommandM(receiverName, result->getValue(),
						positionX, positionY);
			}
			std::cout << "CLienteManager: fin comando M " << std::endl;
		}
	}
//	vm.unlock();
}

void ClientManager::commandE() {
//	Enviar 	E (Erase Object)	tam	nomobj
//	Reecibir	E	tam	nomobj
	std::cout << "CLienteManager: comando E" << std::endl;
	std::string objectName, slotName;
	char type = proxy.commandE(objectName, slotName);
	//vm.lock();
	if (type == '1') {
		proxy.sendCommandE(objectName, slotName);
	} else {
		CustomObject *object = static_cast<CustomObject*>(vm.lookup(objectName));
		if (object == NULL) {
			proxy.sendError("ERROR: Objeto inexistente. ");
		} else {
			bool result = object->removeSlot(slotName);
			if (true) {
				for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
					vm.getClient(i)->sendCommandE(objectName, slotName);
				}
			} else {
				proxy.sendError("ERROR: Slot inexistente. ");
			}
		}
	}
	//	vm.unlock();
	std::cout << "CLienteManager: comando E FIN" << std::endl;
}

void ClientManager::commandR() {
//	Enviar	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
//	Recibir	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
	std::cout << "CLienteManager: comando R" << std::endl;
	std::string objectName, slotName, newSlotName;
	proxy.commandR(objectName, slotName, newSlotName);
//	vm.lock();
	CustomObject *object = static_cast<CustomObject*>(vm.lookup(objectName));
	if (object == NULL) {
		proxy.sendError("ERROR: Objeto inexistente. ");
	} else {
		CustomObject *slot =
				static_cast<CustomObject*>(object->lookup(slotName));
		if (slot == NULL) {
			proxy.sendError("ERROR: Slot inexistente. ");
		} else {
			bool result = object->renameSlot(slot->getName(), newSlotName);
			if (result) {
				for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
					vm.getClient(i)->sendCommandR(objectName, slotName,
							newSlotName);
				}
			} else {
				proxy.sendError("ERROR: Ya existe un slot con ese nombre. ");
			}
//	vm.unlock();
		}
		std::cout << "CLienteManager: comando R   FIN" << std::endl;
	}

	void ClientManager::sendAll() {
//	mv.lock();
		CustomObject *object, *slot, *lobby;
		lobby = vm.getLobby();
		std::vector<std::string> slotList, objectList = lobby->getIndex();
		std::cout << "sendAll antes del for " << std::endl;
		for (unsigned i = 0; i < objectList.size(); ++i) {
			object = static_cast<CustomObject*>(vm.lookup(objectList[i]));
			usleep(250000);
			proxy.sendCommandO(object->getName(), object->getPositionX(),
					object->getPositionY());
			slotList = object->getIndex();
			for (unsigned i = 0; i < slotList.size(); ++i) {
				slot = static_cast<CustomObject*>(object->lookup(slotList[i]));
				usleep(250000);
				proxy.sendCommandS(object->getName(), slot->getName(),
						slot->getValue(), slot->getFlag(), "parent");
			}
			std::cout << "fin sendALL:" << std::endl;
		}
//	mv.unlock();
	}

