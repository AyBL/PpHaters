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
	int x,y;
	unsigned char i, ucantidad;
	char cantidad, tamanio;
	std::string nameobject,nameslot;
	std::string value,type,parent;
	char mut;

	s = socket.Receive(&command, COMMANDSIZE);
	if (s>0){
		std::cout<< command<<std::endl;
	    switch (command) {
			case 'D':
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameobject = std::string(bufferanswer);
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameslot = std::string(bufferanswer);
				(*window)->RemoveSlot(nameobject,nameslot);
				break;
			case 'E':
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameobject = std::string(bufferanswer);
				(*window)->RemoveObject(nameobject);
				break;
	        case 'L':
	            socket.Receive(&cantidad, sizeof(char));
				ucantidad = (unsigned char) cantidad;
				for (i = 0; i < ucantidad; i++){
					socket.Receive(&tamanio, sizeof(char));
					std::memset(bufferanswer, 0, MAXSIZE);
					socket.Receive(bufferanswer, (size_t)tamanio);
					(*menu)->AddLobbies(std::make_tuple(i,bufferanswer));
				}
	            break;
			case 'M':
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameobject = std::string(bufferanswer);
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, 8*sizeof(char));
				for (int i = 0; i < 4; i++){
					x = (x << 8) + bufferanswer[0+i];
		        	y = (y << 8) + bufferanswer[4+i];
		    	}
				x = ntohl(x);
				y = ntohl(y);
				(*window)->Move(nameobject,x,y);
				break;
			case 'O':
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameobject = std::string(bufferanswer);
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, 4*sizeof(char));
				memcpy(&x,bufferanswer,sizeof(int));
				//x = ntohl(x);

				socket.Receive(bufferanswer, 4*sizeof(char));
				memcpy(&y,bufferanswer,sizeof(int));
				//y = ntohl(y);
				std::cout<< nameobject <<std::endl;
				std::cout<< "x: " << x << " y: " << y <<std::endl;
				(*window)->AddObject(nameobject,x,y);
				break;
			case 'S':
				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameobject = std::string(bufferanswer);

				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				nameslot = std::string(bufferanswer);

				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				value = std::string(bufferanswer);

				socket.Receive(&tamanio, sizeof(char));
				std::memset(bufferanswer, 0, MAXSIZE);
				socket.Receive(bufferanswer, (size_t)tamanio);
				type = std::string(bufferanswer);

				socket.Receive(&mut, sizeof(char));

				if (mut == 'p'){
					socket.Receive(&tamanio, sizeof(char));
					std::memset(bufferanswer, 0, MAXSIZE);
					socket.Receive(bufferanswer, (size_t)tamanio);
					parent = std::string(bufferanswer);
				}else{
					parent = "";
				}
				(*window)->AddSlot(nameobject,std::make_tuple(nameslot,value,type,mut,parent));

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
	if (must_be_run)
		must_be_run = false;
}
