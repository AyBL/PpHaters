#ifndef PROXYCLIENTE_H_
#define PROXYCLIENTE_H_

#include "common_Socket.h"
#include "server_Thread.h"
//#include "server_VirtualMachine.h"

#include <vector>
#include <arpa/inet.h>

#define SIZE_BUFFER 50

class VirtualMachine;

class ProxyCliente {
private:
	Socket skt;
	char buffer[SIZE_BUFFER] = { 0 };
	bool clienteEnviando = true;

public:
	explicit ProxyCliente(Socket skt);

	virtual ~ProxyCliente();

	bool masConsultas();

	void sendLobbiesMenu(std::vector<VirtualMachine> &mvs);
	char recvMessage(); // RECV CHAR/BYTE
	std::string getName(); // GETSTRING
	void sendName(std::string name);
	void sendPosition(uint32_t posX, uint32_t posY);
	uint8_t selectedLobby();
	void getPosition(uint32_t &posX, uint32_t &posY);

	void sendCommand(char commandID);

	void commandL();

	void commandO(std::string &name, uint32_t &posX, uint32_t &posY);
	void sendCommandO(std::string name, uint32_t posX, uint32_t posY);

//	void commandO(std::string &name, uint32_t &posX,uint32_t &posY);
	char commandS(std::string &objectName, std::string &stringToParse);
	void sendCommandS(std::string ObjectName, std::string SlotName,
			std::string value, char flag, std::string slotParent);

	void commandP(std::string &objectToMove, uint32_t &newPosX,
			uint32_t &newPosY);
	void sendCommandP(std::string objectName, uint32_t posX, uint32_t posY);

	char commandM(std::string &ObjectName, std::string &SlotName,
			uint32_t &newPosX, uint32_t &newPosY);
	void sendCommandM(std::string ObjectName, std::string SlotName,
			uint32_t posX, uint32_t posY);

	char commandE(std::string &ObjectName, std::string &slotName);
	void sendCommandE(const std::string ObjectName, std::string slotName);

	void sendError(const std::string errorMessage);

//	Enviar	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
//	Recibir	R	tam	nomobj	tam	nomslot	tam	nuevo nombre
	void commandR(std::string &ObjectName, std::string &slotName,
			std::string &newSlotName);
	void sendCommandR(const std::string ObjectName, std::string slotName,
			std::string newSlotName);

	//------- por moviento --------//
	ProxyCliente(const ProxyCliente&) = delete;
	ProxyCliente& operator=(const ProxyCliente&) = delete;
	ProxyCliente(ProxyCliente&& other);
	ProxyCliente& operator=(ProxyCliente&& other);

	//-----------------------
private:
//	std::string recvString();
//	void sendString(std::string str);
//	char recvChar();
//	void sendChar(char c);
//	uint32_t recvPosition();
//	void sendPosition(uint32_t position);
};

#endif /* PROXYCLIENTE_H_ */
