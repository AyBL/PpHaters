#ifndef PROXYCLIENTE_H_
#define PROXYCLIENTE_H_

#include "common_Socket.h"
#include "server_Thread.h"

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

	ProxyCliente(const ProxyCliente&) = delete;
	ProxyCliente& operator=(const ProxyCliente&) = delete;
	ProxyCliente(ProxyCliente&& other);
	ProxyCliente& operator=(ProxyCliente&& other);

	void close();

	char recvChar();
	void sendLobbiesMenu(std::vector<VirtualMachine> &mvs);
	uint8_t selectedLobby();
	/* recibe el numero de la virtual machine a la cual se quiere conectar
	 * de ser una nueva (cod 255) recibe el nombre de la misma */
	bool recvCommandL(uint8_t &indexMV, std::string &VMname);
	/* comando de nuevo objeto*/
	void recvCommandO(std::string &name, uint32_t &posX, uint32_t &posY);
	void sendCommandO(std::string name, uint32_t posX, uint32_t posY);
	/* comando de slot */
	char recvCommandS(std::string &objectName, std::string &stringToParse);
	void sendCommandS(std::string ObjectName, std::string SlotName,
			std::string value, char flag, std::string slotParent);
	/* comando de posiscion */
	char recvCommandP(std::string &objectToMove, uint32_t &newPosX,
			uint32_t &newPosY);
	void sendCommandP(std::string objectName, uint32_t posX, uint32_t posY);
	/* comando de mensaje */
	char recvCommandM(std::string &ObjectName, std::string &SlotName,
			uint32_t &newPosX, uint32_t &newPosY);
	void sendCommandM(std::string ObjectName, std::string SlotName,
			uint32_t posX, uint32_t posY);
	/* comando de borrado */
	char recvCommandE(std::string &ObjectName, std::string &slotName);
	void sendCommandE(const std::string ObjectName, std::string slotName);
	/* coamando para renombrar*/
	void recvCommandR(std::string &ObjectName, std::string &slotName,
			std::string &newSlotName);
	void sendCommandR(const std::string ObjectName, std::string slotName,
			std::string newSlotName);
	/* comando para mover objetos entre maquinas virtuales*/
	void recvCommandD(std::string &ObjectName);
	/* comando de envio de error*/
	void sendError(const std::string errorMessage);

private:
	/* metodos para recepcion y envio de los
	 * distintos tipos de datos necesarios*/
	std::string recvString();
	void sendString(std::string str);
	void sendChar(char c);
	void recvPosition(uint32_t &posX, uint32_t &posY);
	void sendPosition(uint32_t posX, uint32_t posY);
};

#endif /* PROXYCLIENTE_H_ */
