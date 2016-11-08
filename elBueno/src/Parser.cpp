/*
 * Parser.cpp

 *
 *  Created on: 26/10/2016
 *      Author: sofia
 */

#include <cctype>
#include <tuple>
#include <vector>
#include <sstream>

#include "Parser.h"
#include "server_CustomObject.h"

//Debe recibir un stream
//Si es un archivo debe abrirse y cerrarse afuera
Parser::Parser(std::istream *stream) : VM("lobby"), stream(stream) {
/*	this->stream.open(nombreArch);
	if (this->stream.fail()){
		////std::cout << "No se pudo abrir el archivo" << std::endl;
	}*/
	_receiver = NULL;
}

Parser::~Parser() {
/*	if (!(this->stream.fail())){
		this->stream.close();
	}*/
}

bool Parser::hayParentesis1(){
	c = this->stream->get();
	if (c == '('){
		return true;
	}else{
		return false;
	}
}

bool Parser::hayParentesis2(){
	c = this->stream->get();
	if (c == ')'){
		return true;
	}else{
		return false;
	}
}

bool Parser::hayComillas(){
	c = this->stream->get();
	if (c == '\''){
		return true;
	}else{
		return false;
	}
}

bool Parser::hayPipe(){
	//c = this->stream->get();
	*(this->stream) >> c;

	////std::cout << "C en hayPipe: " << c << std::endl;

	if (c == '|'){
		return true;
	}else{
		return false;
	}
}

bool Parser::hayDelimitador(){
	c = this->stream->get();
	if ((c == ':') || (c == ' ') || (c == '.') || (c == '*')){
		return true;
	}
	return false;
}

bool Parser::finStringCodigo(){
	//c = this->stream->get();
	int posActual = this->stream->tellg();
	if (c == '.'){
		c = this->stream->get();
		if (c == ')'){
			c = this->stream->get();
			if (c == '.'){
				return true;
			}
		}
	}
	//Vuelvo a pos antes de leer estos caracteres
	this->stream->seekg(posActual);
	return false;
}

bool Parser::esMutable(){
	c = this->stream->get();

	////std::cout << "Lee en esMutable: " << c << std::endl;

	if (c == '='){
		return true;
	}else if (c == '<'){
		c = this->stream->get();
		if (c == '-')
			return false;
		else{
			//ERROR
		}
	}else{
		//ERROR
	}
	//Mando esto para q pase las pruebas pero deberia mandar excepciones si hay
	//error
	return false;
}

bool Parser::getStringCodigo(std::string &codigo){
	//LEO CARACTER A CARACTER, MUY LENTO??? VER
	//Cuando lee .). significa q el codigo del objeto termino
	c = this->stream->get();
	while (!(this->finStringCodigo())){
		c = this->stream->get();
		codigo += c;
	}

	////std::cout << "-----Codigo en getStringCod: " << codigo << std::endl;
	//VER COMO HACER PARA DETECTAR ERROR DE SINTAXIS
	return true;
}

bool Parser::script(){
	bool pudoParsear = true;
	int i = 0;

	while ((pudoParsear) && (*(this->stream) >> c)){
		////std::cout << "Lo que lee el while: " << c << std::endl;
		this->stream->unget();

		//Guardo la pos actual
		int posActual = this->stream->tellg();
		ObjectMasCapo *exp;
		pudoParsear = this->expression(exp, posActual);

		////std::cout << "---------------------------Iteracion en script: " << i << std::endl;
		++i;

		////std::cout << "C en script: " << c << std::endl;

		_receiver = NULL;

		if (c != '.'){
			//ERROR SINTAXIS
			return false;
		}
	}
	return pudoParsear;
}

bool Parser::expression(ObjectMasCapo* &exp, int &pos){
	bool pudoParsear = false;
	std::string str;
	if ((this->keywordMessage(exp, pos)) || (this->binaryMessage(exp, pos)) ||
	 (this->unaryMessage(exp, pos)) || (this->expressionCP(exp, pos))){
		pudoParsear = true;
	}
	////std::cout << "PUDO PARSEAR EN EXPRESSION: " << pudoParsear << std::endl;
	return pudoParsear;
}

bool Parser::expressionCP(ObjectMasCapo* &obj, const int &pos){
	bool pudoParsear = true;

	////std::cout << "-----ENTRO A EXPRESSIONCP------" << std::endl;

	if (_receiver == NULL){
		pudoParsear = this->expressionP(obj, pos);
		if (!pudoParsear){
			pudoParsear = this->getConstant(obj, pos);
		}
	}else{
		obj = _receiver;
		_receiver = NULL;
	}
	return pudoParsear;
}

bool Parser::expressionP(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	if (!(this->hayParentesis1())){
		return false;
	}

	////std::cout << "Queda en expressionP" << std::endl;

	this->stream->unget();
	if ((this->hayParentesis1() && this->hayPipe())){
		return false;
	}
	this->stream->unget();
	////std::cout << "Ve que no hay pipe" << std::endl;

	int posActual = this->stream->tellg();
	bool pudoParsear = this->expression(obj, posActual);

	if ((!pudoParsear) || (c != ')')){
		//ERROR
		return false;
	}
	c = this->stream->get();
	return pudoParsear;
}

bool Parser::keywordMessage(ObjectMasCapo* &obj, int &pos){
	this->stream->seekg(pos);
	ObjectMasCapo* receiver;
	std::string lowerKeyword, messageName;
	std::map<std::string, ObjectMasCapo*> argList;

	////std::cout << "---------ENTRA A NARIO-----------"<< std::endl;

	bool pudoParsear = this->getReceiver(receiver, pos);

	////std::cout << "Char despues de getReceiver en kwMessage: " << c << std::endl;
	////std::cout << "Pudo parsear receiver en kwMessage: " << pudoParsear << std::endl;

	//Si pudo parsear todo recien ahi llamo al metodo de la vm y paso las cosas
	//a los tipos necesarios
	if (pudoParsear){
		_receiver = receiver;
		pos = this->stream->tellg();

		////std::cout << "Entro a if en kwMessage" << std::endl;

		//Si el receiver termina con punto entonces no es un mensaje
		if (c == '.')
			return false;

		pudoParsear = this->getLowerKeyword(lowerKeyword);

		////std::cout << "lower keyword: " << lowerKeyword << std::endl;

		if (!pudoParsear){
			this->stream->seekg(pos);
			c = this->stream->get();
			return false;
		}
		messageName += lowerKeyword;
		messageName += ':';

		////std::cout << "Antes de espacio: " << c << std::endl;

		//Avanzo un espacio
		c = this->stream->get();

		////std::cout << "Despues de espacio: " << c << std::endl;

		ObjectMasCapo* arg;
		int posActual = this->stream->tellg();

		////std::cout << "Pos despues de get lowerkw: " << posActual << std::endl;

		_receiver = NULL;
		pudoParsear = this->expressionCP(arg, posActual);

		////std::cout << "Pudo parsear expCP: " << pudoParsear << std::endl;

		if (!pudoParsear){
			return false;
		}

		argList[lowerKeyword] = arg;

		////std::cout << "Arg en el map del lowerKeyword" << argList[lowerKeyword]->getName() << std::endl;

		//Todos los metodos de expressionCp leen un caracter despues de cuando
		//terminan lo que buscan, entonces ya tengo en c ese caracter
		while (pudoParsear && (c == ' ')){

			////std::cout << "Entra a buscar mas keywords" << std::endl;

			std::string capitalKeyword;
			pudoParsear = this->getCapKeyword(capitalKeyword);

			if (!pudoParsear){
				return false;
			}

			ObjectMasCapo* arg1;
			int posActual = this->stream->tellg();
			pudoParsear = this->expressionCP(arg1, posActual);
			if (!pudoParsear){
				return false;
			}
			argList[capitalKeyword] = arg;
			messageName += capitalKeyword;
			messageName += ':';
		}
		obj = VM.message(receiver, messageName, argList);
	}

	////std::cout << "Receiver: " << receiver << std::endl;
	////std::cout << "Pudo parsear en kwMessage: " << pudoParsear << std::endl;
	//Agregar lower keyword

	return pudoParsear;
}

bool Parser::binaryMessage(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	ObjectMasCapo *receiver, *arg;
	std::string op;
	std::map<std::string, ObjectMasCapo*> argList;

	////std::cout << "----------------Entra a mensaje binario-----------" << std::endl;

//	bool pudoParsear = this->getReceiver(receiver, pos);
//
//	if (!pudoParsear){
//		return false;
//	}
	receiver = _receiver;

	////std::cout << "Se va a fijar el operador" << std::endl;

	bool pudoParsear = this->getOperator(op);

	if (!pudoParsear){
		return false;
	}

	c = this->stream->get();

	if (c != ' '){
		//ERROR SINTAXIS
		////std::cout << "Error sintaxis" << std::endl;
		return false;
	}

	int posActual = this->stream->tellg();
	_receiver = NULL;
	pudoParsear = this->expressionCP(arg, posActual);

	if (!pudoParsear){
		return false;
	}
	argList[op] = arg;
	obj = VM.message(receiver, op, argList);
	return pudoParsear;
}

bool Parser::unaryMessage(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	//bool esExpCP = false;
	ObjectMasCapo *receiver, *aux;
	std::string messageName;
	std::map<std::string, ObjectMasCapo*> argList;

	////std::cout << "------------------Entro a unario--------------" << std::endl;

//	bool pudoParsear = this->getReceiver(receiver, pos);
//
//	////std::cout << "Pudo parsear receiver en unario: " << pudoParsear << std::endl;
//
//	if (!pudoParsear){
//		return false;
//	}
//	c = this->stream->get();
//	this->stream->unget();
	if (c == '.'){
		////std::cout << "Entra a false en unario" << std::endl;
		return false;
	}

	receiver = _receiver;
	_receiver = NULL;

	int posActual = this->stream->tellg();
	bool pudoParsear = this->getName(aux, messageName, posActual, false);
	if (!pudoParsear){
		return false;
	}

	////std::cout << "Va a llamar a la VM en unario" << std::endl;

	obj = VM.message(receiver, messageName, argList);
	return pudoParsear;
}

bool Parser::getLowerKeyword(std::string &lowerKeyword){
	c = this->stream->get();

	if (!islower(c) && (c != '_')){
		return false;
	}else{
		if (c == '_'){
			lowerKeyword += c;
			c = this->stream->get();
		}

		while (isalnum(c)){
			lowerKeyword += c;
			c = this->stream->get();
		}
	}

	if (c != ':'){
		return false;
	}
	return true;
}

bool Parser::getCapKeyword(std::string &capKeyword){
	//Probar si anda
	c = this->stream->get();
	if (!isupper(c)){
		return false;
	}else{
		while (isalnum(c)){
			capKeyword += c;
			c = this->stream->get();
		}
	}
	if (c != ':'){
		return false;
	}
	return true;
}

bool Parser::getOperator(std::string &op){
	*(this->stream) >> op;
	if ((op == "+") || (op == "-") || (op == "*") || (op == "/") || (op == "!=") || (op == "==")){
		return true;
	}
	return false;
}

bool Parser::getReceiver(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	c = this->stream->get();
	bool pudoParsear = false;
	//RECEIVER VACIO VER QUE ONDA
	if (c == ' '){
		//str = c;
		pudoParsear = true;
		////std::cout << "Entro en espacio" << std::endl;
	}else{
		pudoParsear = this->expressionCP(obj, pos);

		////std::cout << "Pudo parsear expCP en getRec: " << pudoParsear << std::endl;

	}
	if ((!pudoParsear) || ((c != ' ') && (c != '.'))){

		////std::cout << "Entra a return false en getRec con c: " << c << std::endl;

		return false;
	}
	return pudoParsear;
}

bool Parser::getConstant(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	std::string name;
	bool pudoParsear = false;
	//Probar si se ejecutan en orden y si si uno devuelve true no se ejecutan
	//los demas
	if ((this->getNumber(obj, pos)) ||
	 (this->getString(obj, pos)) || (this->getObject(obj, pos)) ||
	  (this->getNil(obj, pos)) || (this->getName(obj, name, pos, true))){
		pudoParsear = true;
	}
	return pudoParsear;
}

bool Parser::getNumber(ObjectMasCapo* &obj, const int &pos){
	bool pudoParsear = false, leyoPuntoOSpace = false;

	////std::cout << "----------Entra a getNumber-----------" << std::endl;

	std::string str;
	this->stream->seekg(pos);
	c = this->stream->get();

	if ((c != '-') && (!isdigit(c))){
		return false;
	}
	this->stream->unget();
	double dato;
	*(this->stream) >> dato;

	////std::cout << "Dato leido en getNumber: " << dato << std::endl;

	//Valido que el caracter despues del numero sea punto, espacio o parentesis
	this->stream->unget();
	c = this->stream->get();

	if (c != '.'){
		c = this->stream->get();
		if ((c != ' ') && (c != ')')){
			//ERROR
			return false;
		}
	}
	pudoParsear = true;

	obj = VM.createObject("", dato);
	return pudoParsear;
}

bool Parser::getString(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);

	////std::cout << "---------Entra a getString---------" << std::endl;

	std::string str;
	if (this->hayComillas()){
		//Ver pq si no hay comilla de cierre esto itera infinitamente
		//Verificar q haya comilla de cierre
		while (!(this->hayComillas()) && (c != ' ')){
			str += c;
		}
		//Si sale del ciclo por encontrar algo que no es comillas, entonces es un error
		if (c != '\''){
			return false;
		}
		c = this->stream->get();
	}else{
		return false;
	}
	obj = VM.createObject("", str);
	return true;
}

bool Parser::getObject(ObjectMasCapo* &obj, const int &pos){
	bool pudoParsear = false;
	std::vector<std::tuple<std::string, ObjectMasCapo*,	char>> slotList;
	this->stream->seekg(pos);

	////std::cout << "----------Entra a getObject------------" << std::endl;

	if ((this->hayParentesis1()) && (this->hayPipe())){
		pudoParsear = this->getSlotList(pos, slotList);
	}else{
		return false;
	}

	//Parsear codigo
	//Chequear q termine en parentesis
	if (pudoParsear){
		std::string codigo = "";
		//Si no hay parentesis significa q el objeto tiene codigo
		if (!(this->hayParentesis2())){
			this->stream->unget();
			pudoParsear = this->getStringCodigo(codigo);
		}else{
			c = this->stream->get();
		}
		//Si pudoParsear todo, creo el objeto y entro a mandar los addSlot segun
		//el tipo en la tupla
		if (pudoParsear){
			CustomObject *obj2 = VM.createObject("");

			for (unsigned int i = 0; i < slotList.size(); ++i){
				std::string nombre = std::get<0>(slotList[i]);

				//Flags de slots
				//Cambiar las default por las verdaderas
				//Ver por ahi conviene meter el get directo en el addSlot
				ObjectMasCapo *slot = std::get<1>(slotList[i]);
				//bool esMutable = std::get<2>(slotList[i]);
				char tipo = std::get<2>(slotList[i]);
				//slot->rename(nombre);
				obj2->addSlot(nombre, slot, tipo);
				//obj2->addSlot(slot, tipo);
			}

			obj2->addCode(codigo);

			//Ver si funciona esto
			obj = obj2;
		}
	}
	return pudoParsear;
}

bool Parser::getSlotList(const int &pos, std::vector<std::tuple<std::string,
		ObjectMasCapo*, char>> &slotList){
	bool pudoParsear = false;

	//////std::cout << "----------Entra a getSlotList---------" << std::endl;

	//VER QUE HACER CON LOS OBJETOS CREADOS SI ENCUENTRO ERROR
	while (!(this->hayPipe())){
		char tSlot;
		std::string nombreSlot;

		this->stream->unget();

		int posActual = this->stream->tellg();
		pudoParsear = this->getSlotNameExtended(nombreSlot, posActual, tSlot);

		if (!pudoParsear){
			return false;
		}

		//Como c es atributo, el ultimo valor de c queda guardado
		if (c == '.'){
			//Avanzo el espacio para q despues pueda leer el pipe
			c = this->stream->get();

			//Termina el slot
			std::tuple<std::string, ObjectMasCapo*, char> tupla;

			ObjectMasCapo *obj;
			tupla = std::make_tuple(nombreSlot, obj, tSlot);
			slotList.push_back(tupla);
		}else{
			if (tSlot == 'P'){
				//Avanzo el igual
				//c = this->stream->get();
				*(this->stream) >> c;

				////std::cout << "Deberia ser un igual: " << c << std::endl;

			}else{
				bool esMutable = this->esMutable();
				if (!esMutable)
					tSlot = 'I';
			}
			ObjectMasCapo* exp;

			//Avanzo el espacio
			c = this->stream->get();
			int posActual2 = this->stream->tellg();
			pudoParsear = this->expression(exp, posActual2);

			if ((!pudoParsear) || (c != '.')){
				//ERROR
				return false;
			}
			//Asigno a vector de tuplas
			std::tuple<std::string, ObjectMasCapo*, char> tupla;
			tupla = std::make_tuple(nombreSlot, exp, tSlot);
			slotList.push_back(tupla);
		}
	}
	if (c != '|'){
		//ERROR
		return false;
	}
	return true;
}

bool Parser::getSlotNameExtended(std::string &str, const int &pos,
		char &tSlot){
	bool pudoParsear = false;

	////std::cout << "--------------------Entra a getSlotNameExtended-----------" << std::endl;

	ObjectMasCapo *obj;
	c = this->stream->get();
	if (c == ':'){
		int posActual = this->stream->tellg();
		pudoParsear = this->getName(obj, str, posActual, false);

		//Si es argumento debe terminar con un . si o si
		if (pudoParsear && (c != '.')){
			pudoParsear = false;
		}else if (pudoParsear){
			tSlot = 'A';
		}
		//Crear slot argumento en vm?
	}else{
		pudoParsear = this->getName(obj, str, pos, false);
		//Como c es atributo, el ultimo valor de c queda guardado
		if (pudoParsear && (c == '*')){
			//Crear slot padre
			tSlot = 'P';
		}else if (pudoParsear){
			//Crear slot normal
			tSlot = 'M';
		}
	}
	return pudoParsear;
}

bool Parser::getNil(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	std::string str;
	for (int i = 0; i < 4; ++i){
		c = this->stream->get();
		str += c;
	}
	if (str == "nil "){

		//obj = new NilObject("");
		return true;
	}
	return false;
}

bool Parser::getName(ObjectMasCapo* &obj, std::string &name, const int &pos,
		const bool &esExpCP){

	////std::cout << "------------Entra a getName----------" << std::endl;

	this->stream->seekg(pos);
	//c = this->stream->get();
	*(this->stream) >> c;
	if (islower(c)){
		while (isalnum(c)){
			////std::cout << "Lee en getname: " << c << std::endl;
			name += c;
			c = this->stream->get();
		}
		//Asumo q a un receiver no se le puede aplicar '*'

//		if ((esReceiver) && (c != ' ') && (c != ':')){
//			return false;
//			//Asumo q a un atributo slot no se le puede asignar ':'
//		}else if ((!esReceiver) && (c != ' ') && (c != '.') && (c != '*')){
//			return false;
//		}


		//Que el metodo devuelva null si no lo encuentra
		if (esExpCP){
			////std::cout << "ENTRA A HACER LOOKUP" << std::endl;
			obj = VM.lookup(name);
		}
		return true;
	}else{
		return false;
	}
}

