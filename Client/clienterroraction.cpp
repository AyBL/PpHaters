#include "clienterroraction.h"
#include "clientwindowobject.h"

ErrorAction::ErrorAction(WindowObject* window, std::string message):
window(window),message(message){
}

ErrorAction::~ErrorAction(){
}

void ErrorAction::run(){
	window->ErrorMessage(message);
}
