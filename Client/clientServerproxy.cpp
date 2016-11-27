#include "clientServerproxy.h"
#include "clientwindowobject.h"
#include <tuple>

Serverproxy::Serverproxy(MenuWindow **menu, WindowObject **window)
:socket(0),menu(menu),window(window),must_be_run(true){
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

int Serverproxy::Send(char *sendbuffer,size_t length){

	socket.Send(sendbuffer, length);

	return 0;
}

int Serverproxy::ReceiveAnswer(){
	int s;

	s = socket.Receive(&command, COMMANDSIZE);
	if (s>0){
		std::cout << "Comando: "<<command << std::endl;
	    switch (command) {
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
			case 'P':
				CommandP();
				break;
			case 'R':
				CommandR();
				break;
			case 'S':
				CommandS();
				break;
			case 'X':
				CommandX();
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
	int s = 1;
	while(must_be_run && (s != 0)){
		s = ReceiveAnswer();
	}

	if (s == 0)
		(*window)->CloseApp();	
}

void Serverproxy::Stop(){
	if (must_be_run){
		must_be_run = false;
		socket.Shutdown();
	}
}

void Serverproxy::CommandE(){
	std::string nameobject1,nameobject2;
	char tamanio;
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject1 = std::string(bufferanswer);
	std::memset(bufferanswer, 0, tamanio);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject2 = std::string(bufferanswer);
	if (nameobject1 == "lobby"){
		(*window)->AddAction(new RemoveObjectAction(*window,nameobject2));	
	} else {
		(*window)->RemoveSlot(nameobject1,nameobject2);
	}	
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
	std::string nameobject,valueslot;
	char tamanio;
	int x,y;
	nameobject = "";

	socket.Receive(&tamanio, sizeof(char));
	std::memset(bufferanswer, 0, MAXSIZE);

	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	std::memset(bufferanswer, 0, (size_t)tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	valueslot = std::string(bufferanswer);
	socket.Receive(bufferanswer, 4*sizeof(char));
	memcpy(&x,bufferanswer,sizeof(int));
	socket.Receive(bufferanswer, 4*sizeof(char));
	memcpy(&y,bufferanswer,sizeof(int));
	(*window)->AddAction(new AddValueAction(*window,nameobject,valueslot,x,y));
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
	(*window)->AddAction(new AddObjectAction(*window,nameobject,x,y));
}

void Serverproxy::CommandP(){
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

void Serverproxy::CommandR(){

	std::string nameobject1,nameobject2,newname;
	char tamanio;
	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject1 = std::string(bufferanswer);
	std::memset(bufferanswer, 0, tamanio);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject2 = std::string(bufferanswer);
	std::memset(bufferanswer, 0, tamanio);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	newname = std::string(bufferanswer);
	if (nameobject1 == "lobby"){
		(*window)->ChangeNameObject(nameobject2,newname);
	} else {
		(*window)->ChangeNameSlot(nameobject1,nameobject2,newname);
	}	
}

void Serverproxy::CommandS(){
	std::string nameobject,nameslot,value,parent;
	char tamanio,mut;

	std::tuple<std::string,std::string,char,std::string> aux;

	std::memset(bufferanswer, 0, MAXSIZE);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameobject = std::string(bufferanswer);
	std::cout << "Nombre Objeto: "<<nameobject;
	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	nameslot = std::string(bufferanswer);
	std::cout << " Nombre Slot: "<<nameslot;
	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	value = std::string(bufferanswer);
	std::cout << " Value Slot: "<<value<< std::endl;
	std::memset(bufferanswer, 0, tamanio);

	socket.Receive(&mut, sizeof(char));

	if (mut == 'P'){
		socket.Receive(&tamanio, sizeof(char));
		socket.Receive(bufferanswer, (size_t)tamanio);
		parent = std::string(bufferanswer);
	}else{
		parent = "";
	}

	aux = std::make_tuple(nameslot,value,mut,parent);
	(*window)->AddSlot(nameobject,aux);
}

void Serverproxy::CommandX(){
	std::string error;
	char tamanio;

	std::memset(bufferanswer, 0, MAXSIZE);
	socket.Receive(&tamanio, sizeof(char));
	socket.Receive(bufferanswer, (size_t)tamanio);
	error = std::string(bufferanswer);
	(*window)->ErrorMessage(error);	
}