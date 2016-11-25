#ifndef INITIALMENU_H
#define INITIALMENU_H

#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include "clientmodelcolumnsmenu.h"
#include <vector>
#include <cstring>

class InitialMenu : public Gtk::Window{
    public:
        InitialMenu(bool &run, char *ip,char *port, std::string &imagens);
        virtual ~InitialMenu();

    protected:
        //Signal handlers:
        void on_button_accept();
        void on_button_quit();
        void on_combo_changed();
        void load_rows();
        ModelColumnsMenu m_Columns;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;
        Gtk::Box m_HBox2;
        Gtk::ComboBox m_Combo;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
        Gtk::Label m_Label_IP;
        Gtk::Label m_Label_Port;
        Gtk::Label m_Label_Imagen;
        Gtk::Entry m_Entry_IP;
        Gtk::Entry m_Entry_Port;
        Gtk::Button m_Button_Accept;
        Gtk::Button m_Button_Quit;
        
        bool &run;
        char *ip;
        char *port;
        std::string &imagen,nameimagenaux;
        std::vector<std::string> names;

};

#endif
