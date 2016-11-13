#ifndef SELFOBJECT_H
#define SELFOBJECT_H
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/menu.h>
#include "clientslottreeview.h"
#include "clientmessagewindow.h"
#include "clientaddslotwindow.h"
#include "clientmovewindow.h"
#include <gtkmm/fixed.h>

class SelfObject : public Gtk::Frame{
    public:
        SelfObject(std::string name, int argc, char **argv);
        virtual ~SelfObject();
        void AddSlot(std::tuple<std::string, std::string,bool,std::string> newslot);
        void RemoveSlot(std::string name);

    protected:
        //Signal handlers:
        void on_button_quit();
        void on_button_message();
        void on_button_addslot();
        void on_button_move();
        void set_moving_state();
        void set_idle_state();
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

        std::map<std::string, std::tuple<std::string,bool,std::string> > slots;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;

        Gtk::ScrolledWindow m_ScrolledWindow;
        SlotTreeView m_TreeView;

        Gtk::Label m_Label;
        Gtk::Button m_Button_Move;

        Gtk::ButtonBox m_ButtonBox;
        Gtk::Button m_Button_Message;
        Gtk::Button m_Button_AddSlot;
        Gtk::Button m_Button_Quit;

        int argc;
        char **argv;

        int x;
        int y;

        bool ismoving;

};

#endif
