#ifndef OBJECTMASCAPO_H_
#define OBJECTMASCAPO_H_

#include <string>
#include <map>

#include "json/json.h"
class ObjectMasCapo {
protected:
	std::string objectName;
	// ver lo de flag con char dirctamente
	bool inmutable = false;
	bool parent = false;
	bool argument = false;
	std::string code = "";

	bool temporal = true;


public:
	ObjectMasCapo(std::string objectName);
	virtual ~ObjectMasCapo();
	virtual ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arg)=0; //{return this;}
	virtual ObjectMasCapo* clone(std::map<std::string, ObjectMasCapo*> arg)=0; //{return this;}
	void setFlags(char flag);
	char getFlag() const;
	void rename(std::string newName);
	std::string getName() const;

	bool isParent() const;
	void notTemp();
	void yesTemp();
	bool isTemp() const;

	virtual std::string getValue()=0;

//	void addMethod(std::string newMethodName,std::map<std::string,ObjectMasCapo*> arguments);
//	void removeMethod(std::string methodToRemove);

	virtual ObjectMasCapo* lookup(std::string slotName);

	std::string getCode() const;
	virtual std::vector<std::string> getIndex();
	virtual ObjectMasCapo* execute(std::string method,
			std::map<std::string, ObjectMasCapo*> arguments)=0;

	//PARA SERIALIZACION
	virtual Json::Value toJson();
};

#endif /* OBJECTMASCAPO_H_ */
