#include "clientmoveobject.h"
#include <iostream>

void MoveObject::load_rows(){
    unsigned int i;

    Gtk::TreeModel::Row row;
    Gtk::TreeModel::Row row2;

    for (i = 0; i < lobbies.size(); i++){
        row = *(m_refTreeModelLobbies->append());
        row[m_Columns_Lobby.m_col_id] = std::get<0>(lobbies[i]);
        row[m_Columns_Lobby.m_col_name] = std::get<1>(lobbies[i]);
        if ( i == 0) {
            m_Combo_Lobby.set_active(row);
            idlobby = std::get<0>(lobbies[i]);    
        }
    }

    for (i = 0; i < objects.size(); i++){
        row2 = *(m_refTreeModelObjects->append());
        row2[m_Columns_Object.m_col_id] = std::get<0>(objects[i]);
        row2[m_Columns_Object.m_col_name] = std::get<1>(objects[i]);
        if ( i == 0) {
            m_Combo_Object.set_active(row2);
            nameobject = std::get<1>(objects[i]);    
        }
    }

}

MoveObject::MoveObject(unsigned char &id, std::string &name):
m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label("Seleccione Lobby: ",true),
m_Label2("Seleccione SelfObject: ",true),
m_Button("Accept"),id(id), name(name){
    set_title("Move SelfObjects");

    //Create the Tree model:
    m_refTreeModelLobbies = Gtk::ListStore::create(m_Columns_Lobby);
    m_Combo_Lobby.set_model(m_refTreeModelLobbies);

    m_refTreeModelObjects = Gtk::ListStore::create(m_Columns_Object);
    m_Combo_Object.set_model(m_refTreeModelObjects);

    this->load_rows();

    //Add the model columns to the Combo (which is a kind of view),
    //rendering them in the default way:
    m_Combo_Lobby.pack_start(m_Columns_Lobby.m_col_id);
    m_Combo_Lobby.pack_start(m_Columns_Lobby.m_col_name);

    m_Combo_Object.pack_start(m_Columns_Object.m_col_id);
    m_Combo_Object.pack_start(m_Columns_Object.m_col_name);


    m_VBox.pack_start(m_Label);
    m_VBox.pack_start(m_Combo_Lobby);
    m_VBox.pack_start(m_Label2);
    m_VBox.pack_start(m_Combo_Object);
    m_VBox.pack_start(m_Button);

    //Add the ComboBox to the window.
    add(m_VBox);

    //Connect signal handler:
    m_Combo_Lobby.signal_changed().connect( sigc::mem_fun(*this,
        &MoveObject::on_combo_changed_lobby));

    m_Combo_Object.signal_changed().connect( sigc::mem_fun(*this,
        &MoveObject::on_combo_changed_object));

    m_Button.signal_clicked().connect( sigc::mem_fun(*this,
        &MoveObject::on_button_accept));

    show_all_children();
}

MoveObject::~MoveObject(){
}

void MoveObject::on_combo_changed_lobby(){
    Gtk::TreeModel::iterator iter = m_Combo_Lobby.get_active();
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            idlobby = row[m_Columns_Lobby.m_col_id];
        }
    }
}

void MoveObject::on_combo_changed_object(){
    Gtk::TreeModel::iterator iter = m_Combo_Object.get_active();
    Glib::ustring uname;
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            uname = row[m_Columns_Lobby.m_col_name];
            nameobject = uname;
        }
    }
}

void MoveObject::on_button_accept(){
    id = idlobby;
    name = nameobject;
    hide();
}

void MoveObject::AddLobbies(std::tuple<unsigned char,std::string> newlobbies){
    lobbies.push_back(newlobbies);
    m_refTreeModelLobbies->clear();
    this->load_rows();
}

void MoveObject::AddObjects(std::tuple<unsigned char,std::string> newobjects){
    objects.push_back(newobjects);
    m_refTreeModelObjects->clear();
    this->load_rows();
}
