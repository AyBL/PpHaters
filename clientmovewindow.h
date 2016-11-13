#ifndef MOVEWINDOW_H
#define MOVEWINDOW_H

#include <gtkmm.h>

class MoveWindow : public Gtk::Window{
    public:
        MoveWindow(int &x, int &y);
        virtual ~MoveWindow();

    protected:
        //Signal handlers:
        void on_button_close();
        void on_button_move();

        //Child widgets:
        Gtk::Box m_HBox;
        Gtk::Box m_HBox2;
        Gtk::Box m_HBox3;
        Gtk::Box m_VBox;
        Gtk::Entry m_Entry;
        Gtk::Entry m_Entry2;
        Gtk::Label m_Label;
        Gtk::Label m_Label2;
        Gtk::Button m_Button_Close;
        Gtk::Button m_Button_Move;

        int &x;
        int &y;
};

#endif
