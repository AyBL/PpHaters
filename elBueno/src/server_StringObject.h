#ifndef SERVER_STRINGOBJECT_H_
#define SERVER_STRINGOBJECT_H_

#include <string>
#include "server_ObjectMasCapo.h"

class StringObject:public ObjectMasCapo {
private:
	std::string value;
	typedef ObjectMasCapo* (StringObject::*MFP)(std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;
public:
	StringObject(std::string name, std::string value);
	virtual ~StringObject();

	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);

	ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments);

};

#endif /* SERVER_STRINGOBJECT_H_ */
