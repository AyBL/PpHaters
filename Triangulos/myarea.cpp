#include "myarea.h"

MyArea::MyArea(){
}

MyArea::~MyArea(){
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){

	Gtk::Allocation allocation = get_allocation();

	//Obtenemos tamaño de la ventana
	width = allocation.get_width();
	height = allocation.get_height();

	//Seteamos el color a blanco
	cr->set_source_rgb(1, 1, 1);

	cr->paint();

	//Seteamos el color a negro
	cr->set_source_rgb(0, 0, 0);

	//Nos movemos a la siguiente posicion
	cr->move_to(width,0);

	//trazamos linea
	cr->line_to(width,height);
	cr->line_to(0,height);
	cr->line_to(width,0);

	//rellenamos
	cr->fill();

  	return Gtk::DrawingArea::on_draw(cr);
}