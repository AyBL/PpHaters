/*
 * Parser.h
 *
 *  Created on: 26/10/2016
 *      Author: sofia
 */

#ifndef PARSER_H_
#define PARSER_H_

	#include <fstream>
	#include <cstdlib>
	#include <iostream>
	#include <istream>
	#include <string>
	#include <vector>
	#include <tuple>
	#include <set>

	#include "server_VirtualMachine.h"

	class Parser {
	private:
		VirtualMachine &VM;
		//Char actual que estoy leyendo
		char c;
		std::istream *stream;
/*		enum Tipo { INT, DOUBLE, STRING, OBJECT, NIL, NAME, NARIO, BINARIO,
			UNARIO};*/
//		enum TipoSlot { ARG, PADRE, MUTABLE, INMUTABLE};
		ObjectMasCapo *_receiver;
		std::string _messageName;
		const std::set<std::string> OPERADORES{"+", "-", "*", "/", "!=", "==", ":"};

	public:
		Parser(std::istream *stream, VirtualMachine &VM);
		~Parser();
		bool hayParentesis1();
		bool hayParentesis2();
		bool hayComillas();
		bool hayPipe();
		bool hayDelimitador();
		bool finStringCodigo();
		bool esMutable();
		bool getStringCodigo(std::string &codigo);
		ObjectMasCapo* script();
		bool expression(ObjectMasCapo* &exp, int &pos, const bool &esSlot);
		bool expressionCP(ObjectMasCapo* &obj, const int &pos);
		bool expressionP(ObjectMasCapo* &obj, const int &pos);
		bool keywordMessage(ObjectMasCapo* &obj, int &pos);
		bool binaryMessage(ObjectMasCapo* &obj, const int &pos);
		bool unaryMessage(ObjectMasCapo* &obj, int pos,
				const bool &esSlot);
		bool getLowerKeyword(std::string &lowerKeyword);
		bool getCapKeyword(std::string &capKeyword);
		bool esOperator(const std::string &str);
		bool getOperator(std::string &op);
		bool getReceiver(ObjectMasCapo* &obj, const int &pos);
		bool getConstant(ObjectMasCapo* &obj, const int &pos);
		bool getNumber(ObjectMasCapo* &obj, const int &pos);
		bool getString(ObjectMasCapo* &obj, const int &pos);
		bool getObject(ObjectMasCapo* &obj, const int &pos);
		bool getSlotList(const int &pos, std::vector<std::tuple<std::string,
				ObjectMasCapo*, char>> &slotList);
		bool getSlotNameExtended(std::string &str, const int &pos,
				char &tSlot);
		bool getNil(ObjectMasCapo* &obj, const int &pos);
		bool getName(ObjectMasCapo* &obj, std::string &name, int pos,
				bool esExpCP);
	};

#endif /* PARSER_H_ */
