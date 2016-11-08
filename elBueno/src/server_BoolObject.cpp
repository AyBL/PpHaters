#include "server_BoolObject.h"

#include <iostream>

BoolObject::BoolObject(std::string name, bool value) :
		ObjectMasCapo(name), value(value) {
// agregar los methodos aritmeticos SON SEIS
	this->methods["print"] = &BoolObject::print;
	this->methods["clone"] = &BoolObject::clone;
}

BoolObject::~BoolObject() {
}

ObjectMasCapo* BoolObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
//	std::cout << "print " << value << std::endl;
	if (value){
		std::cout << "true";
	}else{
		std::cout << "false";
	}
	return this;
}
ObjectMasCapo* BoolObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "clone" << std::endl;
	return this;
}

ObjectMasCapo* BoolObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}
