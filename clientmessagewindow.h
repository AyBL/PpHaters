#ifndef GTKMM_MESSAGEWINDOW_H
#define GTKMM_MESSAGEWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/entry.h>

class MessageWindow : public Gtk::Window{
    public:
        MessageWindow(std::string &message);
        virtual ~MessageWindow();

    protected:
        //Signal handlers:
        void on_checkbox_visibility_toggled();
        void on_button_close();
        void on_button_doit();
        void on_button_getit();

        //Child widgets:
        Gtk::Box m_HBox;
        Gtk::Box m_VBox;
        Gtk::Entry m_Entry;
        Gtk::Button m_Button_Close;
        Gtk::Button m_Button_GetIt;
        Gtk::Button m_Button_DoIt;
        Gtk::CheckButton m_CheckButton_Visible;
        std::string &message;
};

#endif
