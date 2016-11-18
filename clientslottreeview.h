#ifndef SLOTTREEVIEW_H
#define SLOTTREEVIEW_H

#include <gtkmm/treeview.h>
#include <gtkmm/menu.h>
#include <gtkmm/liststore.h>
#include "clientmodelcolumns.h"
#include <vector>
#include <map>
#include <tuple>
#include "clientServerproxy.h"

class SlotTreeView : public Gtk::TreeView{
    public:
        SlotTreeView(std::map<std::string,
            std::tuple<std::string,char,std::string> > &slots,
            std::string nameselfobject, Serverproxy &proxy);
        virtual ~SlotTreeView();
        void load_rows();

    protected:
        // Override Signal handler:
        // Alternatively, use signal_button_press_event().connect_notify()
        bool on_button_press_event(GdkEventButton* button_event) override;

        //Signal handler for popup menu items:
        void on_menu_file_popup_delete();
        void on_menu_file_popup_accept();
        void on_menu_file_popup_obtain();

        ModelColumns m_Columns;

        //The Tree model:
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

        Gtk::Menu m_Menu_Popup;

        std::map<std::string,
            std::tuple<std::string,char,std::string> > &slots;
        std::string nameselfobject;
        Serverproxy &proxy;
};

#endif
