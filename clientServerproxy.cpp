#include "clientServerproxy.h"
#include "clientwindowobject.h"
#include <tuple>

Serverproxy::Serverproxy(MenuWindow **menu, WindowObject **window):socket(0),
menu(menu),window(window),must_be_run(true){
}

int Serverproxy::Connect(const char *hostname,const  char *port){
	int s;
	s = socket.Connect(hostname, port);
	if (s != 0){
		socket.Shutdown();
		return 1;
	}
	return 0;
}

int Serverproxy::SendCommand(char *sendbuffer){

	socket.Send(sendbuffer, strlen(sendbuffer));

	return 0;
}

int Serverproxy::SendPositions(char *sendbuffer){

	socket.Send(sendbuffer, sizeof(int));

	return 0;
}

int Serverproxy::ReceiveAnswer(){
	int s;

	s = socket.Receive(&command, COMMANDSIZE);
	if (s>0){
		std::cout<< command<<std::endl;
	    switch (command) {
			case 'D':
				CommandD();
				break;
			case 'E':
				CommandE();
				break;
	        case 'L':
	            CommandL();
	            break;
			case 'M':
				CommandM();
				break;
			case 'O':
				CommandO();
				break;
			case 'S':
				CommandS();
				break;
			default:
				break;
	    }
	}
 	return s;
}

Serverproxy::~Serverproxy(){
	socket.Shutdown();
}

void Serverproxy::run(){
	while(must_be_run){
		ReceiveAnswer();
	}
}

void Serverproxy::Stop(){
	if (must_be_run){
		must_be_run = false;
		socket.Shutdown();
	}
}

void Serverproxy::CommandD(){
	std::string nameobject,nameslot;
	char tamanio;
	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, tamanio);
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameslot = std::string(bufferanswer);
	(*window)->RemoveSlot(nameobject,nameslot);
}

void Serverproxy::CommandE(){
	std::string nameobject;
	char tamanio;
	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	(*window)->RemoveObject(nameobject);
}

void Serverproxy::CommandL(){
	unsigned char i, ucantidad;
	char cantidad, tamanio;

	socket.Receive(&cantidad, sizeof(char));
	ucantidad = (unsigned char) cantidad;
	for (i = 0; i < ucantidad; i++){
		socket.Receive(&tamanio, sizeof(char));
		std::memset(bufferanswer, 0, MAXSIZE);
		socket.Receive(bufferanswer, (size_t)tamanio);
		(*menu)->AddLobbies(std::make_tuple(i,bufferanswer));
	}
}
void Serverproxy::CommandM(){
	std::string nameobject;
	char tamanio;
	int x,y;
	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	socket.Receive(bufferanswer, sizeof(int));
	memcpy(&x,bufferanswer,sizeof(int));
	socket.Receive(bufferanswer, sizeof(int));
	memcpy(&y,bufferanswer,sizeof(int));

	(*window)->Move(nameobject,x,y);
}

void Serverproxy::CommandO(){
	std::string nameobject,nameslot;
	char tamanio;
	int x,y;
	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	socket.Receive(bufferanswer, 4*sizeof(char));
	memcpy(&x,bufferanswer,sizeof(int));
	socket.Receive(bufferanswer, 4*sizeof(char));
	memcpy(&y,bufferanswer,sizeof(int));
	std::cout<< nameobject <<std::endl;
	std::cout<< "x: " << x << " y: " << y <<std::endl;
	(*window)->AddObject(nameobject,x,y);
}

void Serverproxy::CommandS(){
	std::string nameobject,nameslot,value,type,parent;
	char tamanio,mut;

	std::tuple<std::string,std::string,std::string,char,std::string> aux;

	std::memset(bufferanswer, 0, MAXSIZE);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);

	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameslot = std::string(bufferanswer);

	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	value = std::string(bufferanswer);

	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	type = std::string(bufferanswer);

	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&mut, sizeof(char));

	if (mut == 'p'){
		socket.Receive(&tamanio, sizeof(char));
		socket.Receive(bufferanswer, (size_t)tamanio);
		parent = std::string(bufferanswer);
	}else{
		parent = "";
	}

	aux = std::make_tuple(nameslot,value,type,mut,parent);
	(*window)->AddSlot(nameobject,aux);
}
