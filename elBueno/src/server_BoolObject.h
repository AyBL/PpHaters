#ifndef SERVER_BOOLOBJECT_H_
#define SERVER_BOOLOBJECT_H_

#include <string>

#include "server_ObjectMasCapo.h"

class BoolObject:public ObjectMasCapo {
private:
	bool value;
	typedef ObjectMasCapo* (BoolObject::*MFP)(
			std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;
public:
	BoolObject(std::string name, bool value);
	virtual ~BoolObject();
	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);

	ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments);
};

#endif /* SERVER_BOOLOBJECT_H_ */
