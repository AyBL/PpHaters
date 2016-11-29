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

ProxyCliente::ProxyCliente(ProxyCliente && other)
{
	skt = std::move(other.skt);
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

void ProxyCliente::close() {
	skt.shutdown();
}

void ProxyCliente::sendLobbiesMenu(std::vector<VirtualMachine> &mvs) {
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
	int size = mensaje.length();
	memcpy(buffer, mensaje.c_str(), size);
	skt.send(buffer, size);
}

bool ProxyCliente::recvCommandL(uint8_t &indexMV, std::string &VMname) {
	char command = recvChar();
	if (command == 'L') {
		indexMV = selectedLobby();
		if (indexMV == 255) {
			VMname = recvString();
		}
		return true;
	}
	return false;
}

std::string ProxyCliente::recvString() {
	std::string name;
	unsigned char lenName;
	memset(buffer, 0, sizeof(buffer));
	skt.recv(buffer, sizeof(char));
	memcpy(&lenName, buffer, sizeof(char));
	skt.recv(buffer, static_cast<uint8_t>(lenName));
	return std::string(buffer);
}

uint8_t ProxyCliente::selectedLobby() {
	memset(buffer, 0, sizeof(buffer));
	unsigned char aux;
	skt.recv(buffer, 1);
	memcpy(&aux, buffer, sizeof(char));
	return static_cast<uint8_t>(aux);
}

char ProxyCliente::recvChar() {
	char command;
	skt.recv(&command, sizeof(char));
	return command;
}

void ProxyCliente::recvPosition(uint32_t &posX, uint32_t &posY) {
	memset(buffer, 0, sizeof(buffer));
	skt.recv(buffer, sizeof(uint32_t) * 2);
	memcpy(&posX, buffer, sizeof(uint32_t));
	memcpy(&posY, &buffer[4], sizeof(uint32_t));
	posX = ntohl(posX);
	posY = ntohl(posY);
}

void ProxyCliente::recvCommandO(std::string &name, uint32_t &posX,
		uint32_t &posY) {
	name = recvString();
	recvPosition(posX, posY);
}

void ProxyCliente::sendCommandO(std::string name, uint32_t posX,
		uint32_t posY) {
	sendChar('O');
	sendString(name);
	sendPosition(posX, posY);
}

void ProxyCliente::sendChar(char commandID) {
	buffer[0] = commandID;
	skt.send(buffer, sizeof(char));
}

void ProxyCliente::sendString(std::string name) {
	unsigned char aux = name.length();
	std::string mensaje;
	mensaje += std::string(1, aux) + name;
	int size = mensaje.length();
	memcpy(buffer, mensaje.c_str(), size);
	skt.send(buffer, size);
}

void ProxyCliente::sendPosition(uint32_t posX, uint32_t posY) {
	posX = htonl(posX);
	posY = htonl(posY);
	memcpy(buffer, &posX, sizeof(uint32_t));
	memcpy(&buffer[4], &posY, sizeof(uint32_t));
	skt.send(buffer, sizeof(uint32_t) * 2);
}

char ProxyCliente::recvCommandS(std::string &objectName,
		std::string &stringToParse) {
	char type = recvChar();
	objectName = recvString();
	stringToParse = recvString();
	return type;
}

void ProxyCliente::sendCommandS(std::string objectName, std::string slotName,
		std::string value, char flag, std::string slotParent) {
	sendChar('S');
	sendString(objectName);
	sendString(slotName);
	sendString(value);
	sendChar(flag);
	if (flag == 'P') {
		sendString(slotParent);
	}
}

char ProxyCliente::recvCommandP(std::string &objectToMove, uint32_t &newPosX,
		uint32_t &newPosY) {
	char type = recvChar();
	objectToMove = recvString();
	recvPosition(newPosX, newPosY);
	return type;
}

void ProxyCliente::sendCommandP(std::string objectName, uint32_t posX,
		uint32_t posY) {
	sendChar('P');
	sendString(objectName);
	sendPosition(posX, posY);
}

char ProxyCliente::recvCommandM(std::string &ObjectName, std::string &SlotName,
		uint32_t &newPosX, uint32_t &newPosY) {
	char type = recvChar();
	ObjectName = recvString();
	SlotName = recvString();
	recvPosition(newPosX, newPosY);
	return type;
}

void ProxyCliente::sendCommandM(std::string objectName, std::string SlotName,
		uint32_t posX, uint32_t posY) {
	sendChar('M');
	sendString(objectName);
	sendString(SlotName);
	sendPosition(posX, posY);
}

char ProxyCliente::recvCommandE(std::string &ObjectName,
		std::string &slotName) {
	char type = recvChar();
	ObjectName = recvString();
	slotName = recvString();
	return type;
}

void ProxyCliente::sendCommandE(const std::string ObjectName,
		std::string slotName) {
	sendChar('E');
	sendString(ObjectName);
	sendString(slotName);
}

void ProxyCliente::recvCommandR(std::string &ObjectName, std::string &slotName,
		std::string &newSlotName) {
	ObjectName = recvString();
	slotName = recvString();
	newSlotName = recvString();
}
void ProxyCliente::sendCommandR(const std::string ObjectName,
		std::string slotName, std::string newSlotName) {
	sendChar('R');
	sendString(ObjectName);
	sendString(slotName);
	sendString(newSlotName);
}

void ProxyCliente::sendError(const std::string errorMessage) {
	sendChar('X');
	sendString(errorMessage);
}

void ProxyCliente::recvCommandD(std::string &ObjectName) {
	ObjectName = recvString();
}
