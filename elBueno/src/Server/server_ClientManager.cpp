#include "server_ClientManager.h"
#include "Parser.h"
#include <sstream>
#include <unistd.h>

ClientManager::ClientManager(ProxyCliente proxy, VirtualMachine &mv,
		std::vector<VirtualMachine> &availableVMs) :
		proxy(std::move(proxy)), vm(mv), availableVMs(availableVMs) {
	vm.connectNewClient(&this->proxy);
	sendAll();
}

ClientManager::~ClientManager() {
}

void ClientManager::stopListening() {
	keepListening = false;
	proxy.close();
}

void ClientManager::run() {
	while (keepListening) {
		char command = proxy.recvChar();
		switch (command) {
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
		case 'D': {
//			commandD();
			break;
		}
		default: {
			stopListening();
			break;
		}
		}
	}
}

void ClientManager::commandO() {
	std::string newObjectName;
	uint32_t positionX, positionY;
	proxy.recvCommandO(newObjectName, positionX, positionY);
	vm.lock();
	CustomObject *newObject = static_cast<CustomObject*>(vm.getLobby()->addSlot(
			newObjectName, vm.createObject(newObjectName), 'I'));
	if (newObject == NULL) {
		proxy.sendError("ERROR: Ya existe un objeto con ese nombre. ");
	} else {
		newObject->setPosition(positionX, positionY);
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandO(newObjectName, positionX, positionY);
		}
	}
	vm.unlock();
}
void ClientManager::commandS() {
	std::string object, stringParse;
	ObjectMasCapo* slot;
	bool err = false;
	char type = proxy.recvCommandS(object, stringParse);
	vm.lock();
	if (type == '0') {
		std::stringstream code(stringParse);
		Parser parserCode(&code, this->vm);
		slot = parserCode.script();
		if (slot == NULL) {
			proxy.sendError("ERROR: Ya existe un slot con ese nombre. ");
			err = true;
		}
	} else {
		CustomObject* obj = static_cast<CustomObject*>(vm.lookup(object));
		if (obj == NULL) {
			proxy.sendError("ERROR: Objeto inexistente. ");
			err = true;
		} else {
			obj->addCode(stringParse);
			std::map<std::string, ObjectMasCapo*> argument;
			slot = vm.message(NULL, obj->getName(), argument);
			obj->addCode("");
		}
	}
	if (!err) {
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandS(object, slot->getName(),
					slot->getValue(), slot->getFlag(), getParentName(slot));
		}
	}
	vm.unlock();
}

void ClientManager::commandP() {
	std::string objectToMove;
	uint32_t positionX, positionY;
	char type = proxy.recvCommandP(objectToMove, positionX, positionY);
	vm.lock();
	bool err = false;
	if (type == '0') {
		CustomObject* obj = static_cast<CustomObject*>(vm.lookup(objectToMove));
		if (obj == NULL) {
			proxy.sendError("ERROR: Objeto inexistente. ");
			err = true;
		} else {
			obj->setPosition(positionX, positionY);
		}
	}
	if (!err) {
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandP(objectToMove, positionX, positionY);
		}
	}
	vm.unlock();
}

void ClientManager::commandM() {
	std::string receiverName, message;
	uint32_t positionX, positionY;
	std::map<std::string, ObjectMasCapo*> argument;
	char type = proxy.recvCommandM(receiverName, message, positionX, positionY);
	vm.lock();
	ObjectMasCapo* receiver = vm.lookup(receiverName);
	if (receiver == NULL) {
		proxy.sendError("ERROR: Objeto inexistente. ");
	} else {
		ObjectMasCapo* result = vm.message(receiver, message, argument);
		if (result == NULL)
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
		}
	}
	vm.unlock();
}

void ClientManager::commandE() {
	std::cout << "CLienteManager: comando E" << std::endl;
	std::string objectName, slotName;
	char type = proxy.recvCommandE(objectName, slotName);
	vm.lock();
	if (type == '1') {
		for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
			vm.getClient(i)->sendCommandE(objectName, slotName);
		};
	} else {
		CustomObject *object = static_cast<CustomObject*>(vm.lookup(objectName));
		if (object == NULL) {
			proxy.sendError("ERROR: Objeto inexistente. ");
		} else {
			bool result = true;
			object->removeSlot(slotName);
			if (result) {
				for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
					vm.getClient(i)->sendCommandE(objectName, slotName);
				}
			} else {
				proxy.sendError("ERROR: Slot inexistente. ");
			}
		}
	}
	vm.unlock();
}

void ClientManager::commandR() {
	std::string objectName, slotName, newSlotName;
	proxy.recvCommandR(objectName, slotName, newSlotName);
	vm.lock();
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
		}
	}
	vm.unlock();
}

void ClientManager::sendAll() {
	CustomObject *lobby, *object, *slot;
	vm.lock();
	lobby = vm.getLobby();
	std::vector<std::string> slotList, objectList = lobby->getIndex();
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
					slot->getValue(), slot->getFlag(),getParentName(slot));
		}
	}
	vm.unlock();
}

std::string ClientManager::getParentName(ObjectMasCapo *obj) {
	std::string parentName = "";
	if (obj->isParent()) {
		parentName = static_cast<CustomObject*>(obj)->getElementInIndexAt(0);
	}
	return parentName;
}

void ClientManager::commandD() {
	std::string objectToMove;
	proxy.sendLobbiesMenu(availableVMs);
	uint8_t index = proxy.selectedLobby();
	proxy.recvCommandD(objectToMove);
	vm.lock();
	CustomObject *slot, *object = static_cast<CustomObject*>(vm.lookup(
			objectToMove));
	if (object == NULL) {
		proxy.sendError("ERROR: Objeto inexistente. ");
	} else {
		availableVMs[index].lock();
		static_cast<CustomObject*>(availableVMs[index].getLobby())->addSlot(
				object->getName(), object, object->getFlag());
		std::vector<std::string> slotList = object->getIndex();
		for (unsigned i = 0; i < availableVMs[index].numberOfUsers(); ++i) {
			availableVMs[index].getClient(i)->sendCommandO(object->getName(),
					object->getPositionX(), object->getPositionY());
			for (unsigned i = 0; i < slotList.size(); ++i) {
				slot = static_cast<CustomObject*>(object->lookup(slotList[i]));
				usleep(250000);
				availableVMs[index].getClient(i)->sendCommandS(
						object->getName(), slot->getName(), slot->getValue(),
						slot->getFlag(), getParentName(slot));
			}
		}
		availableVMs[index].unlock();
		bool result = true;      // CAMBIAR CUANDO INTEGRE CON SOFI
		object->removeSlot(objectToMove);
		if (result) {
			for (unsigned i = 0; i < vm.numberOfUsers(); ++i) {
				vm.getClient(i)->sendCommandE("lobby", objectToMove);
			}
		}
		vm.unlock();
	}
}
