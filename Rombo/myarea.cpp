#include "myarea.h"

MyArea::MyArea(int width, int height):width(width),height(height){
}

MyArea::~MyArea(){
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){

	//Seteamos el color a negro
	cr->set_source_rgb(0, 0, 0);

	//Nos movemos a la siguiente posicion
	cr->move_to(width/2,0);

	//trazamos linea
	cr->line_to(width,height/2);
	cr->line_to(width/2,height);
	cr->line_to(0,height/2);
	cr->line_to(width/2,0);
	
	cr->set_source_rgb(1, 1, 1);
	//rellenamos
	cr->fill();

  	return Gtk::DrawingArea::on_draw(cr);
}