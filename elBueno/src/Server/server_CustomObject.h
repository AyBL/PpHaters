#ifndef CUSTOMOBJECT_H_
#define CUSTOMOBJECT_H_

//#include <map>
#include <vector>
#include <tuple>
#include <string>

#include "server_ObjectMasCapo.h"

class CustomObject: public ObjectMasCapo {
private:
	std::map<std::string, ObjectMasCapo*> slots;
	std::vector<std::string> index;
	typedef ObjectMasCapo* (CustomObject::*MFP)(
			std::map<std::string, ObjectMasCapo*>);
	std::map<std::string, MFP> methods;
	int positionX;
	int positionY;
	bool nameRepeats(std::string name);


public:
	CustomObject(std::string name = "customObject");
	virtual ~CustomObject();
	std::string getElementInIndexAt(size_t pos);
	std::vector<std::string> getIndex();
	std::string getValue();
	ObjectMasCapo* addSlot(std::string slotName,ObjectMasCapo* newSlot,char flag);
	ObjectMasCapo* addSlot(std::map<std::string, ObjectMasCapo*> arguments);
	void removeSlot(ObjectMasCapo* newSlot);
	void removeSlot(const std::string &slotName);
	bool changeSlot(const std::string &slotName,
			ObjectMasCapo* newSlot);
	ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arguments);
	ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arguments);

	void addCode(std::string newCode);

	ObjectMasCapo* lookup(std::string slotName);

	ObjectMasCapo* execute(std::string method, std::map<std::string,
			ObjectMasCapo*> arguments);

	int getPositionX() const;
	int getPositionY() const;
	void setPosition(int posX, int posY);
	
	// asco pero bueno
	ObjectMasCapo* getSlot(int slotIndex=-1);

	
	//PARA SERIALIZACION
	Json::Value toJson();

};

#endif /* CUSTOMOBJECT_H_ */
