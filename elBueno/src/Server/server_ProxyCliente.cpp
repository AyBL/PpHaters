#include "server_ProxyCliente.h"
#include "server_VirtualMachine.h"
#include <cstring>

//#define los char de comandos

ProxyCliente::ProxyCliente(Socket skt) :
		skt(std::move(skt)) {
}

ProxyCliente::~ProxyCliente() {
	skt.shutdown();
}

// CAMBIAR COMO COMANDO O
void ProxyCliente::sendLobbiesMenu(std::vector<VirtualMachine> &mvs) {
	std::cout << "entro de envio menu lobbies" << std::endl;
	memset(buffer, 0, sizeof(buffer));
	unsigned char aux = 'L';
	std::string mensaje(1, aux);
	std::string lobbyName;
	aux = mvs.size();
	mensaje += std::string(1, aux);
	for (unsigned i = 0; i < mvs.size(); ++i) {
		lobbyName = mvs[i].getName();
		aux = lobbyName.length();
		mensaje += std::string(1, aux) + lobbyName;
	}
//	std::cout << "size: " << mensaje.length() << std::endl;
	int size = mensaje.length();
	memcpy(buffer, mensaje.c_str(), size);
	skt.send(buffer, size); // falta chequear errores, gila
	std::cout << "fin de envio menu lobbies" << std::endl;
}

std::string ProxyCliente::getName() {
	std::string name;
	unsigned char lenName;
	memset(buffer, 0, sizeof(buffer));
	skt.recv(buffer, sizeof(char));
	memcpy(&lenName, buffer, sizeof(char));
	skt.recv(buffer, static_cast<uint8_t>(lenName));
	std::cout << "nombre recibido: " << buffer << std::endl;
	return std::string(buffer);
}

uint8_t ProxyCliente::selectedLobby() {
	memset(buffer, 0, sizeof(buffer));
	unsigned char aux;
//	skt.recv(buffer, 1); // sacar aca debe llegar la L
	skt.recv(buffer, 1);
	memcpy(&aux, buffer, sizeof(char));
	return static_cast<uint8_t>(aux);
}

char ProxyCliente::recvMessage() {
//	std::cout << "recibe mensaje gatoo" << std::endl;
	char command;
	skt.recv(&command, sizeof(char));
//	std::cout << "command: " << command << std::endl;
	return command;
}

void ProxyCliente::getPosition(uint32_t &posX, uint32_t &posY) {
	memset(buffer, 0, sizeof(buffer));
	skt.recv(buffer, sizeof(uint32_t) * 2);
	memcpy(&posX, buffer, sizeof(uint32_t));
	memcpy(&posY, &buffer[4], sizeof(uint32_t));
//	posX = ntohl(posX);
//	posY = ntohl(posY);
}

void ProxyCliente::commandL() {
	//	L	Num Lobby
	//	L	255	tamanio	Nombre
	memset(buffer, 0, sizeof(buffer));
	unsigned char aux;
	skt.recv(buffer, 1);
	memcpy(&aux, buffer, sizeof(char));
	std::cout << "vamo lo pi " << static_cast<uint8_t>(aux) << std::endl;
	if (static_cast<uint8_t>(aux) == 255) {
		std::string newLobbyName;
		skt.recv(buffer, 1);
		memcpy(&aux, buffer, sizeof(char));
		skt.recv(buffer, static_cast<uint8_t>(aux));
		std::cout << "buffer recibido: " << buffer << std::endl;
		;
	} else {
		// recibe index
		std::cout << static_cast<uint8_t>(aux) << std::endl;
	}
}

//create an object
void ProxyCliente::commandO(std::string &name, uint32_t &posX, uint32_t &posY) {
//	std::cout << "recibo los de comando O: ";
	name = getName();
	std::cout << name << " nombre ingresado\n";
	getPosition(posX, posY);
	std::cout << " " << posX << " " << posY << " \n";
}

void ProxyCliente::sendCommandO(std::string name, uint32_t posX,
		uint32_t posY) {
//	buffer[0] = 'O';
//	skt.send(buffer, sizeof(char));
	sendCommand('O');
	sendName(name);
	sendPosition(posX, posY);
}

void ProxyCliente::sendCommand(char commandID) { //usar define para los nombres de los comandos
	buffer[0] = commandID;
	skt.send(buffer, sizeof(char));
}

void ProxyCliente::sendName(std::string name) {
	unsigned char aux = name.length();
	std::string mensaje;
	mensaje += std::string(1, aux) + name;
	int size = mensaje.length();
	std::cout << size << std::endl;
	memcpy(buffer, mensaje.c_str(), size);
	skt.send(buffer, size);
}

void ProxyCliente::sendPosition(uint32_t posX, uint32_t posY) {
//	posX = htonl(posX);
//	posY = htonl(posY);
//	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, &posX, sizeof(uint32_t));
	memcpy(&buffer[4], &posY, sizeof(uint32_t));
	skt.send(buffer, sizeof(uint32_t) * 2);
}

char ProxyCliente::commandS(std::string &objectName,
		std::string &stringToParse) {
	char type = recvMessage();
	objectName = getName();
	stringToParse = getName();
	return type;
}

void ProxyCliente::sendCommandS(std::string objectName, std::string slotName,
		std::string value, char flag, std::string slotParent) {
	//faltan todos los chequeos de errores
	std::cout << "ProxyCliente: sendCommandA " << objectName << " " << slotName
			<< " " << value << " " << flag << " " << slotParent << std::endl;
	sendCommand('S');
	sendName(objectName);
	sendName(slotName);
	sendName(value);
	sendCommand(flag); // queda feo con este nombre de metodo pero envia un char
	if (flag == 'P') {
		sendName(slotParent);
	}
	std::cout << "ProxyCliente:fin de envio comando A " << std::endl;
}

//Enviar	P (Position)	tam	nomobj	int x	int y
void ProxyCliente::commandP(std::string &objectToMove, uint32_t &newPosX,
		uint32_t &newPosY) {
	objectToMove = getName();
	getPosition(newPosX, newPosY);
}

void ProxyCliente::sendCommandP(std::string objectName, uint32_t posX,
		uint32_t posY) {
	sendCommand('P');
	sendName(objectName);
	sendPosition(posX, posY);
}

//Enviar	M (Message)	tam	nomobj	tam	nomslot	x	y
//Recibir	M	tam	nomobj	tam	string	x	y

char ProxyCliente::commandM(std::string &ObjectName, std::string &SlotName,
		uint32_t &newPosX, uint32_t &newPosY) {
	char type = recvMessage();
	ObjectName = getName();
	SlotName = getName();
	getPosition(newPosX, newPosY);
	return type;
}

void ProxyCliente::sendCommandM(std::string objectName, std::string SlotName,
		uint32_t posX, uint32_t posY) {
	sendCommand('M');
	sendName(objectName);
	sendName(SlotName);
	sendPosition(posX, posY);
}

void ProxyCliente::commandE(std::string &ObjectName, std::string &slotName) {
	ObjectName = getName();
	slotName = getName();
}

void ProxyCliente::sendCommandE(const std::string ObjectName,
		std::string slotName) {
	sendCommand('E');
	sendName(ObjectName);
	sendName(slotName);
}

bool ProxyCliente::masConsultas() {
	return clienteEnviando;
}

void ProxyCliente::commandR(std::string &ObjectName, std::string &slotName,
		std::string &newSlotName) {
	ObjectName = getName();
	slotName = getName();
	newSlotName = getName();
}
void ProxyCliente::sendCommandR(const std::string ObjectName,
		std::string slotName, std::string newSlotName) {
	sendCommand('R');
	sendName(ObjectName);
	sendName(slotName);
	sendName(newSlotName);
}

void ProxyCliente::sendError(const std::string errorMessage){
	sendCommand('X');
	sendName(errorMessage);
}

ProxyCliente::ProxyCliente(ProxyCliente&& other) {
	skt=std::move(other.skt);
	memcpy(buffer, other.buffer, sizeof(buffer));
	memset(other.buffer, 0, sizeof(buffer));
	clienteEnviando = other.clienteEnviando;
}
ProxyCliente& ProxyCliente::operator=(ProxyCliente&& other) {
	skt=std::move(other.skt);
	memcpy(buffer, other.buffer, sizeof(buffer));
	memset(other.buffer, 0, sizeof(buffer));
	clienteEnviando = other.clienteEnviando;
	return *this;
}

