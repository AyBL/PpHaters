#ifndef WINDOWOBJECT_H
#define WINDOWOBJECT_H
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/fixed.h>
#include <gtkmm/menu.h>
#include "clientselfobject.h"
#include "clientnamewindow.h"
#include <map>
#include <arpa/inet.h>

#define HEIGHT 768
#define WIDTH 1366

class Serverproxy;

class WindowObject : public Gtk::Window{
    public:
        WindowObject(int argc, char **argv, std::string name,Serverproxy &proxy);
        virtual ~WindowObject();
        void AddObject(std::string name,int x, int y);
        void RemoveObject(std::string name);
        void AddSlot(std::string name, std::tuple<std::string, std::string,std::string,char,std::string> newslot);
        void RemoveSlot(std::string nameobject,std::string nameslot);
        void Move(std::string nameobject,int x, int y);

    protected:
        //Signal handlers:
        // void on_button_move();
        bool on_button_press_event(GdkEventButton* button_event) override;
        void on_menu_file_popup_create();


        // void reload_objects();

        Gtk::Menu m_Menu_Popup;
        Gtk::Fixed fix;
        int argc;
        char **argv;
        std::map <std::string,SelfObject*> selfobjects;
        Serverproxy &proxy;


};

#endif
