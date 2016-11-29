#ifndef MOVEOBJECT_H
#define MOVEOBJECT_H

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

class MoveObject : public Gtk::Window{
    public:
        MoveObject(unsigned char &id, std::string &name);
        virtual ~MoveObject();
        void AddLobbies(std::tuple<unsigned char,std::string> newlobbies);
        void AddObjects(std::tuple<unsigned char,std::string> newobjects);

    protected:
        //Signal handlers:
        void on_combo_changed_lobby();
        void on_combo_changed_object();
        void on_button_accept();
        void load_rows(); 
        ModelColumnsMenu m_Columns_Lobby;
        ModelColumnsMenu m_Columns_Object;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::ComboBox m_Combo_Lobby;
        Gtk::ComboBox m_Combo_Object;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModelLobbies;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModelObjects;
        Gtk::Label m_Label;
        Gtk::Label m_Label2;
        
        Gtk::Button m_Button;

        std::vector<std::tuple<unsigned char,std::string> > lobbies;
        std::vector<std::tuple<unsigned char,std::string> > objects;

        unsigned char &id;
        std::string &name;

        unsigned char idlobby;
        std::string nameobject;
};

#endif