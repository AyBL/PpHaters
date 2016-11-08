#include "server_ObjectMasCapo.h"
#include <iostream>

ObjectMasCapo::ObjectMasCapo(std::string objectName) :
		objectName(objectName) {
//	std::cout << "se creo o+c nombre: " << this->objectName << std::endl;
//	methods["print"] = &ObjectMasCapo::print;
//	methods["clone"] = &ObjectMasCapo::clone;

}

ObjectMasCapo::~ObjectMasCapo() {
}

void ObjectMasCapo::setFlags(char flag ){
	switch (flag) {
		case 'A': {
			argument = true;
			break;
		}
		case 'P': {
			parent = true;
			inmutable = true;
			break;
		}
		case 'I': {
			inmutable = true;
			break;
		}
		default:
			break;
		}
}

void ObjectMasCapo::rename(std::string newName) {
	objectName = newName;
}

std::string ObjectMasCapo::getName() {
	return objectName;
}

bool ObjectMasCapo::isParent() {
	return parent;
}

//void ObjectMasCapo::addMethod(std::string newMethodName,
//		std::map<std::string, ObjectMasCapo*> arguments){
//	methods[newMethodName]= arguments;
//}
//
//void ObjectMasCapo::removeMethod(std::string methodToRemove){
//	if (methods.find(methodToRemove) == methods.end()) {
//		//no existe metodo error??
//	}else {
//		methods.erase(methodToRemove);
//	}
//}

ObjectMasCapo* ObjectMasCapo::lookup(std::string slotName) {
	if (slotName == objectName) {
		return this;
	} else {
		return NULL;
	}
}

std::string ObjectMasCapo::getCode() {
	return code;
}

//ObjectMasCapo* ObjectMasCapo::execute(std::string method,
//		std::map<std::string, ObjectMasCapo*> arguments) {
//	MFP fp = methods[method];
//	return (this->*fp)(arguments);
//}
