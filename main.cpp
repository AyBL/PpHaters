
#include "clientwindowobject.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include "clientServerproxy.h"
#include <vector>

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create(argc, argv, "Window Lobbies");
	auto app2 = Gtk::Application::create(argc, argv, "Lobby");
	const char *ip = "127.0.0.1";
	const char *port = "9090";

	std::string name;

	WindowObject *window;
	MenuWindow *menuwindow;

	Serverproxy proxy(&menuwindow,&window);

	proxy.Connect(ip,port);

	menuwindow = new MenuWindow(name,proxy);

	proxy.ReceiveAnswer();

	app->run(*menuwindow);

	delete(menuwindow);

	window = new WindowObject(argc,argv,name,proxy);

	proxy.start();
	window->AddObject("Hola",50,50);
	window->AddSlot("Hola",std::make_tuple("yo","10","int",'m',""));

	app2->run(*window);

	delete(window);

	proxy.Stop();

	proxy.join();

	return 0;
}
