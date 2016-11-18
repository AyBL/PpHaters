#include "clientremoveobjectaction.h"
#include "clientwindowobject.h"

RemoveObjectAction::RemoveObjectAction(WindowObject *window,std::string name):
window(window), name(name){
}

RemoveObjectAction::~RemoveObjectAction(){
}

void RemoveObjectAction::run(){
	window->RemoveObject(name);
}
