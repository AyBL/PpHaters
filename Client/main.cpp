
#include "clientwindowobject.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include "clientServerproxy.h"
#include <vector>

int main(int argc, char *argv[]){

	auto app = Gtk::Application::create(argc, argv, "window.lobbies");
	auto app2 = Gtk::Application::create(argc, argv, "one.lobby");
	char ip[100];
	char port[10];

	std::string name;

	WindowObject *window;
	MenuWindow *menuwindow;

	Serverproxy proxy(&menuwindow,&window);

	std::cout << "Ingrese IP: ";
	std::cin >> ip;
	std::cout << "Ingrese PORT: ";
	std::cin >> port;

	proxy.Connect(ip,port);

	menuwindow = new MenuWindow(name,proxy);

	proxy.ReceiveAnswer();

	app->run(*menuwindow);

	delete(menuwindow);

	window = new WindowObject("/usr/bin/Iron-Throne.jpg",argc,argv,name,proxy);

	proxy.start();

	app2->run(*window);

	delete(window);

	proxy.Stop();

	proxy.join();

	return 0;
}
