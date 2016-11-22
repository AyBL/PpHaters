#ifndef VALUE_H
#define VALUE_H
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/fixed.h>
#include <gtkmm/window.h>
#include "clientServerproxy.h"
#include <vector>
#include <string>
#include <cstring>

class ValueObject : public Gtk::Frame{
    public:
        ValueObject(std::string name,int x, int y, Serverproxy &proxy);
        virtual ~ValueObject();
        void AddParent(std::string parent);
        void SetPosition(int posx, int posy);
        void GetPosition(int &posx, int &posy);
        std::vector<std::string> GetParents();

    protected:
        //Signal handlers:
        void on_button_quit();
        void on_button_move();
        void set_moving_state();
        void set_idle_state();
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

        //Child widgets:
        Gtk::Box m_VBox;
        Gtk::Box m_HBox;

        Gtk::Label m_Label;

        Gtk::Button m_Button_Move;
        Gtk::Button m_Button_Quit;

        std::string name;

        int x,dx;
        int y,dy;

        Serverproxy &proxy;

        bool ismoving;

        std::vector<std::string> parents;
};

#endif
