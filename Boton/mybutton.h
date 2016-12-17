#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>

class MyButton : public Gtk::Window{
	public:
		MyButton();
		virtual ~MyButton();

	protected:
		void button_close();

	private:
		Gtk::Button button;
};

#endif 