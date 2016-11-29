/*
 * server_NilObject.cpp
 *
 *  Created on: 17/11/2016
 *      Author: sofia
 */

#include "server_NilObject.h"
#include <iostream>

NilObject::NilObject(const std::string &name): ObjectMasCapo(name) {
//	this->value = "nil";
	methods["print"] = &NilObject::print;
	methods["clone"] = &NilObject::clone;
}

NilObject::~NilObject() {
}

std::string NilObject::getValue(){
	return "nil"; // NO SE SI ESTO O STRING VACIO
}

ObjectMasCapo* NilObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << value;
	return this;
}

ObjectMasCapo* NilObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	NilObject *newObj = new NilObject(this->getName());
	return newObj;
}

ObjectMasCapo* NilObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	MFP fp = methods[method];
	return (this->*fp)(arguments);
}

Json::Value NilObject::toJson(){
	Json::Value value(Json::objectValue);
	value["type"] = "Nil";
	value["name"] = this->getName();
	value["value"] = this->value;
	std::string str = std::string(1, this->getFlag());
	value["slotType"] = str;
	return value;
}

