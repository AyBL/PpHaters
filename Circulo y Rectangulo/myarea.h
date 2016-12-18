#ifndef MYAREA_H
#define MYAREA_H

#include <gtkmm/drawingarea.h>
#include <math.h>

class MyArea: public Gtk::DrawingArea {
	public:
		MyArea();
		virtual ~MyArea();

	protected:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

};

#endif