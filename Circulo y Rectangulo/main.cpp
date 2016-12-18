#include "myarea.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

int main(int argc, char **argv){
	auto app = Gtk::Application::create(argc,argv,"my.area");

	Gtk::Window window;
	MyArea area;

	window.add(area);

	area.show();

	return app->run(window);
}