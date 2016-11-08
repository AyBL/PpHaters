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
	std::cout << "clone" << std::endl;
	return this;
}

ObjectMasCapo* StringObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}
