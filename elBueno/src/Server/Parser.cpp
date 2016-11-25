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
Parser::Parser(std::istream *stream, VirtualMachine &VM) : VM(VM), stream(stream), c() {
	_receiver = NULL;
}

Parser::~Parser() {
/*	if (!(this->stream.fail())){
		this->stream.close();
	}*/
}

bool Parser::hayParentesis1(){
	*(this->stream) >> c;
	return (c == '(');
}

bool Parser::hayParentesis2(){
	*(this->stream) >> c;

	std::cout << "Hay parentesis: " << c << std::endl;

	return (c == ')');
}

bool Parser::hayComillas(){
	c = this->stream->get();
	return (c == '\'');
}

bool Parser::hayPipe(){
	//c = this->stream->get();
	*(this->stream) >> c;

	std::cout << "C en hayPipe: " << c << std::endl;

	return (c == '|');
}

bool Parser::hayDelimitador(){
	c = this->stream->get();
	return  (c == ':') || (c == ' ') || (c == '.') || (c == '*');
//		return true;
//	}
//	return false;
}

bool Parser::finStringCodigo(){
	//c = this->stream->get();
	int posActual = this->stream->tellg();
	if (c == '.'){
		*(this->stream) >> c;
		if (c == ')'){
			*(this->stream) >> c;
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
	std::string str;
//	c = this->stream->get();
	*(this->stream) >> str;

	std::cout << "Lee en esMutable: " << str << std::endl;

	if (str == "="){
		return false;
	}else if (str == "<-"){
		return true;
	}else{
		//ERROR
		std::cout << "Solo se acepta '=' o '<-' para describir un slot"
				<< std::endl;
		throw(-1);
	}
	return false;
}

bool Parser::getStringCodigo(std::string &codigo){
	//LEO CARACTER A CARACTER, MUY LENTO??? VER
	//Cuando lee .). significa q el codigo del objeto termino
	while (!(this->finStringCodigo())){
		c = this->stream->get();
		codigo += c;
	}

	std::cout << "-----Codigo en getStringCod: " << codigo << std::endl;
	//VER COMO HACER PARA DETECTAR ERROR DE SINTAXIS
	//NO REGISTRA LOS ERRORES CUANDO VM LLAMA A PARSER PARA EJECUTAR CODIGO DE OBJETO
	//ES PQ NO PARSEA BIEN ACA, SI NO ENCUENTRA .). SE TRABA
	//CLAVAR TRY CATCH EN VM CUANDO LLAMA AL PARSER? MEDIO FEO
	return true;
}

ObjectMasCapo* Parser::script(){
	bool pudoParsear = true;
	int i = 0;
	ObjectMasCapo *exp = NULL;

	while ((pudoParsear) && (*(this->stream) >> c)){
		std::cout << "Lo que lee el while: " << c << std::endl;
		this->stream->unget();

		//Guardo la pos actual
		int posActual = this->stream->tellg();

		pudoParsear = this->expression(exp, posActual, false);

		std::cout << "---------------------------Iteracion en script: " << i << std::endl;
		++i;

		std::cout << "C en script: " << c << std::endl;

		_receiver = NULL;

		if (c == ' ')
			*(this->stream) >> c;

		if (c != '.'){
			std::cout << "Falta '.' de cierre" << std::endl;
			//ERROR SINTAXIS
			throw(-1);
		}
	}
	std::cout << "Recibe obj en script como: " << exp->getName() << std::endl;
	return exp;
}

bool Parser::expression(ObjectMasCapo* &exp, int &pos, const bool &esSlot){
	bool pudoParsear = false;
	std::string str;

	if ((this->keywordMessage(exp, pos)) || (this->binaryMessage(exp, pos)) ||
	 (this->unaryMessage(exp, pos, esSlot)) || (this->expressionCP(exp, pos))){
		pudoParsear = true;
	}
	std::cout << "PUDO PARSEAR EN EXPRESSION: " << pudoParsear << std::endl;
	return pudoParsear;
}

bool Parser::expressionCP(ObjectMasCapo* &obj, const int &pos){
	bool pudoParsear = true;

	std::cout << "-----ENTRO A EXPRESSIONCP------" << std::endl;

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

	std::cout <<"------ENTRA A EXPRESSIONP-----" << std::endl;

	this->stream->seekg(pos);
	if (!(this->hayParentesis1())){
		return false;
	}

	this->stream->unget();
	if ((this->hayParentesis1() && this->hayPipe())){
		return false;
	}
	this->stream->unget();
	int posActual = this->stream->tellg();
	bool pudoParsear = this->expression(obj, posActual, false);

	if (c == ' ')
		*(this->stream) >> c;
	if ((!pudoParsear) || (c != ')')){
		//ERROR
		std::cout << "Falta ')' " << std::endl;
		throw(-1);
		return false;
	}
	c = this->stream->get();
	return pudoParsear;
}

bool Parser::keywordMessage(ObjectMasCapo* &obj, int &pos){
	this->stream->seekg(pos);
	ObjectMasCapo *receiver, *receiverAux;
	std::string lowerKeyword, messageName;
	std::map<std::string, ObjectMasCapo*> argList;
	int posAux;

	std::cout << "---------ENTRA A NARIO-----------"<< std::endl;

	bool pudoParsear = this->getReceiver(receiver, pos);

	std::cout << "C despues de getReceiver: " << c << std::endl;

//	*(this->stream) >> c;

	//Si pudo parsear todo recien ahi llamo al metodo de la vm y paso las cosas
	//a los tipos necesarios
	if (pudoParsear){
		if ((c != ':') && (receiver != NULL)){
			_receiver = receiver;
			pos = this->stream->tellg();

			std::cout << "Entro a pudoParsear con rec != null y c != :" << std::endl;
			std::cout << "Entra con c: " << c << std::endl;

			//Si el receiver termina con punto entonces no es un kwMessage
			if (c == '.')
				return false;
		}else{

			std::cout << "Entro a pudoParsear con c igual a : o rec null" << std::endl;

			//Para que tome el : en caso de que haya
			this->stream->unget();
			posAux = this->stream->tellg();
			//Que vuelva a leer la palabra pero como lowerKeyword
			this->stream->seekg(pos);
			//Pierde mem?
			//Si el mensaje es en realidad un binario para cambiar el valor
			//del slot, debo guardar el receiver
			receiverAux = receiver;
			receiver = NULL;
		}
	}else{
		this->stream->unget();
		receiver = NULL;
	}

	pudoParsear = this->getLowerKeyword(lowerKeyword);

//	std::cout << "lower keyword: " << lowerKeyword << std::endl;

	if (!pudoParsear){
		this->stream->seekg(pos);
		if (receiver != NULL)
			c = this->stream->get();
		return false;
	}
	messageName += lowerKeyword;
	messageName += ':';

	//Avanzo un espacio
	c = this->stream->get();

	ObjectMasCapo* arg;
	int posActual = this->stream->tellg();

	_receiver = NULL;
	pudoParsear = this->expressionCP(arg, posActual);

	if (!pudoParsear){
		return false;
	}

	argList[lowerKeyword] = arg;

	//Todos los metodos de expressionCp leen un caracter despues de cuando
	//terminan lo que buscan, entonces ya tengo en c ese caracter
	while (pudoParsear && (c == ' ')){
		std::string capitalKeyword;
		pudoParsear = this->getCapKeyword(capitalKeyword);

		std::cout << "Capital keyw: " << capitalKeyword << std::endl;

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

	std::cout << "MESSAGE NAME: " << messageName << std::endl;

	obj = VM.message(receiver, messageName, argList);
	if (obj == NULL){
		_receiver = receiverAux;
		pos = posAux;
		return false;
	}
	return pudoParsear;
}

bool Parser::binaryMessage(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	ObjectMasCapo *receiver, *arg;
	std::string op;
	std::map<std::string, ObjectMasCapo*> argList;

	std::cout << "----------------Entra a mensaje binario-----------" << std::endl;

//	bool pudoParsear = this->getReceiver(receiver, pos);
//
//	if (!pudoParsear){
//		return false;
//	}
	receiver = _receiver;

	bool pudoParsear = this->getOperator(op);

	std::cout << "Operador leido: " << op << std::endl;

	if (!pudoParsear){
		//Me fijo si el receiver es vacio y lo primero q leyo fue un operator
		if (this->esOperator(receiver->getName())){
//			this->stream->seekg(comienzoExpresion);

			//ACA DEBERIA VERIFICAR QUE EL STRING OP ESTA COMPUESTO POR DIGITOS
			std::stringstream ss(op);
			double dato;
			ss >> dato;
			arg = VM.createObject("", dato);
		}else
			return false;
	}else{

		c = this->stream->get();

		if (c != ' '){
			//ERROR SINTAXIS
			std::cout << "Falta espacio" << std::endl;
			throw(-1);
			return false;
		}

		int posActual = this->stream->tellg();
		_receiver = NULL;
		pudoParsear = this->expressionCP(arg, posActual);

		if (!pudoParsear)
			return false;
	}

	if (op == ":"){
		bool pudoAsignar = VM.assignmentMessage(receiver->getName(), arg);
		if (pudoAsignar)
			VM.appendObject(arg);
		//Si fallo asignacion, devolver null?
		obj = NULL;
	}else{
		argList[op] = arg;
		obj = VM.message(receiver, op, argList);
		//VER SI LE PUEDO METER OB+CAPO *&obj CMO PARAMETRO AL APPEND MIENTRAS TANTO:
		ObjectMasCapo *obj2 = obj;
		//Hago que el obj se appendee al vector de ObjectsCreated de la VM
		VM.appendObject(obj2);
	}
	return pudoParsear;
}

bool Parser::unaryMessage(ObjectMasCapo* &obj, int pos, const bool &esSlot){
	this->stream->seekg(pos);
	bool pudoParsear;
	ObjectMasCapo *receiver, *aux;
	std::string messageName;
	std::map<std::string, ObjectMasCapo*> argList;

	std::cout << "------------------Entro a unario--------------" << std::endl;

//	bool pudoParsear = this->getReceiver(receiver, pos);
//
//	std::cout << "Pudo parsear receiver en unario: " << pudoParsear << std::endl;
//
//	if (!pudoParsear){
//		return false;
//	}
//	c = this->stream->get();
//	this->stream->unget();
	if (c == ' ')
		*(this->stream) >> c;

	if ((c == '.') && !esSlot){
		messageName = _messageName;
	}else if (esSlot){
		return false;
	}else{
		receiver = _receiver;
		_receiver = NULL;

		pudoParsear = this->getName(aux, messageName, pos, false);

		if (!pudoParsear){
			return false;
		}
	}
	obj = VM.message(receiver, messageName, argList);
	return pudoParsear;
}

bool Parser::getLowerKeyword(std::string &lowerKeyword){
//	c = this->stream->get();
	*(this->stream) >> c;
	if (!islower(c) && (c != '_'))
		return false;

	if (c == '_'){
		lowerKeyword += c;
		c = this->stream->get();
	}

	while (isalnum(c)){
		lowerKeyword += c;
		c = this->stream->get();
	}

	return c == ':';

//	if (c != ':'){
//		//ERROR
//		return false;
//	}
//	return true;
}

bool Parser::getCapKeyword(std::string &capKeyword){
	//Probar si anda
	c = this->stream->get();
	if (!isupper(c))
		return false;
//	}else{
	while (isalnum(c)){
		capKeyword += c;
		c = this->stream->get();
	}
//	}
	return (c == ':');
//	if (c != ':'){
//		return false;
//	}
//	return true;
}

bool Parser::esOperator(const std::string &str){
	return OPERADORES.count(str) == 1;
}

bool Parser::getOperator(std::string &op){
	*(this->stream) >> op;
	return this->esOperator(op);
}

bool Parser::getReceiver(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	c = this->stream->get();
	bool pudoParsear = false;

	pudoParsear = this->expressionCP(obj, pos);
	//Por ahi al pedo validar que viene despues del receiver, igual si hay que hacerlo
	//poner todos los operadores tamb
//	if ((!pudoParsear) || ((c != ' ') && (c != '.') && (c != ':'))){
//		return false;

	return pudoParsear;
}

bool Parser::getConstant(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);
	std::string name;
//	bool pudoParsear = false;
	//Probar si se ejecutan en orden y si si uno devuelve true no se ejecutan
	//los demas
	return this->getNumber(obj, pos) ||
	       this->getString(obj, pos) ||
	       this->getObject(obj, pos) ||
	       this->getNil(obj, pos) ||
	       this->getName(obj, name, pos, true);

//	return pudoParsear;
}

bool Parser::getNumber(ObjectMasCapo* &obj, const int &pos){
	std::cout << "----------Entra a getNumber-----------" << std::endl;

	std::string str;
	this->stream->seekg(pos);
//	c = this->stream->get();
	*(this->stream) >> c;
	if ((c != '-') && (!isdigit(c))){
		return false;
	}
	this->stream->unget();
	double dato;
	*(this->stream) >> dato;

	std::cout << "Dato leido en getNumber: " << dato << std::endl;

	//Valido que el caracter despues del numero sea punto o parentesis
	//(Acepta que haya espacios hasta el . o ) )

	//VER SI ES NECESARIO VALIDAR

	this->stream->unget();
	*(this->stream) >> c;

	std::cout << "C despues de unget en getNumber: " << c << std::endl;

	if (c != '.'){
		std::string str;
		//Si es operador vuelvo atras para que lo lea el metodo correspondiente
		if (this->getOperator(str)){
			for (int i = 0; i < str.size(); ++i){
				this->stream->unget();
			}
		}else{
			this->stream->unget();
			*(this->stream) >> c;
		}
		std::cout << "Como no hay . lee: " << c << std::endl;

	}

//	if ((c != '.') && (c != ')')){
//		return false;
//	}

	obj = VM.createObject("", dato);
	return true;
}

bool Parser::getString(ObjectMasCapo* &obj, const int &pos){
	this->stream->seekg(pos);

	std::cout << "---------Entra a getString---------" << std::endl;

	std::string str;
	*(this->stream) >> c;
	if (c == '\''){
		while (!(this->hayComillas()) && (c != ' ')){
			str += c;
		}
		//Si sale del ciclo por encontrar algo que no es comillas, entonces es un error
		if (c != '\''){
			if (c == ' '){
				//ERROR SINTAXIS
				std::cout << "No se aceptan espacios en strings" << std::endl;
			}else{
				//ERROR SINTAXIS
				std::cout << "Faltan comillas de cierre en el string" << std::endl;
			}
			throw(-1);
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

	std::cout << "----------Entra a getObject------------" << std::endl;

	if (this->hayParentesis1() && this->hayPipe()){
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
				ObjectMasCapo *slot = std::get<1>(slotList[i]);
				char tipo = std::get<2>(slotList[i]);
				ObjectMasCapo *objAgregado = obj2->addSlot(nombre, slot, tipo);
				if (tipo == 'P')
					VM.appendObject(objAgregado);
			}

			std::cout << "Llega al final de recorrer el slotList" << std::endl;

			obj2->addCode(codigo);

			//Ver si funciona esto
			obj = obj2;

			std::cout << "Llega al final de crear el Object" << std::endl;

		}
	}
	return pudoParsear;
}

bool Parser::getSlotList(const int &pos, std::vector<std::tuple<std::string,
		ObjectMasCapo*, char>> &slotList){
	bool pudoParsear = false;

	std::cout << "----------Entra a getSlotList---------" << std::endl;

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
				*(this->stream) >> c;
			}else{
				bool esMutable = this->esMutable();
				if (!esMutable)
					tSlot = 'I';
			}
			ObjectMasCapo* exp;

			//Avanzo el espacio
			c = this->stream->get();
			int posActual2 = this->stream->tellg();
			pudoParsear = this->expression(exp, posActual2, true);

			if (!pudoParsear){
				return false;
			}else if (c != '.'){
				if (c == ' '){
					*(this->stream) >> c;
					if (c != '.'){
						//ERROR
						std::cout << "Falta '.' para indicar fin del slot" << std::endl;
						throw(-1);
					}
				}else{
					std::cout << "Falta '.' para indicar fin del slot" << std::endl;
					throw(-1);
				}
			}
			//Asigno a vector de tuplas
			std::tuple<std::string, ObjectMasCapo*, char> tupla;
			tupla = std::make_tuple(nombreSlot, exp, tSlot);
			slotList.push_back(tupla);
		}
	}
	//VER PQ NO LO REGISTRA SI NO QUE SIGUE EN EL WHILE LOGICAMENTE
	if (c != '|'){
		//ERROR
		std::cout << "Falta '|' para indicar fin de lista de slots" << std::endl;
		throw(-1);
		return false;
	}
	return true;
}

bool Parser::getSlotNameExtended(std::string &str, const int &pos,
		char &tSlot){
	bool pudoParsear = false;

	std::cout << "--------------------Entra a getSlotNameExtended-----------" << std::endl;

	ObjectMasCapo *obj;
//	c = this->stream->get();
	*(this->stream) >> c;
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
		}else{
			//Puede ser que quiera redefinir un operador
			this->stream->unget();
			pudoParsear = this->getOperator(str);
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

bool Parser::getName(ObjectMasCapo* &obj, std::string &name, int pos,
		bool esExpCP){

	std::cout << "------------Entra a getName----------" << std::endl;

	this->stream->seekg(pos);
	*(this->stream) >> c;

	if (!islower(c))
		return false;

//	if (islower(c)){
	while (isalnum(c)){
		name += c;
		c = this->stream->get();
	}

	std::cout << "Name en getName: " << name << std::endl;

	if (name == "true"){
		obj = VM.createObject("", true);
	}else if (name == "false"){
		obj = VM.createObject("", false);
	}else if (esExpCP){
		obj = VM.lookup(name);
	}
	this->_messageName = name;
	return true;
//	}else{
//		return false;
//	}
}
