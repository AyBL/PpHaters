#include "server_CustomObject.h"
#include <iostream> // para el print
#define LOG(str) std::cout<< __FILE__ ":" << __LINE__ << ": " << str << std::endl
CustomObject::CustomObject(std::string name) :
		ObjectMasCapo(name), positionX(0), positionY(0) {
	methods["print"] = &CustomObject::print;
	methods["clone"] = &CustomObject::clone;
	methods["_addSlot:"] = &CustomObject::addSlot;
	methods["_RemoveSlots:"] = &CustomObject::removeSlot;
}

CustomObject::~CustomObject() {

	std::cout << "Entra a destructor Custom de puntero: " << this << std::endl;

	std::cout << "Entra a destructor Custom de " << this->getName()
			<< " con slot size: " << slots.size() << std::endl;
	if (!(this->isParent())) {

		std::cout << "Entra a que: " << this->getName() << " no es parent"
				<< std::endl;

//		for(auto const &it : slots){
//			if (it.second != NULL){
////				delete it.second;
//				it.second->yesTemp();
//			}
//		}
		this->tempEverySlot();
	} else {

		std::cout << "Entra a que: " << this->getName() << " ES parent"
				<< std::endl;

		//Si es padre solo tiene un slot
		this->slots.begin()->second = NULL;
//		this->slots[0] = NULL;
	}
}

void CustomObject::tempEverySlot() {
	for (auto &it : slots) {
		if (it.second != NULL) {

			std::cout << "Entra al slot: " << it.second->getName() << std::endl;

			if ((it.second->getIndex().size() != 0)
					&& (it.second->getFlag() != 'P'))
				static_cast<CustomObject*>(it.second)->tempEverySlot();

			it.second->yesTemp();
			it.second = NULL;
		}
	}
}

bool CustomObject::nameRepeats(std::string name) {
	if (slots.find(name) != slots.end())
		return true;
	else
		return false;
//	unsigned i = 0;
//	while (i < slots.size()) {
//		if (slots[i]->getName() == name) {
//			return true;
//		}
//		++i;
//	}
}

std::string CustomObject::getElementInIndexAt(size_t pos) {
	return this->index[pos];
}

std::vector<std::string> CustomObject::getIndex() {
	return this->index;
}

//METODO
ObjectMasCapo* CustomObject::addSlot(std::string slotName,
		ObjectMasCapo* newSlot, char flag) {
//	std::cout<<"se agrego slot "<<slotName<<std::endl;
	if (nameRepeats(slotName)) {
		std::cout << "ERROR nombre repetido " << std::endl;
		return NULL;
	}
	if (flag == 'P') {

		CustomObject *slotParent = new CustomObject(slotName);
		slotParent->notTemp();

		std::cout << "Slot parent: " << slotParent << std::endl;

//		CustomObject *other = static_cast<CustomObject*>(newSlot);

//VER SI BIEN
		slotParent->slots[newSlot->getName()] = newSlot;
		slotParent->index.push_back(newSlot->getName());
//		slotParent->slots.push_back(newSlot);

		slotParent->setFlags(flag);
		this->slots[slotName] = slotParent;
		this->index.push_back(slotName);
//		this->slots.push_back(slotParent);
		return slotParent;
	} else {
		newSlot->rename(slotName);
		newSlot->setFlags(flag);
		newSlot->notTemp();
		this->slots[slotName] = newSlot;
		this->index.push_back(slotName);
//		this->slots.push_back(newSlot);
		return newSlot;
	}
}
//PARA LOS MENSAJES
ObjectMasCapo* CustomObject::addSlot(
		std::map<std::string, ObjectMasCapo*> arguments) {
	ObjectMasCapo *ultimo = NULL;  // ACA PONDRIA UN NULL PARA INICIALIZAR XQ SINO [-Werror=maybe-uninitialized]

	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();

	CustomObject *other = static_cast<CustomObject*>(it->second);
	for (auto &it : other->slots) {
		if (nameRepeats(it.second->getName())) {
			std::cout << "ERROR: ya existe un slot llamado: "
					<< it.second->getName() << std::endl;
			return NULL;
		}
		it.second->notTemp();
		this->slots[it.second->getName()] = it.second;
		this->index.push_back(it.second->getName());
		ultimo = it.second;
		it.second = NULL;
	}
//	for (unsigned i = 0; i < other->slots.size(); ++i) {
//		other->slots[i]->notTemp();
//
//		this->slots.push_back(other->slots[i]);
//		other->slots[i] = NULL;
//
//		// aca deberia moverlos no copiarlos
//		// o bien hacer que despues apunte a null
//		// porque cuando el garbarge pase y lo elimine le va a eliminar los
//		// los slot y NO QUEREMOS ESO
////		std::cout<<"se agrega el slot "<<std::endl;
//	}
//	delete other;
	return ultimo;
//	return slots[slots.size() - 1];
}

ObjectMasCapo* CustomObject::removeSlot(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::map<std::string, ObjectMasCapo*>::iterator it = arguments.begin();

	CustomObject *other = static_cast<CustomObject*>(it->second);
	for (auto &it : other->slots) {
		this->removeSlot(it.second->getName());

		//Ver si necesario
//		it.second->yesTemp();
//		it.second = NULL;
	}
	//Cambiar
	return this;
}

void CustomObject::removeSlot(const std::string &slotName) {
	//VER SI FUNCIONA
//	delete this->slots.at(slotName);

//Lo pongo como temp asi lo borra el garbage collector
//Si no existe el slot, .at lanza una excepcion
	this->slots.at(slotName)->yesTemp();

	std::cout << "Antes de remover tamaño index: " << this->index.size()
			<< std::endl;
	std::cout << "-----SLOT A REMOVER: " << this->slots.at(slotName)->getName()
			<< std::endl;

	this->slots.erase(slotName);

	std::vector<std::string>::iterator it;
	it = index.begin();
	std::string s = (*it);
	while (s != slotName && it != index.end()) {
		++it;
		s = (*it);
	}
	index.erase(it);

	std::cout << "Despues de remover tamaño index: " << this->index.size()
			<< std::endl;

//    if (slots.size() > 0) {
//        std::vector<ObjectMasCapo*>::iterator it;
//        it=slots.begin();
//        std::string s=(*it)->getName();
//        while (s != slotName && it != slots.end()) {
//            ++it;
//            s =(*it)->getName();
//        }
//        slots.erase(it);
//    }
}

bool CustomObject::changeSlot(const std::string &slotName,
		ObjectMasCapo* newSlot) {
	//VALIDAR QUE EXISTA EL SLOT
	if (!nameRepeats(slotName)) {
		std::cout << "ERROR slot no existente " << std::endl;
		return false;
	}
	std::cout << "Entra a changeSlot a cambiar: " << slotName << std::endl;

	CustomObject *container;
	ObjectMasCapo *slotToChange = this->lookup(slotName, container);

	std::cout << "SlotToChange: " << slotToChange->getName() << std::endl;
	std::cout << "Container: " << container->getName() << std::endl;

	if ((slotToChange->getFlag() != 'I') && (slotToChange->getFlag() != 'P')) {
		newSlot->rename(slotName);
		newSlot->notTemp();
//		this->slots[slotName] = newSlot;
		container->slots[slotName] = newSlot;
		//Agregar a vector objects created?
		//Marco el objeto a reemplazar como temp para que sea eliminado despues
		//por el garbage collector

		slotToChange->yesTemp();

		std::cout << "Slot nuevo: " << container->slots.at(slotName)->getName()
				<< std::endl;

		return true;
	} else {
		std::cout << "El slot no se puede modificar ya que es inmutable"
				<< std::endl;
		//Excepcion?
		return false;
	}
}

bool CustomObject::renameSlot(const std::string &oldName,
		const std::string &newName) {
	if (slots.find(newName) != slots.end()) {
		return false;
	} //Asumo que ya se chequeo que exista el slot al que se le va a cambiar el nombre
	slots[newName] = slots.at(oldName);
	slots.at(oldName)->rename(newName);
	slots.erase(oldName);
	std::vector<std::string>::iterator it;
	it = index.begin();
	std::string s = (*it);
	while (s != oldName && it != index.end()) {
		++it;
		s = (*it);
	}
	*it = newName;
	return true;
}

ObjectMasCapo* CustomObject::clone(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "CustomObject clone" << std::endl;
	CustomObject *newObj = new CustomObject(this->getName());
//	newObj->index = this->index;
	newObj->addCode(this->getCode());
	//Ver si ponerle la misma pos o ponerle 0
	newObj->setPosition(this->getPositionX(), this->getPositionY());
	newObj->setFlags(this->getFlag());
	if (this->getFlag() != 'P') {
		for (auto const &it : this->slots) {
//			newObj->slots[it.first] = it.second->clone(arguments);
			if (it.second->getFlag() != 'P')
				newObj->addSlot(it.first, it.second->clone(arguments),
						it.second->getFlag());
			else {
				//Como addSlot para los objs padres les hace un obj contenedor
				//y aca ya esta hecho, directamente agrego
				newObj->slots[it.first] = it.second->clone(arguments);
				newObj->slots[it.first]->notTemp();
				newObj->index.push_back(it.first);
			}
		}
	} else {
		for (auto const &it : this->slots) {
			//Le asigno el puntero que apunta al padre
			//Ver si se libera bien o deberia usar addSLot
			newObj->slots[it.first] = it.second;
		}
	}
	return newObj;
}

ObjectMasCapo* CustomObject::print(
		std::map<std::string, ObjectMasCapo*> arguments) {
	std::cout << "CustomObject Print" << std::endl;
//	for (unsigned i=0;i<slots.size();++i){
//		slots[i]->print(arguments);
//	}
	return this; /// no se que deberia hacer este print
}

void CustomObject::addCode(std::string newCode) {
	this->code = newCode;
}

ObjectMasCapo* CustomObject::lookup(const std::string &slotName,
		CustomObject* &container) {
// POR AHORA SOLO RETONA SI ESTA DENTRO DEL EL QUE LO LLAMA
// ME FALTA HACERLO RECUSIVO CON LOS PARENT
	std::vector<CustomObject*> parent;
	if (slots.find(slotName) != slots.end()) {
		container = this;

		std::cout << "En lookup encuentra: " << slots.at(slotName) << " para "
				<< slotName << std::endl;

		return slots.at(slotName);
	} else {
		for (auto const &it : slots) {
			if (it.second->isParent())
				parent.push_back(static_cast<CustomObject*>(it.second));
		}
	}
//	unsigned i = 0;
//	while (i < slots.size()) {
//		if (slots[i]->getName() == slotName) {
//			return slots[i];
//		} else {
//			if (slots[i]->isParent()) {
////				slots[i]->lookup(slotName);
//				parent.push_back(static_cast<CustomObject*>(slots[i]));
//			}
//		}
//		++i;
//}
	ObjectMasCapo *aux, *resultado = NULL;
	CustomObject *actualParent;
	bool encontrado = false;
	for (unsigned i = 0; i < parent.size(); ++i) {
		actualParent =
				static_cast<CustomObject*>(parent[i]->slots.begin()->second);
		aux = actualParent->lookup(slotName);
		if (aux != NULL) {
			if (!encontrado) {
				encontrado = true;
				container = actualParent;
				resultado = aux; //
			} else {
				std::cout << "ERROR dos parent con el mismo slot" << std::endl;
//				resultado = NULL; //cositas1//
			}
		}
	}
	return resultado;
}

ObjectMasCapo* CustomObject::lookup(std::string slotName) {
// POR AHORA SOLO RETONA SI ESTA DENTRO DEL EL QUE LO LLAMA
// ME FALTA HACERLO RECUSIVO CON LOS PARENT
	std::vector<CustomObject*> parent;
	if (slots.find(slotName) != slots.end())
		return slots.at(slotName);
	else {
		for (auto const &it : slots) {
			if (it.second->isParent())
				parent.push_back(static_cast<CustomObject*>(it.second));
		}
	}
//	unsigned i = 0;
//	while (i < slots.size()) {
//		if (slots[i]->getName() == slotName) {
//			return slots[i];
//		} else {
//			if (slots[i]->isParent()) {
////				slots[i]->lookup(slotName);
//				parent.push_back(static_cast<CustomObject*>(slots[i]));
//			}
//		}
//		++i;
//}
	ObjectMasCapo *aux, *resultado = NULL;
	CustomObject *actualParent;
	bool encontrado = false;
	for (unsigned i = 0; i < parent.size(); ++i) {
		actualParent =
				static_cast<CustomObject*>(parent[i]->slots.begin()->second);
		aux = actualParent->lookup(slotName);
		if (aux != NULL) {
			if (!encontrado) {
				encontrado = true;
				resultado = aux; //
			} else {
				std::cout << "ERROR dos parent con el mismo slot" << std::endl;
//				resultado = NULL; //cositas1//
			}
		}
	}
	return resultado;
}

ObjectMasCapo* CustomObject::execute(std::string method,
		std::map<std::string, ObjectMasCapo*> arguments) {
	if (methods.find(method) != methods.end()) {
		MFP fp = methods[method];
		return (this->*fp)(arguments);
	} else
		return NULL;
}

int CustomObject::getPositionX() const {
	return positionX;
}
int CustomObject::getPositionY() const {
	return positionY;
}

void CustomObject::setPosition(int posX, int posY) {
	positionX = posX;
	positionY = posY;
}

unsigned CustomObject::numberOfSlots() {
	return this->slots.size();
}

std::string CustomObject::getValue() {
//	return code;
	return this->getCode();
}

Json::Value CustomObject::toJson() {
	//Json::Value value(Json::objectValue);
	Json::Value customObj_json(Json::objectValue), slots_json(Json::arrayValue);
	for (auto const &it : slots) {
		slots_json.append(it.second->toJson());
	}
//	for (int i = 0; i < this->slots.size(); ++i){
//		slots_json.append(slots[i]->toJson());
//	}

	std::cout << "------Jsoneo los slots de CustomObj-----" << this->getName()
			<< std::endl;

	customObj_json["type"] = "Custom";
	customObj_json["name"] = this->getName();
	customObj_json["slots"] = slots_json;
	customObj_json["code"] = this->getCode();
	customObj_json["pos x"] = this->getPositionX();
	customObj_json["pos y"] = this->getPositionY();

	std::string str = std::string(1, this->getFlag());
	customObj_json["slotType"] = str;

	return customObj_json;
}

