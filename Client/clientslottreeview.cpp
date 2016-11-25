#include "clientslottreeview.h"
#include <iostream>

void SlotTreeView::load_rows(){
    //Fill the TreeView's model

    std::map<std::string, std::tuple<std::string,char,std::string> >::iterator it;

    Gtk::TreeModel::Row row;

    m_refTreeModel->clear();
    nvslots.clear();

    for (it = slots.begin(); it != slots.end(); ++it ){
        std::tuple<std::string,char,std::string>& aux = it->second;
        char auxmut = std::get<1>(aux);
        nvslots.push_back(std::make_tuple(it->first,std::get<0> (aux)));
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = nvslots.size() - 1;
        row[m_Columns.m_col_name] = it->first ;
        row[m_Columns.m_col_value] = std::get<0> (aux);
        if ((auxmut == 'I')||(auxmut == 'P'))
            row[m_Columns.m_col_mutable] = false;
        else
            row[m_Columns.m_col_mutable] = true;
        row[m_Columns.m_col_parent] = std::get<2>(aux);
    }
}

SlotTreeView::SlotTreeView(std::map<std::string,
    std::tuple<std::string,char,std::string> > &slots,
    std::string nameselfobject,Serverproxy &proxy, int &x, int &y):
    slots(slots),nameselfobject(nameselfobject),proxy(proxy),x(x),y(y){

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    set_model(m_refTreeModel);

    //Add the TreeView's view columns:
    append_column("ID", m_Columns.m_col_id);
    append_column_editable("Name", m_Columns.m_col_name);
    append_column_editable("Value", m_Columns.m_col_value);
    append_column("Mutable", m_Columns.m_col_mutable);
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
    bool return_value;

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
    char sendbuffer[200];
    std::memset(sendbuffer, 0, 200);
    if (refSelection){
        Gtk::TreeModel::iterator iter = refSelection->get_selected();
        if (iter){
            Glib::ustring name = (*iter)[m_Columns.m_col_name];
            buffer = "D"+std::string(1,nameselfobject.size());
            buffer = buffer + nameselfobject + std::string(1,name.size());
            buffer = buffer + name;
            memcpy(sendbuffer, buffer.c_str(), buffer.size() );
            // proxy.Send(sendbuffer,strlen(sendbufer));
        }
    }
}

void SlotTreeView::on_menu_file_popup_accept(){
    auto refSelection = get_selection();
    int id;
    Glib::ustring name;
    Glib::ustring value;
    bool mut;

    std::tuple<std::string,std::string> nvslotsaux;
    std::string nameant,valueant;
    char sendbuffer[200];
    std::string namebuffer,valuebuffer;

    std::memset(sendbuffer, 0, 200);

    if (refSelection){
        Gtk::TreeModel::iterator iter = refSelection->get_selected();
        if (iter){
            id = (*iter)[m_Columns.m_col_id];
            name = (*iter)[m_Columns.m_col_name];
            value = (*iter)[m_Columns.m_col_value];
            mut = (*iter)[m_Columns.m_col_mutable];

            nvslotsaux = nvslots[id];
            nameant = std::get<0>(nvslotsaux);
            valueant = std::get<1>(nvslotsaux);

            if (name != nameant){
                namebuffer = "R"+ std::string(1,nameselfobject.size());
                namebuffer = namebuffer + nameselfobject;
                namebuffer = namebuffer + std::string(1,nameant.size());
                namebuffer = namebuffer + nameant;
                namebuffer = namebuffer + std::string(1,name.size());
                namebuffer = namebuffer + name;
                memcpy(sendbuffer, namebuffer.c_str(), namebuffer.size() );
                // proxy.Send(sendbuffer,strlen(sendbuffer));
                std::memset(sendbuffer, 0, 200);
            }

            if (mut && (value != valueant)){
                valuebuffer = name + ": " + value + ".";
                valuebuffer = std::string(1,valuebuffer.size()) + valuebuffer;
                valuebuffer = ""+std::string(1,nameselfobject.size())+nameselfobject+valuebuffer;

                memcpy(sendbuffer, valuebuffer.c_str(), valuebuffer.size() );
                // proxy.Send(sendbuffer,strlen(sendbuffer));
            }
        }
    }
}

void SlotTreeView::on_menu_file_popup_obtain(){
    auto refSelection = get_selection();
    std::string buffer;
    int posx, posy;
    char sendbuffer[200];
    char type = '1';
    std::memset(sendbuffer, 0, 200);
    if (refSelection){
        Gtk::TreeModel::iterator iter = refSelection->get_selected();
        if (iter){
            Glib::ustring name = (*iter)[m_Columns.m_col_name];
            buffer = "M" + std::string(1,type) +std::string(1,nameselfobject.size());
            buffer = buffer + nameselfobject + std::string(1,name.size());
            buffer = buffer + name;
            memcpy(sendbuffer, buffer.c_str(), buffer.size() );
            proxy.Send(sendbuffer,strlen(sendbuffer));
            posx = x - 100;
            posy = y + 100;

            memcpy(sendbuffer, &posx, sizeof(int));
            proxy.Send(sendbuffer,sizeof(int));

            memcpy(sendbuffer, &posy, sizeof(int));
            proxy.Send(sendbuffer,sizeof(int));
        }
    }
}
