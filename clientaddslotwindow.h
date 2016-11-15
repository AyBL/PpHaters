#ifndef ADDSLOTWINDOW_H
#define ADDSLOTWINDOW_H


#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

class AddSlotWindow : public Gtk::Window{
    public:
        AddSlotWindow(std::string &slot);
        virtual ~AddSlotWindow();

    protected:
        //Signal handlers:
        void on_button_close();
        void on_button_create();

        //Child widgets:
        Gtk::Box m_HBox;
        Gtk::Box m_VBox;
        Gtk::Entry m_Entry;
        Gtk::Label m_Label;
        Gtk::Button m_Button_Close;
        Gtk::Button m_Button_Create;
        std::string &slot;
};

#endif
