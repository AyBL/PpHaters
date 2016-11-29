#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

#include "Parser.h"
#include "server_Server.h"

int main(int argc, char *argv[]) {
	//  HACERLO BIEN
	if (argc == 2) {
		std::fstream arch;
		arch.open(argv[1]);
		VirtualMachine vm("lobby");
//		vm.JsonLoad("salida1.txt");
		try{
			Parser p(&arch, vm);
//			bool pudoParsear = p.script();
			p.script();  // SI NO USAMOS EL BOOL [-Werror=unused-variable]
			arch.close();
		}catch(...){
			std::cout << "SYNTAX ERROR" << std::endl;
		}
		vm.JsonSave("salida1.txt");
	} else {
		Server s(argv[1], argv[2]);
		s.start();
		char esQ;
		while ((std::cin >> esQ && esQ != 'q')) {}
		s.stop();
		std::cout<<"entra join"<<std::endl;
		s.join();
		std::cout<<"sale main"<<std::endl;
	}
	return 0;
}
