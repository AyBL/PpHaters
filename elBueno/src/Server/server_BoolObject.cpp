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
	std::cout << "Bool clone" << std::endl;
	BoolObject *newObj = new BoolObject(this->getName(), this->value);
	return newObj;
}

ObjectMasCapo* BoolObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}

std::string BoolObject::getValue() {
	if (value) {
		return "true";
	} else {
		return "false";
	}
}


Json::Value BoolObject::toJson(){
	
	Json::Value value(Json::objectValue);
	value["type"] = "Bool";
	value["name"] = this->getName();
	value["value"] = this->value;

	std::string str = std::string(1, this->getFlag());
	value["slotType"] = str;

	return value;
}
