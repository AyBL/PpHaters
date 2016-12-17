#ifndef MYAREA_H
#define MYAREA_H

#include <gtkmm/drawingarea.h>

class MyArea : public Gtk::DrawingArea{
	public:
		MyArea();
		virtual ~MyArea();

	protected:
		//Override default signal handler:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

	private:
		int width;
		int height;
};

#endif 