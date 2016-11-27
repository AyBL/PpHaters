#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include "clientmodelcolumnsmenu.h"
#include <vector>
#include <tuple>

class Serverproxy;

class MenuWindow : public Gtk::Window{
    public:
        MenuWindow(std::string &name,Serverproxy &proxy);
        virtual ~MenuWindow();
        void AddLobbies(std::tuple<unsigned char,std::string> newlobbies);

    protected:
        //Signal handlers:
        void on_combo_changed();
        void on_button_accept();
        void load_rows(); 
        ModelColumnsMenu m_Columns;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::ComboBox m_Combo;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
        Gtk::Label m_Label;
        Gtk::Label m_Label2;
        Gtk::Entry m_Entry;
        Gtk::Button m_Button;

        std::vector<std::tuple<unsigned char,std::string> > lobbies;

        unsigned char id;
        std::string &name;
        Serverproxy &proxy;
};

#endif