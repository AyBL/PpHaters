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
#include <gtkmm/drawingarea.h>
#include <string>


class SelfObject : public Gtk::Frame{
    public:
        SelfObject(std::string name, int posx, int posy, int argc,
            char **argv,Serverproxy &proxy);
        virtual ~SelfObject();
        void AddSlot(std::tuple<std::string, std::string,char,std::string> newslot);
        void RemoveSlot(std::string name);
        void SetPosition(int posx, int posy);
        void GetPosition(int &posx, int &posy);
        void ChangeName(std::string newname);
        void ChangeNameSlot(std::string nameslot,std::string newname);

    protected:
        //Signal handlers:
        void on_button_quit();
        void on_button_message();
        void on_button_addslot();
        void on_button_move();
        void set_moving_state();
        void set_idle_state();
        void on_checkbox_editable_toggled();
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

        std::map<std::string, std::tuple<std::string,char,std::string> > slots;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;
        Gtk::Box m_HBox2;

        Gtk::ScrolledWindow m_ScrolledWindow;

        Gtk::Label m_Label;
        Gtk::Entry m_Entry;
        Gtk::CheckButton m_CheckButton_Editable;
        Gtk::Button m_Button_Move;

        Gtk::Button m_Button_Message;
        Gtk::Button m_Button_AddSlot;
        Gtk::Button m_Button_Quit;

        int x,y;
        SlotTreeView m_TreeView;
        std::string name;

        int argc;
        char **argv;
        Serverproxy &proxy;

        bool ismoving;

        int dx,dy;
};

#endif
