#include "myarea.h"

MyArea::MyArea(){
}

MyArea::~MyArea(){
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	int xc,yc,radio,xr,yr,heightr,widthr,height,width;

	Gtk::Allocation allocation = get_allocation();

	height = allocation.get_height();
	width = allocation.get_width();

	xc = width/2;
	yc = height/2;

	radio = height/12;

	heightr = height/3;
	widthr = width/2;

	xr = width/4;
	yr = yc + radio;

	cr->set_source_rgb(1,1,1);
	cr->paint();

	cr->set_source_rgb(0,0,0);
	cr->set_line_width(1);

	cr->save();

	cr->rectangle(xr,yr,widthr,heightr);

	cr->set_source_rgb(0.5,0,0.5);

	cr->fill_preserve();

	cr->restore();

	cr->save();

	cr->stroke();

	cr->arc(xc,yc,radio,0,2*M_PI);

	cr->set_source_rgb(0.5,0.5,0.5);

	cr->fill_preserve();

	cr->restore();

	cr->stroke();

	return Gtk::DrawingArea::on_draw(cr);

}