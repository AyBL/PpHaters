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
	// cambiar por un char de flag y mas facil, alta gil soy
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

//AGREGA SOFI
char ObjectMasCapo::getFlag() const{
	if (argument)
		return 'A';
	else if (parent)
		return 'P';
	else if (inmutable)
		return 'I';
	else
		return 'M';
}

void ObjectMasCapo::rename(std::string newName) {
	objectName = newName;
}

std::string ObjectMasCapo::getName() const {
	return objectName;
}

bool ObjectMasCapo::isParent() const {
	return this->parent;
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

std::string ObjectMasCapo::getCode() const{
	return code;
}

std::vector<std::string> ObjectMasCapo::getIndex(){
	std::vector<std::string> v;
	return v;
}

Json::Value ObjectMasCapo::toJson(){}
//ObjectMasCapo* ObjectMasCapo::execute(std::string method,
//		std::map<std::string, ObjectMasCapo*> arguments) {
//	MFP fp = methods[method];
//	return (this->*fp)(arguments);
//}

void ObjectMasCapo::notTemp(){
	this->temporal = false;
}

void ObjectMasCapo::yesTemp(){
	this->temporal = true;
}

bool ObjectMasCapo::isTemp() const{
	return this->temporal;
}
