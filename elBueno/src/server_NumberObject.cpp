#include "server_NumberObject.h"
#include  "server_BoolObject.h"
#include <iostream>


NumberObject::NumberObject(std::string name, double value) :
		ObjectMasCapo(name), value(value) {
	methods["print"] = &NumberObject::print;
	methods["clone"] = &NumberObject::clone;
	methods["suma"] = &NumberObject::suma;
	methods["resta"] = &NumberObject::resta;
	methods["multiplicacion"] = &NumberObject::multiplicacion;
	methods["division"] = &NumberObject::division;
	methods["igual"] = &NumberObject::igual;
	methods["distinto"] = &NumberObject::distinto;
	methods["plus"] = methods["+"] = methods["suma"];
	methods["minus"] = methods["-"] = methods["resta"];
	methods["times"] = methods["*"] = methods["multiplicacion"];
	methods["into"] = methods["/"] = methods["division"];
	methods["equal"] = methods["=="] = methods["igual"];
	methods["notequal"] = methods["!="] = methods["distinto"];
}

NumberObject::~NumberObject() {
}

ObjectMasCapo* NumberObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "print " << value << std::endl;
	return this;
}
ObjectMasCapo* NumberObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "clone" << std::endl;
	return this;
}

//--------------------------- OPERACIONES----------------------------------
ObjectMasCapo* NumberObject::suma(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	value = value + (other->value);
	return this;
}
ObjectMasCapo* NumberObject::resta(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	value = value - (other->value);
	return this;
}
ObjectMasCapo* NumberObject::multiplicacion(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	value = value * (other->value);
	return this;
}
ObjectMasCapo* NumberObject::division(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	value = value / (other->value);
	return this;
}
ObjectMasCapo* NumberObject::distinto(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	bool resultado = (value != other->value);
	return (new BoolObject("", resultado));
}
ObjectMasCapo* NumberObject::igual(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	bool resultado = (value == other->value);
	return (new BoolObject("", resultado));
}
//-------------------------------------------------------------------------------

ObjectMasCapo* NumberObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}
