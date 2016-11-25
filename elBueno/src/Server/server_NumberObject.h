#ifndef SERVER_NUMBEROBJECT_H_
#define SERVER_NUMBEROBJECT_H_

#include <string>

#include "server_ObjectMasCapo.h"
#include "json/json.h"

class NumberObject:public ObjectMasCapo {
private:
	double value;
	typedef ObjectMasCapo* (NumberObject::*MFP)(std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;
public:
	NumberObject(std::string name, double value);
	~NumberObject();
	std::string getValue();
	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);

	//--------OPERACIONES---------
	ObjectMasCapo* suma(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* resta(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* multiplicacion(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* division(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* distinto(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* igual(std::map<std::string, ObjectMasCapo*> arguments);
	//------------------------------

	ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments);

	//---------PARA SERIALIZACION-------
	Json::Value toJson();
	void JsonSave();
};

#endif /* SERVER_NUMBEROBJECT_H_ */
