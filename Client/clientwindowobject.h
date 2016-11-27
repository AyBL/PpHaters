#ifndef WINDOWOBJECT_H
#define WINDOWOBJECT_H
#include "clientactions.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/fixed.h>
#include <gtkmm/menu.h>
#include <gtkmm/image.h>
#include "clientselfobject.h"
#include "clientnamewindow.h"
#include "clientvaluewindow.h"
#include "clientdrawingarea.h"
#include <map>
#include <arpa/inet.h>

#define HEIGHT 768
#define WIDTH 1366

class Serverproxy;

class WindowObject : public Gtk::Window{
    public:
        WindowObject(std::string imagen, int argc, char **argv, std::string name,Serverproxy &proxy);
        virtual ~WindowObject();
        void AddAction(Actions *action);
        void AddObject(std::string name,int x, int y);
        void RemoveObject(std::string name);
        void AddValueObject(std::string name,std::string value,int x, int y);
        // void RemoveValueObject(std::string name);
        void AddSlot(std::string name, std::tuple<std::string, std::string,char,std::string> newslot);
        void RemoveSlot(std::string nameobject,std::string nameslot);
        void Move(std::string nameobject,int x, int y);
        void ChangeNameObject(std::string nameobject, std::string newname);
        void ChangeNameSlot(std::string nameobject, std::string nameslot, std::string newname);
        void Invalidate();


    protected:
        //Signal handlers:
        bool on_button_press_event(GdkEventButton* button_event) override;
        void on_menu_file_popup_create();
        void on_menu_file_popup_close();
        
        void on_refresh();

        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

        std::map <std::string,SelfObject*> selfobjects;
        std::map <std::string,ValueObject*> valueobjects;
        std::vector <Actions*> actions;

        Gtk::Menu m_Menu_Popup;
        Gtk::Image m_Image;
        MyDrawingArea area;
        Gtk::Fixed fix;
        int argc;
        char **argv;
        Serverproxy &proxy;
};

#endif
