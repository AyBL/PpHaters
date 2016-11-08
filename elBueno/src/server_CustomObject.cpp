#include "server_CustomObject.h"
#include <iostream> // para el print
CustomObject::CustomObject(std::string name) :
		ObjectMasCapo(name) {
//	self= this;3
//	slots.push_back(this);
	// agrego los metodos nativos
	methods["print"] = &CustomObject::print;
	methods["clone"] = &CustomObject::clone;
	methods["_addSlot:"] = &CustomObject::addSlot;

}

CustomObject::~CustomObject() {
	for (unsigned i = 1; i < slots.size(); ++i) {
		delete slots[i];
	}
}

bool CustomObject::nameRepeats(std::string name) {
	unsigned i = 0;
	while (i < slots.size()) {
		if (slots[i]->getName() == name) {
			return true;
		}
		++i;
	}
	return false;
}

void CustomObject::addSlot(std::string slotName, ObjectMasCapo* newSlot,
		char flag) {
	if (nameRepeats(slotName)){
		std::cout<< "ERROR nombre repetido "<<std::endl;
	}
	if (flag == 'P') {
		CustomObject *slotParent = new CustomObject(slotName);
		CustomObject *other = static_cast<CustomObject*>(newSlot);
		for (unsigned i = 0; i < other->slots.size(); ++i) {
			slotParent->slots.push_back(other->slots[i]);
		}
		slotParent->setFlags(flag);
		slots.push_back(slotParent);
	} else {
		newSlot->rename(slotName);
		newSlot->setFlags(flag);
		slots.push_back(newSlot);
	}
}

ObjectMasCapo* CustomObject::addSlot(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	CustomObject *other = static_cast<CustomObject*>(it->second);
	for (unsigned i = 0; i < other->slots.size(); ++i) {
		this->slots.push_back(other->slots[i]); // aca deberia moverlos no copiarlos
		// o bien hacer que despues apunte a null
		// porque cuando el garbarge pase y lo elimine le va a eliminar los
		// los slot y NO QUEREMOS ESO
//		std::cout<<"se agrega el slot "<<std::endl;
	}
	return this;
}

void CustomObject::removeSlot(ObjectMasCapo* newSlot) {

}
void CustomObject::removeSlot(std::string slotName) {

}

ObjectMasCapo* CustomObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "CustomObject clone" << std::endl;
	return this;
}

ObjectMasCapo* CustomObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "CustomObject Print" << std::endl;
//	for (unsigned i=0;i<slots.size();++i){
//		slots[i]->print(arguments);
//	}
	return this; /// no se que deberia hacer este print
}

void CustomObject::addCode(std::string newCode) {
	this->code = newCode;
}

ObjectMasCapo* CustomObject::lookup(std::string slotName) {
// POR AHORA SOLO RETONA SI ESTA DENTRO DEL EL QUE LO LLAMA
// ME FALTA HACERLO RECUSIVO CON LOS PARENT
	std::vector<ObjectMasCapo*> parent;
	unsigned i = 0;
	while (i < slots.size()) {
		if (slots[i]->getName() == slotName) {
			return slots[i];
		} else {
			if (slots[i]->isParent()) {
//				slots[i]->lookup(slotName);
				parent.push_back(slots[i]);
			}
		}
		++i;
	}
	ObjectMasCapo *aux, *resultado = NULL;
	bool encontrado = false;
	for (unsigned i = 0; i < parent.size(); ++i) {
		aux = parent[i]->lookup(slotName);
		if (aux != NULL) {
			if (!encontrado) {
				encontrado = true;
				resultado = aux; //
			} else {
				std::cout << "ERROR dos parent con el mismo slot" << std::endl;

			}
		}
	}
	return resultado;
}

ObjectMasCapo* CustomObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}
