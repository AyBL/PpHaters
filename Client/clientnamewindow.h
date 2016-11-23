#ifndef GTKMM_NAMEWINDOW_H
#define GTKMM_NAMEWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <string>

class NameWindow : public Gtk::Window{
    public:
        NameWindow(std::string &name);
        virtual ~NameWindow();

    protected:
        //Signal handlers:
        void on_button_accept();

        //Child widgets:
        Gtk::Box m_HBox;
        Gtk::Label m_Label;
        Gtk::Entry m_Entry;
        Gtk::Button m_Button_Accept;

        std::string &name;
};

#endif
