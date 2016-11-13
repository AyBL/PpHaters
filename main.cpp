#include "clientwindowobject.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include <vector>

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create(argc, argv, "Window Lobbies");
	auto app2 = Gtk::Application::create(argc, argv, "Lobby");

	std::vector<std::tuple<unsigned char,std::string>> lobbies;
	std::string name;
	lobbies.push_back(std::make_tuple(0,"Hola"));
	lobbies.push_back(std::make_tuple(1,"Puto"));

	MenuWindow window2(lobbies,name);

	//Shows the window and returns when it is closed.
	app->run(window2);

	WindowObject window(argc,argv,name);
	// Shows the window and returns when it is closed.

	window.AddObject("Hola");
	window.AddSlot("Hola",std::make_tuple("yo","10",true,"int"));

	return app2->run(window);
}
