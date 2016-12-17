#include "myarea.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <string>

#define WIDTH 1024
#define HEIGHT 500

int main(int argc, char* argv[]){

	/*Instanciamos un objeto de clase Gtk::Application el cual se 
	utiliza para lanzar la aplicacion. Recibe como argumentos 
	el argc y argv el cual debe estar vacio y un ID de la aplicacion
	cuyo nombre debe seguir ciertas reglas.*/
  	auto app = Gtk::Application::create(argc, argv, "my.rombo");

  	//Instanciamos un widget Window, este es widget que lanza la aplicacion.
  	Gtk::Window window;

  	//Seteamos un titulo
	window.set_title("Rombo");

	//Seteamos el tamaño
	window.set_default_size(WIDTH, HEIGHT);

	//Instanciamos nuestro widget personalizado
	MyArea area(WIDTH, HEIGHT);

	//Agregamos nuestro widget al widget Window
	window.add(area);

	//Hacemos visible nuestro widget
	area.show();

	//Lanzamos la aplicacion
	return app->run(window);
}