#include <iostream>
#include "clientwindowobject.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include "clientServerproxy.h"
#include "clientinitialmenu.h"
#include <vector>

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create(argc, argv, "initial.menu");
	auto app1 = Gtk::Application::create(argc, argv, "window.lobbies");
	auto app2 = Gtk::Application::create(argc, argv, "one.lobby");

	char ip[100];
	char port[10];
	bool run;
	std::string nameimagen;
	std::string name;

	std::memset(ip,0,100);
	std::memset(port,0,10);

	InitialMenu menu(run,ip,port,nameimagen);

	WindowObject *window;
	MenuWindow *menuwindow;

	Serverproxy proxy(&menuwindow,&window);

	app->run(menu);

	if (!run){
		std::cout << "Cancelado" << std::endl;
		return 1;
	}

	proxy.Connect(ip,port);

	menuwindow = new MenuWindow(name,proxy);

	proxy.ReceiveAnswer();

	app1->run(*menuwindow);

	delete(menuwindow);

	window = new WindowObject(nameimagen,argc,argv,name,proxy);

	proxy.start();

	app2->run(*window);

	delete(window);

	proxy.Stop();

	proxy.join();

	return 0;
}
