#ifndef OBJECTMASCAPO_H_
#define OBJECTMASCAPO_H_

#include <string>
#include <map>

class ObjectMasCapo {
protected:
	std::string objectName;
	bool inmutable=false;
	bool parent=false;
	bool argument=false;
//	std::map<std::string,std::map<std::string, ObjectMasCapo*>> methods;
	std::string code = "";
//	enum TipoSlot {"ARG","PADRE","MUTABLE","INMUTABLE"};

//	typedef ObjectMasCapo* (ObjectMasCapo::*MFP)(std::map<std::string, ObjectMasCapo*>);
//	std::map<std::string, MFP> methods;

public:
	ObjectMasCapo(std::string objectName);
	virtual ~ObjectMasCapo();
	virtual ObjectMasCapo* print(std::map<std::string, ObjectMasCapo*> arg)=0; //{return this;}
	virtual ObjectMasCapo* clone(std::map<std::string,ObjectMasCapo*> arg)=0;//{return this;}
	void setFlags(char flag);
	void rename(std::string newName);
	std::string getName();

	bool isParent();

//	void addMethod(std::string newMethodName,std::map<std::string,ObjectMasCapo*> arguments);
//	void removeMethod(std::string methodToRemove);

	virtual ObjectMasCapo* lookup(std::string slotName);

	std::string getCode();
	virtual ObjectMasCapo* execute(std::string method, std::map<std::string, ObjectMasCapo*> arguments)=0;
};

#endif /* OBJECTMASCAPO_H_ */
