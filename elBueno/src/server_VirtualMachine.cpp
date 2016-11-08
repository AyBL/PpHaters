#include "server_VirtualMachine.h"
#include <iostream>
#include <sstream>

#include "Parser.h"

VirtualMachine::VirtualMachine(std::string name) :
		name(name) {
	lobby = createObject("lobby");
}

VirtualMachine::~VirtualMachine() {
//	for (unsigned i = 0; i < objectsCreated.size(); ++i) {
//		delete objectsCreated[i];
//	}
	delete lobby; // si lo paso a stack no es necesario
}

void VirtualMachine::collect() {

}

//---------------------------------------------------------
CustomObject* VirtualMachine::createObject(std::string name) {
	CustomObject* newObject = new CustomObject(name);
	objectsCreated.push_back(newObject);
	return newObject;
}

NumberObject* VirtualMachine::createObject(std::string name, double value) {
	NumberObject* newObject = new NumberObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}

StringObject* VirtualMachine::createObject(std::string name,
		std::string value) {
	StringObject* newObject = new StringObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}

BoolObject* VirtualMachine::createObject(std::string name, bool value) {
	BoolObject* newObject = new BoolObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}
//-----------------------------------------------------------

ObjectMasCapo* VirtualMachine::lookup(std::string slotName) {
//	std::cout<<"buscando slot: "<<slotName<<std::endl;
	if (lobby->getName() == slotName) {
		return lobby;
	} else {
		return lobby->lookup(slotName);
	}
}
ObjectMasCapo* VirtualMachine::message(ObjectMasCapo* receiver,
		std::string method, std::map<std::string, ObjectMasCapo*> argument) {
	ObjectMasCapo* aux = receiver->lookup(method);

	if (aux != NULL) {
//		std::cout<<"no encontrado "<<std::endl;
		if (aux->getCode() != "") {
			std::stringstream code(aux->getCode());
//			std::cout<< "llama a parser con cod:  "<<aux->getCode() <<std::endl;
			Parser parserCode(&code);
			parserCode.script();
		}
		return NULL;
	} else {
//		std::cout<<"no encontrado "<<std::endl;
		receiver = receiver->execute(method, argument);
		return receiver;
	}
}

