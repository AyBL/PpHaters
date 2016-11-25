#ifndef CUSTOMOBJECT_H_
#define CUSTOMOBJECT_H_

//#include <map>
#include <vector>
#include <string>

#include "server_ObjectMasCapo.h"

class CustomObject: public ObjectMasCapo {
private:
//	ObjectMasCapo *self;
	std::vector<ObjectMasCapo*> slots;
	typedef ObjectMasCapo* (CustomObject::*MFP)(
			std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;

	bool nameRepeats(std::string name);
public:
	CustomObject(std::string name = "customObject");
	virtual ~CustomObject();

	void addSlot(std::string slotName,ObjectMasCapo* newSlot,char flag);
	ObjectMasCapo* addSlot(std::map<std::string, ObjectMasCapo*> arguments);
	void removeSlot(ObjectMasCapo* newSlot);
	void removeSlot(std::string slotName);
	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);

	void addCode(std::string newCode);

	ObjectMasCapo* lookup(std::string slotName);

	ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments);

};

#endif /* CUSTOMOBJECT_H_ */
