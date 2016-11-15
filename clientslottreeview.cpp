#include "clientslottreeview.h"
#include <iostream>

void SlotTreeView::load_rows(){
    //Fill the TreeView's model

    std::map<std::string, std::tuple<std::string,std::string,char,std::string> >::iterator it;

    Gtk::TreeModel::Row row;
    char auxmut;

    for (it = slots.begin(); it != slots.end(); ++it ){
        std::tuple<std::string,std::string,char,std::string>& aux = it->second;
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = 0;
        row[m_Columns.m_col_name] = it->first ;
        row[m_Columns.m_col_value] = std::get<0> (aux);
        auxmut = std::get<2>(aux);
        if ((auxmut == 'i')||(auxmut == 'p'))
            row[m_Columns.m_col_mutable] = false;
        else
            row[m_Columns.m_col_mutable] = true;
        row[m_Columns.m_col_type] = std::get<1>(aux);
        row[m_Columns.m_col_parent] = std::get<3>(aux);

    }

}

SlotTreeView::SlotTreeView(std::map<std::string,
    std::tuple<std::string,std::string,char,std::string> > &slots,
    std::string nameselfobject,Serverproxy &proxy):slots(slots),
    nameselfobject(nameselfobject),proxy(proxy){

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    set_model(m_refTreeModel);

    //Add the TreeView's view columns:
    append_column("ID", m_Columns.m_col_id);
    append_column_editable("Name", m_Columns.m_col_name);
    append_column_editable("Value", m_Columns.m_col_value);
    append_column("Mutable", m_Columns.m_col_mutable);
    append_column("Type", m_Columns.m_col_type);
    append_column("Parent", m_Columns.m_col_parent);

    load_rows();

    //Fill popup menu:
    auto item = Gtk::manage(new Gtk::MenuItem("_Accept Changes", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &SlotTreeView::on_menu_file_popup_accept) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Remove", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &SlotTreeView::on_menu_file_popup_delete) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Obtain", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &SlotTreeView::on_menu_file_popup_obtain) );
    m_Menu_Popup.append(*item);

    m_Menu_Popup.accelerate(*this);
    m_Menu_Popup.show_all(); //Show all menu items when the menu pops up
}

SlotTreeView::~SlotTreeView(){
}

bool SlotTreeView::on_button_press_event(GdkEventButton* button_event){
    bool return_value = false;

    //Call base class, to allow normal handling,
    //such as allowing the row to be selected by the right-click:
    return_value = TreeView::on_button_press_event(button_event);

    //Then do our custom stuff:
    if ((button_event->type == GDK_BUTTON_PRESS)&&(button_event->button == 3)){
        m_Menu_Popup.popup(button_event->button, button_event->time);
    }

    return return_value;
}

void SlotTreeView::on_menu_file_popup_delete(){
    auto refSelection = get_selection();
    std::string buffer;
    char sendbuffer[1000];
    if (refSelection){
        Gtk::TreeModel::iterator iter = refSelection->get_selected();
        if (iter){
            Glib::ustring name = (*iter)[m_Columns.m_col_name];
            buffer = "D"+std::string(1,nameselfobject.size());
            buffer = buffer + nameselfobject + std::string(1,name.size());
            buffer = buffer + name;
            memcpy(sendbuffer, buffer.c_str(), buffer.size() );
            proxy.SendCommand(sendbuffer);
        }
    }
}

void SlotTreeView::on_menu_file_popup_accept(){
    // auto refSelection = get_selection();
    // int id;
    // Glib::ustring name;
    // Glib::ustring value;
    // bool mut;
    //
    // if (refSelection){
    //     Gtk::TreeModel::iterator iter = refSelection->get_selected();
    //     if (iter){
    //         id = (*iter)[m_Columns.m_col_id];
    //         name = (*iter)[m_Columns.m_col_name];
    //         value = (*iter)[m_Columns.m_col_value];
    //         mut = (*iter)[m_Columns.m_col_mutable];
    //         std::get<0>(slots[id]) = name;
    //         if (mut)
    //             std::get<1>(slots[id]) = value;
    //
    //         m_refTreeModel->clear();
    //         load_rows();
    //
    //     }
    // }
}

void SlotTreeView::on_menu_file_popup_obtain(){
}
