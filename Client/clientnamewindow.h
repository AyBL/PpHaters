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
        NameWindow(std::string &name, bool &create);
        virtual ~NameWindow();

    protected:
        //Signal handlers:
        void on_button_accept();
        void on_button_cancel();

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;
        Gtk::Box m_HBox2;
        Gtk::Label m_Label;
        Gtk::Entry m_Entry;
        Gtk::Button m_Button_Accept;
        Gtk::Button m_Button_Cancel;

        std::string &name;
        bool &create;
};

#endif
