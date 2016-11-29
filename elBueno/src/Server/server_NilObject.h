/*
 * server_NilObject.h
 *
 *  Created on: 17/11/2016
 *      Author: sofia
 */

#ifndef SERVER_NILOBJECT_H_
#define SERVER_NILOBJECT_H_

#include "server_ObjectMasCapo.h"

class NilObject: public ObjectMasCapo {
private:
	std::string value = "nil";
	typedef ObjectMasCapo* (NilObject::*MFP)(std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;
public:
	NilObject(const std::string &name);
	~NilObject();
	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);
	std::string getValue();

	ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments);

	//PARA SERIALIZACION
	Json::Value toJson();
};

#endif /* SERVER_NILOBJECT_H_ */
