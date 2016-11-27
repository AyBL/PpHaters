#ifndef GTKMM_ERRORWINDOW_H
#define GTKMM_ERRORWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <string>

class ErrorWindow : public Gtk::Window{
    public:
        ErrorWindow(std::string message);
        virtual ~ErrorWindow();

    protected:
        //Signal handlers:
        void on_button_close();

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Label m_Label;
        Gtk::Button m_Button_Close;
};

#endif
