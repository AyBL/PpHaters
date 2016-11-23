
#include "clientwindowobject.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include "clientServerproxy.h"
#include <vector>

int main(int argc, char *argv[]){
	auto app = Gtk::Application::create(argc, argv, "window.lobbies");
	auto app2 = Gtk::Application::create(argc, argv, "one.lobby");
	const char *ip = "127.0.0.1";
	const char *port = "9999";

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

	app2->run(*window);

	delete(window);

	proxy.Stop();

	proxy.join();

	return 0;
}
