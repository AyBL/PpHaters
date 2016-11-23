#include "clientaddobjectaction.h"
#include "clientwindowobject.h"

AddObjectAction::AddObjectAction(WindowObject* window, std::string name, int x, int y):
window(window),name(name),x(x),y(y){
}

AddObjectAction::~AddObjectAction(){
}

void AddObjectAction::run(){
	window->AddObject(name,x,y);
}
