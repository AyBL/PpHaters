#include "clientaddvalueaction.h"
#include "clientwindowobject.h"

AddValueAction::AddValueAction(WindowObject* window, std::string name, std::string value, int x, int y):
window(window),name(name),value(value),x(x),y(y){
}

AddValueAction::~AddValueAction(){
}

void AddValueAction::run(){
	window->AddValueObject(name,value,x,y);
}
