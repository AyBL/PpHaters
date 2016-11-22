#ifndef SELFOBJECT_H
#define SELFOBJECT_H
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/menu.h>
#include <gtkmm/scrolledwindow.h>
#include "clientslottreeview.h"
#include "clientmessagewindow.h"
#include "clientaddslotwindow.h"
#include <gtkmm/fixed.h>
#include "clientServerproxy.h"

class SelfObject : public Gtk::Frame{
    public:
        SelfObject(std::string name, int posx, int posy, int argc,
            char **argv,Serverproxy &proxy);
        virtual ~SelfObject();
        void AddSlot(std::tuple<std::string, std::string,char,std::string> newslot);
        void RemoveSlot(std::string name);
        void SetPosition(int posx, int posy);
        void GetPosition(int &posx, int &posy);

    protected:
        //Signal handlers:
        void on_button_quit();
        void on_button_message();
        void on_button_addslot();
        void on_button_move();
        void set_moving_state();
        void set_idle_state();
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

        std::map<std::string, std::tuple<std::string,char,std::string> > slots;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;

        Gtk::ScrolledWindow m_ScrolledWindow;

        Gtk::Label m_Label;
        Gtk::Button m_Button_Move;

        Gtk::ButtonBox m_ButtonBox;
        Gtk::Button m_Button_Message;
        Gtk::Button m_Button_AddSlot;
        Gtk::Button m_Button_Quit;
        SlotTreeView m_TreeView;

        std::string name;
        int x,dx;
        int y,dy;

        int argc;
        char **argv;
        Serverproxy &proxy;


        bool ismoving;
};

#endif
