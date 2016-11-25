#include "server_NumberObject.h"
#include  "server_BoolObject.h"
#include <iostream>
#include <fstream>


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

std::string NumberObject::getValue(){
	std::string f_str = std::to_string(value);
	return f_str;
}

ObjectMasCapo* NumberObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << value << std::endl;
	return this;
}
ObjectMasCapo* NumberObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "NumberObj clone" << std::endl;
	NumberObject *newObj = new NumberObject(this->getName(), this->value);
	return newObj;
}

//--------------------------- OPERACIONES----------------------------------
ObjectMasCapo* NumberObject::suma(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
//	value = value + (other->value);
//	return this;
	double resultado = value + (other->value);
	return (new NumberObject("", resultado));
}
ObjectMasCapo* NumberObject::resta(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	double resultado = value - (other->value);
	return (new NumberObject("", resultado));
}
ObjectMasCapo* NumberObject::multiplicacion(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	double resultado = value * (other->value);
	return (new NumberObject("", resultado));
}
ObjectMasCapo* NumberObject::division(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();
	NumberObject *other = static_cast<NumberObject*>(it->second);
	double resultado = value / (other->value);
	return (new NumberObject("", resultado));
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

Json::Value NumberObject::toJson(){
	Json::Value value(Json::objectValue);
	value["type"] = "Number";
	value["name"] = this->getName();
	value["value"] = this->value;

	std::string str = std::string(1, this->getFlag());
	value["slotType"] = str;

	return value;
}

