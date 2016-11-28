#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#include <vector>
#include <map>
#include <mutex>

#include "server_ObjectMasCapo.h"
#include "server_NumberObject.h"
#include "server_CustomObject.h"
#include "server_NilObject.h"
#include "server_BoolObject.h"
#include "server_StringObject.h"

#include "server_ProxyCliente.h"

// LOCKEAR ESTO PARA QUE NO HAYA CONFLICTOS ENTRE CLIENTES

class VirtualMachine {
private:
	std::string name;
	CustomObject *lobby;
	CustomObject *context;
	std::vector<ObjectMasCapo*> objectsCreated;
	std::vector<ProxyCliente*> connectedClients;
	std::mutex vmMutex;

public:
	VirtualMachine(std::string name = "lobby1");
	virtual ~VirtualMachine();
	void collect(); // falata ver que onda

	void connectNewClient(ProxyCliente *newClient);
	ProxyCliente* getClient(unsigned indexClient) const;
	unsigned numberOfUsers() const;

	void lock();
	void unlock();

	std::string getName(); // para seleccionar la salas despues

	//---- message---//
	ObjectMasCapo* lookup(std::string slotName);
	void changeArgs(bool toNil, CustomObject* &msgObj, std::string msgName,
			std::map<std::string, ObjectMasCapo*> &argument);
	bool assignmentMessage(const std::string &slotName, ObjectMasCapo *newSlot);
	ObjectMasCapo* message(ObjectMasCapo* receiver, std::string method,
			std::map<std::string, ObjectMasCapo*> argument);
	//---------------//

	CustomObject* createObject(std::string name); // customObject
	NilObject* createNilObject(const std::string &name);
	StringObject* createObject(std::string name, std::string value);
	NumberObject* createObject(std::string name, double value);
	BoolObject* createObject(std::string name, bool value);
	void appendObject(ObjectMasCapo* obj);
	void appendSlots(ObjectMasCapo *obj);
	ObjectMasCapo* cloneObject(ObjectMasCapo* obj,
			std::map<std::string, ObjectMasCapo*> arguments);

	VirtualMachine(const VirtualMachine&) = delete;

	VirtualMachine& operator=(const VirtualMachine&) = delete;

	VirtualMachine(VirtualMachine&& other);

	VirtualMachine& operator=(VirtualMachine&& other);

	CustomObject* getLobby() const;

	//PARA SERIALIZACION
	Json::Value toJson();
	void JsonSave(const std::string &nombreArch);
	ObjectMasCapo* fromJson(Json::Value objJson);
	CustomObject* fromJsonToCustom(CustomObject* &obj, Json::Value customObj_json);
	CustomObject* fromJsonToCustom(Json::Value customObj_json);
	ObjectMasCapo* fromJsonToNumber(Json::Value obj_json);
	ObjectMasCapo* fromJsonToString(Json::Value obj_json);
	ObjectMasCapo* fromJsonToBool(Json::Value objJson);
	ObjectMasCapo* fromJsonToPadre(Json::Value customObjJson);
	void JsonLoad(const std::string &nombreArch);

};

#endif /* VIRTUALMACHINE_H_ */
