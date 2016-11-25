#include "server_StringObject.h"
#include <iostream>

StringObject::StringObject(std::string name, std::string value) :
		ObjectMasCapo(name), value(value) {
	methods["print"] = &StringObject::print;
	methods["clone"] = &StringObject::clone;
}

StringObject::~StringObject() {
}

ObjectMasCapo* StringObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << value;
	return this;
}

ObjectMasCapo* StringObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "String clone" << std::endl;
	StringObject *newObj = new StringObject(this->getName(), this->value);
	return newObj;
}

ObjectMasCapo* StringObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}

std::string StringObject::getValue(){
	return value;
}


Json::Value StringObject::toJson(){
	Json::Value value(Json::objectValue);
	value["type"] = "String";
	value["name"] = this->getName();
	value["value"] = this->value;
	std::string str = std::string(1, this->getFlag());
	value["slotType"] = str;
	return value;
}
