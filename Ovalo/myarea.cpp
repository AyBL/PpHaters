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

	cr->set_source_rgb(0, 0, 0);

	cr->paint();

	cr->save();

	//Trasladamos el centro a este punto
	cr->translate(width/2,height/2);

	//Cambiamos las escalas
	cr->scale(width / 2.0, height / 2.0);

	//trazamos un ovalo
	cr->arc(0,0,1.0,0,2*M_PI); 

	//Seteamos el color a blanco
	cr->set_source_rgb(1, 1, 1);

	//rellenamos
	cr->fill_preserve();

	cr->restore();

	cr->set_source_rgb(0, 1, 0);

	//cr->set_line_width(1);

	cr->stroke();

  	return Gtk::DrawingArea::on_draw(cr);
}