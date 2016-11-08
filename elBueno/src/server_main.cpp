//#include <iostream>
//#include <iostream>
//
//#include "server_VirtualMachine.h"
//#include "server_CustomObject.h"
//#include "server_NumberObject.h"
//#include "server_BoolObject.h"
//#include "server_StringObject.h"

//enum TipoSlot {ARG,PADRE,MUTABLE,INMUTABLE};

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>

#include "Parser.h"

int main(int argc, char *argv[]){
	std::fstream arch;
	arch.open(argv[1]);
	//archPtr = &arch;
	std::string str = "x = 5";
	std::stringstream ss (str);
//	Parser p(&ss);
	Parser p(&arch);
	bool pudoParsear = p.script();
//	std::cout << "Pudo parsear en main: " << pudoParsear << std::endl;
	arch.close();
	return 0;
}

//int main(int argc, char* argv[]) {
//	VirtualMachine mv;
//	std::map<std::string, ObjectMasCapo*> arguments;
//
//	NumberObject *unNum1 = mv.createObject("ocho", 8.0);
//	NumberObject *unNum2 = mv.createObject("cinco", 5.0);
//	CustomObject *b = mv.createObject("punto");
//	CustomObject *metodo = mv.createObject("");
//	metodo->addCode("'holuuus\n' print.'soy-the-best\n' print.");
//	metodo->rename("print");
//	char flag = 'M';
//	b->addSlot(unNum1, flag);
//	b->addSlot(unNum2, flag);
//	b->addSlot(metodo, flag);
//	CustomObject *vacio = mv.createObject("");
//	vacio->addSlot(b, flag);
//
////	arguments["_addSlot"] = b;
//	arguments["_addSlot"] = vacio;
//	mv.message(mv.lookup("lobby"), "_addSlot:", arguments);
//
//	mv.message(mv.lookup("lobby"), "print", arguments);
//
////	arguments["1"] = unNum1;
////	arguments["2"] = unNum2;
////	mv.message(b, "_addSlot", arguments);
////	mv.message(b, "print", arguments);
////	mv.message(mv.lookup("lobby"), "_addSlot", arguments);
////	ObjectMasCapo* aux = mv.lookup("ocho");
//
//	mv.message(mv.lookup("punto"), "print", arguments);
////	arguments["+"] = unNum1;
////	mv.message(mv.lookup("cinco"), "plus", arguments);
////	mv.message(mv.lookup("cinco"), "print", arguments);
////	mv.message(mv.lookup("cinco"), "+", arguments);
////	mv.message(mv.lookup("cinco"), "print", arguments);
//
////	BoolObject *unBool1 = static_cast<BoolObject*>(mv.message(mv.lookup("ocho"), "==", arguments));
////	mv.message(unBool1, "print", arguments);
////
////	BoolObject *unBool = mv.createObject("caca", true);
////	mv.message(unBool, "print", arguments);
////	std::cout << std::endl;
//
////	StringObject *unstg =mv.createObject("caca","mascaca");
////	mv.message(unstg, "print", arguments);
////	std::cout << std::endl;
//
//	return 0;
//}
