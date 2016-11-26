#include "clientmenuwindow.h"
#include "clientServerproxy.h"
#include <iostream>

void MenuWindow::load_rows(){
    unsigned int i;

    Gtk::TreeModel::Row row;

    for (i = 0; i < lobbies.size(); i++){
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = std::get<0>(lobbies[i]);
        row[m_Columns.m_col_name] = std::get<1>(lobbies[i]);
    }

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_id] = 255;
    row[m_Columns.m_col_name] = "Nuevo";

    m_Combo.set_active(row);
    id = 255;
    name = "Nuevo";

}

MenuWindow::MenuWindow(std::string &name,Serverproxy &proxy):
m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label("Seleccione Lobby: ",true),
m_Label2("Ingrese nombre de nuevo Lobby: ",true),
m_Button("Accept"),name(name),proxy(proxy){
    set_title("Menu Lobbies");

    m_Entry.set_max_length(50);
    m_Entry.set_text("Nuevo");

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    this->load_rows();

    //Add the model columns to the Combo (which is a kind of view),
    //rendering them in the default way:
    m_Combo.pack_start(m_Columns.m_col_id);
    m_Combo.pack_start(m_Columns.m_col_name);

    m_VBox.pack_start(m_Label);
    m_VBox.pack_start(m_Combo);
    m_VBox.pack_start(m_Label2);
    m_VBox.pack_start(m_Entry);
    m_VBox.pack_start(m_Button);

    //Add the ComboBox to the window.
    add(m_VBox);

    //Connect signal handler:
    m_Combo.signal_changed().connect( sigc::mem_fun(*this,
        &MenuWindow::on_combo_changed));

    m_Button.signal_clicked().connect( sigc::mem_fun(*this,
        &MenuWindow::on_button_accept));

    show_all_children();
}

MenuWindow::~MenuWindow(){
}

void MenuWindow::on_combo_changed(){
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    Glib::ustring uname;
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            //Get the data for the selected row, using our knowledge of the tree
            //model:
            id = row[m_Columns.m_col_id];
            uname = row[m_Columns.m_col_name];
            name = uname;
        }
    }
}

void MenuWindow::on_button_accept(){
    std::string buffer("L");
    char sendbuffer[200];
    size_t bytestosend;

    buffer = buffer + std::string(1,id);

    if (id == 255){
        Glib::ustring uname;
        uname = m_Entry.get_text();
        name = uname;
        
        buffer = buffer + std::string(1,name.size()) + name;
    } 

    std::memset(sendbuffer, 0, 200);

    memcpy( sendbuffer, buffer.c_str(), buffer.size() );

    bytestosend = strlen(sendbuffer);
    if (id == 0)
        bytestosend += 1;

    proxy.Send(sendbuffer,bytestosend);
    hide();
}

void MenuWindow::AddLobbies(std::tuple<unsigned char,std::string> newlobbies){
    lobbies.push_back(newlobbies);
    m_refTreeModel->clear();
    this->load_rows();
}
