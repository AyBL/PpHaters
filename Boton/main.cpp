#include "mybutton.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <string>

int main(int argc, char* argv[]){

	/*Instanciamos un objeto de clase Gtk::Application el cual se 
	utiliza para lanzar la aplicacion. Recibe como argumentos 
	el argc y argv el cual debe estar vacio y un ID de la aplicacion
	cuyo nombre debe seguir ciertas reglas.*/
  	auto app = Gtk::Application::create(argc, argv, "my.button");

	//Instanciamos nuestro widget personalizado
	MyButton button;

	//Lanzamos la aplicacion
	return app->run(button);
}