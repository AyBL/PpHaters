#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#include <vector>
#include <map>

#include "server_ObjectMasCapo.h"
#include "server_NumberObject.h"
#include "server_CustomObject.h"
#include "server_BoolObject.h"
#include "server_StringObject.h"

class VirtualMachine {
private:
	std::string name;
	ObjectMasCapo* lobby;
//	 CustomObeject lobby; ???? asi que onda???
	std::vector<ObjectMasCapo*> objectsCreated;
//	enum TipoSlot {
//		ARG, PADRE, NORMAL
//	};

public:
	VirtualMachine(std::string name="lobby1");
	virtual ~VirtualMachine();
	void collect();  // falata ver que onda

	// getName para seleccionar la salas despues

	//---- message---//
	ObjectMasCapo* lookup(std::string slotName);
	ObjectMasCapo* message(ObjectMasCapo* receiver, std::string method,
			std::map<std::string, ObjectMasCapo*> argument);
	//---------------//

	CustomObject* createObject(std::string name); // customObject
	StringObject* createObject(std::string name, std::string value);
	NumberObject* createObject(std::string name, double value);
	BoolObject* createObject(std::string name, bool value);

};

#endif /* VIRTUALMACHINE_H_ */
