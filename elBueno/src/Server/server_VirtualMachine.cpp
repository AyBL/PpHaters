#include "server_VirtualMachine.h"
#include <iostream>
#include <sstream>

#include "Parser.h"

#define DELIMITER ':'

void VirtualMachine::connectNewClient(ProxyCliente *newClient) {
//	std::cout<<"se agregara "<<connectedClients.size()<<std::endl;
	connectedClients.push_back(newClient);
	std::cout<<"se agrego proxy n:  "<<connectedClients.size()<<std::endl;
}
ProxyCliente* VirtualMachine::getClient(unsigned indexClient) const{
	if (indexClient >= connectedClients.size()) return NULL;
	return connectedClients[indexClient];
}

unsigned VirtualMachine::numberOfUsers() const{
	return connectedClients.size();
}

VirtualMachine::VirtualMachine(std::string name) :
		name(name) {
	lobby = createObject("lobby");
	context = lobby;
}

VirtualMachine::~VirtualMachine() {

	std::cout << "Objects Created size: " << objectsCreated.size() << std::endl;

	for (unsigned i = 1; i < objectsCreated.size(); ++i) {

		std::cout<<"Entra a for en destr de VM, al obj: "<< objectsCreated[i]<<std::endl;
		std::cout<<"Entra a for en destr de VM, al obj con nombre: "<< objectsCreated[i]->getName()<<std::endl;

		if ((objectsCreated[i]!= NULL) && (objectsCreated[i]->isTemp())) {

			std::cout<<"Entra a objectsCreated, al obj: "<< objectsCreated[i]->getName()<<std::endl;
			std::cout << "En iteracion: " << i << std::endl;

			delete objectsCreated[i];
		}
	}
//	std::cout << "se destruyo Obj" << std::endl;
	if (lobby != NULL) {
		delete lobby; // si lo paso a stack no es necesario
	}
}

VirtualMachine::VirtualMachine(VirtualMachine&& other) {
	this->name = std::move(other.name);
	this->lobby = std::move(other.lobby);
	this->context = std::move(other.context);
	this->objectsCreated = std::move(other.objectsCreated);
	other.lobby =NULL;
	other.context =NULL;
}

VirtualMachine& VirtualMachine::operator=(VirtualMachine&& other) {
	this->name = std::move(other.name);
	this->lobby = std::move(other.lobby);
	this->context = std::move(other.context);
	this->objectsCreated = std::move(other.objectsCreated);
	other.lobby =NULL;
	other.context =NULL;
	return *this;
}


void VirtualMachine::collect() {
//	for (unsigned i = 0; i < objectsCreated.size(); ++i) {
//		if (objectsCreated[i] != NULL && objectsCreated[i]->isTemp()) {
//			delete objectsCreated[i];
//		}
//	}
}

std::string VirtualMachine::getName() {
	return name;
}

CustomObject* VirtualMachine::getLobby() const{
	return this->lobby;
}

//---------------------------------------------------------
// ver si se puede hacer una clase creadora para los chequeos de que salga
// correcto el new  onda aplicar RAII y despues mandar todo por movimiento
// o es batir fruta fuerte.

CustomObject* VirtualMachine::createObject(std::string name) {
//	std::cout << "se creo Obj" << std::endl;
	CustomObject* newObject = new CustomObject(name);
	objectsCreated.push_back(newObject);
	return newObject;
}

NilObject* VirtualMachine::createNilObject(const std::string &name){
	NilObject* newObject = new NilObject(name);
	objectsCreated.push_back(newObject);
	return newObject;
}

NumberObject* VirtualMachine::createObject(std::string name, double value) {
//	std::cout << "se creo Obj" << std::endl;
	NumberObject* newObject = new NumberObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}

StringObject* VirtualMachine::createObject(std::string name,
		std::string value) {
//	std::cout << "se creo Obj" << std::endl;
	StringObject* newObject = new StringObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}

BoolObject* VirtualMachine::createObject(std::string name, bool value) {
//	std::cout << "se creo Obj" << std::endl;
	BoolObject* newObject = new BoolObject(name, value);
	objectsCreated.push_back(newObject);
	return newObject;
}

void VirtualMachine::appendObject(ObjectMasCapo* obj){
	objectsCreated.push_back(obj);
}

ObjectMasCapo* VirtualMachine::cloneObject(ObjectMasCapo* obj,
		std::map<std::string, ObjectMasCapo*> arguments){
	ObjectMasCapo* cloned = obj->clone(arguments);
	//Primero se appendean los slots y luego el objeto contenedor para no
	//liberar dos veces

	std::cout << "Objects created size ANTES de appendear clonados: " << objectsCreated.size() << std::endl;

	std::vector<std::string> clonedIndex = cloned->getIndex();

	std::cout << "ClonedIndex size: " << clonedIndex.size() << std::endl;

	if (clonedIndex.size() != 0){
		for (int i = 0; i < clonedIndex.size(); ++i){
			//Deberia appendear los slots de los slots
			this->appendObject(cloned->lookup(clonedIndex[i]));
		}
	}
	this->appendObject(cloned);

	std::cout << "Objects created size DESPUES de appendear clonados: " << objectsCreated.size() << std::endl;

	return cloned;
}

//-----------------------------------------------------------

ObjectMasCapo* VirtualMachine::lookup(std::string slotName) {
	std::cout<<"Virtual machine lookup buscando: "<<slotName<<std::endl;
	// tengo que evaluar el context
	if ((context->getName() == slotName) || (slotName == "self")) {
		return context;
	} else {
		return context->lookup(slotName);
	}

}
void VirtualMachine::changeArgs(bool toNil, CustomObject* &msgObj,
		std::string msgName, std::map<std::string, ObjectMasCapo*> &argument) {
	size_t pos;
	std::string argName;
	ObjectMasCapo* arg;
	int i = 0;
	//Primero spliteo el string para obtener el orden de los argumentos
	while ((pos = msgName.find(DELIMITER)) != std::string::npos){
		argName = msgName.substr(0, pos);

		std::cout << "Arg salido del split del nombre: " << argName
				<< std::endl;

		msgName.erase(0, pos + 1);
		if (!toNil){
			arg = argument.at(argName);
			std::cout << "Arg sacado del map de argumentos: " << arg << std::endl;
		}else{
//			arg = new NilObject("");
//			this->appendObject(arg);
			arg = this->createNilObject("");
		}
		msgObj->changeSlot(msgObj->getElementInIndexAt(i), arg);
		++i;
	}
}

bool VirtualMachine::assignmentMessage(const std::string &slotName,
		ObjectMasCapo *newSlot){
	return this->context->changeSlot(slotName, newSlot);
}

ObjectMasCapo* VirtualMachine::message(ObjectMasCapo* receiver,
		std::string method, std::map<std::string, ObjectMasCapo*> argument) {
	if (receiver == NULL){
		std::cout << "Receiver es null" << std::endl;

		receiver = this->context;
		std::cout << "El contexto es: " << context->getName() << std::endl;
	}

	std::cout << "El metodo es " << method << std::endl;

	ObjectMasCapo* aux = receiver->lookup(method);
	if (aux != NULL) {

		std::cout << "Aux: " << aux->getName() << std::endl;

		if (aux->getCode() != "") {
			//Un objeto que tenga un slot metodo con codigo es un CustomObject
			CustomObject *auxCustom = static_cast<CustomObject*>(aux);
			if (!argument.empty()){
				//Este metodo deberia aplicarse para cualquier obj q tenga slots args
				this->changeArgs(false, auxCustom, method, argument);
			}

			ObjectMasCapo* receiverAsParent = this->cloneObject(receiver, argument);

			//Seteo el receiver como padre del objeto mensaje
			ObjectMasCapo *addedObj = auxCustom->addSlot("self", receiverAsParent, 'P');
			this->appendObject(addedObj);
//			context = static_cast<CustomObject*>(receiver);
			context = auxCustom;
			std::stringstream code(auxCustom->getCode());
//			std::cout<< "llama a parser con cod:  "<<aux->getCode() <<std::endl;
			Parser parserCode(&code, *this);
			ObjectMasCapo* obj = parserCode.script();

			auxCustom->removeSlot("self");

			if (!argument.empty()){
				this->changeArgs(true, auxCustom, method, argument);
			}
			context = lobby;
			return obj; // ????
		} else {
//			return NULL;  //ERROR el metodo no es ejecutable
			// mm maybe para el cli, si no es ejec devolver el valor
			// no se si como nuevo objeto o que aun VER
			//return receiver;
			//Sofi cambia receiver por aux
			return aux;
		}
	} else {
//		std::cout<<"no encontrado "<<std::endl;
		if (method == "clone"){
			receiver = this->cloneObject(receiver, argument);
		}else if (method == "self"){
			return receiver;
		}else{
			receiver = receiver->execute(method, argument);
		}
		return receiver;
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Json::Value VirtualMachine::toJson(){
	Json::Value value(Json::objectValue);
	value["name"] = this->name;
	value["lobby"] = this->lobby->toJson();

	std::cout << "Jsonea lobby" << std::endl;

	return value;
}

void VirtualMachine::JsonSave(const std::string &nombreArch){
	std::ofstream out(nombreArch, std::ofstream::out);
	Json::Value obj_json(Json::objectValue), contacts_json(Json::arrayValue);

	obj_json["VM"] = this->toJson();

	out << obj_json;

	out.close();
}

CustomObject* VirtualMachine::fromJsonToCustom(CustomObject* &obj, Json::Value customObjJson){
	Json::Value slots_json;

	obj->addCode(customObjJson["code"].asString());
	obj->setFlags(customObjJson["slotType"].asString()[0]);
	slots_json = customObjJson["slots"];

	for (Json::Value::iterator it = slots_json.begin(); it != slots_json.end(); ++it){
		obj->addSlot((*it)["name"].asString(), this->fromJson(*it),
				(*it)["slotType"].asString()[0]);
	}
	return obj;
}

CustomObject* VirtualMachine::fromJsonToCustom(Json::Value customObjJson){
	Json::Value slots_json;
	char tipoSlot;

	CustomObject *obj = this->createObject(customObjJson["name"].asString());

	obj->addCode(customObjJson["code"].asString());
	obj->setFlags(customObjJson["slotType"].asString()[0]);
	obj->setPosition(customObjJson["pos x"].asInt(), customObjJson["pos y"].asInt());
	slots_json = customObjJson["slots"];

	for (Json::Value::iterator it = slots_json.begin(); it != slots_json.end(); ++it){
		tipoSlot = (*it)["slotType"].asString()[0];
		if (tipoSlot != 'P')
			obj->addSlot((*it)["name"].asString(), this->fromJson(*it),
					tipoSlot);
		else{
			obj->addSlot((*it)["name"].asString(), this->fromJsonToPadre(*it),
					tipoSlot);
		}
	}
	return obj;
}

ObjectMasCapo* VirtualMachine::fromJsonToNumber(Json::Value objJson){
	NumberObject *obj = this->createObject(objJson["name"].asString(),
			objJson["value"].asDouble());
	obj->setFlags(objJson["slotType"].asString()[0]);
	return obj;
}

ObjectMasCapo* VirtualMachine::fromJsonToString(Json::Value objJson){
	StringObject *obj = this->createObject(objJson["name"].asString(),
			objJson["value"].asString());
	obj->setFlags(objJson["slotType"].asString()[0]);
	return obj;
}

ObjectMasCapo* VirtualMachine::fromJsonToBool(Json::Value objJson){
	BoolObject *obj = this->createObject(objJson["name"].asString(),
			objJson["value"].asBool());
	obj->setFlags(objJson["slotType"].asString()[0]);
	return obj;
}

//ESTE METODO ES PARA QUE CUANDO EL SLOT SEA PADRE, NO SE CREE UN NUEVO OBJ SI
//NO QUE SE LE ASIGNE UN PUNTERO AL OBJ YA EXISTENTE
ObjectMasCapo* VirtualMachine::fromJsonToPadre(Json::Value customObjJson){
	Json::Value slots_json;
	ObjectMasCapo *padre;
	char tipoSlot;
//	CustomObject *obj = this->createObject(customObjJson["name"].asString());

	std::cout << "Deberia ser pepe: " << customObjJson["name"].asString() << std::endl;

//	obj->addCode(customObjJson["code"].asString());
//	obj->setFlags(customObjJson["slotType"].asString()[0]);
	slots_json = customObjJson["slots"];

	for (Json::Value::iterator it = slots_json.begin(); it != slots_json.end(); ++it){
		//VER SI HAY QUE HACER LOOKUP POR EL CONTEXTO
		padre = this->lobby->lookup((*it)["name"].asString());

//		std::cout << "Deberian ser iguales: " << (*it)["name"].asString() << " y "
//				<< padre->getName() << std::endl;

//		obj->addSlot(padre->getName(), padre,
//				(*it)["slotType"].asString()[0]);
	}
	return padre;
}

ObjectMasCapo* VirtualMachine::fromJson(Json::Value objJson){
	ObjectMasCapo *obj;
	std::string str = objJson["type"].asString();
	if (str == "Custom")
		obj = this->fromJsonToCustom(objJson);
	else if (str == "Number")
		obj = this->fromJsonToNumber(objJson);
	else if (str == "String")
		obj = this->fromJsonToString(objJson);
	else
		obj = this->fromJsonToBool(objJson);

	return obj;
}

void VirtualMachine::JsonLoad(const std::string &nombreArch) {
	std::ifstream in(nombreArch);
	Json::Value objJson;
	in >> objJson;

	this->fromJsonToCustom(this->lobby, objJson["VM"]["lobby"]);

	//PARA CUANDO TENGAMOS VECTOR DE VMS
//	for (Json::Value::iterator it = obj_json.begin(); it != obj_json.end(); ++it) {
//		this->lobby = this->fromJsonToCustom(*it);
//		std::cout << "Supera lobby" << std::endl;
//	}
	in.close();
}
