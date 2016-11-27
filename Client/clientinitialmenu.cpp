#include "clientinitialmenu.h"
#include <iostream>

void InitialMenu::load_rows(){
    unsigned int i;

    Gtk::TreeModel::Row row;

    for (i = 0; i < names.size(); i++){
        row = *(m_refTreeModel->append());
        row[m_Columns.m_col_id] = i;
        row[m_Columns.m_col_name] = names[i];

        if ( i == 0){
            m_Combo.set_active(row);
            nameimagenaux = names[i];
        }

    }
}

InitialMenu::InitialMenu(bool &run, char *ip,char *port, std::string &imagen):
m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label_IP("Ingrese IP: ",true),
m_Label_Port("Ingrese PUERTO: ",true),m_Label_Imagen("Seleccione imagen de fondo: ",true), 
m_Button_Accept("Accept"),m_Button_Quit("Cancel"),run(run),ip(ip),port(port),imagen(imagen){
    set_title("Initial Menu");

    m_Entry_IP.set_max_length(50);
    m_Entry_Port.set_max_length(50);

    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_Combo.set_model(m_refTreeModel);

    //Add the model columns to the Combo (which is a kind of view),
    //rendering them in the default way:
    m_Combo.pack_start(m_Columns.m_col_id);
    m_Combo.pack_start(m_Columns.m_col_name);

    m_VBox.pack_start(m_HBox);
    m_VBox.pack_start(m_Label_Imagen);
    m_VBox.pack_start(m_Combo);
    m_VBox.pack_start(m_HBox2);

    m_HBox.pack_start(m_Label_IP);
    m_HBox.pack_start(m_Entry_IP);
    m_HBox.pack_start(m_Label_Port);
    m_HBox.pack_start(m_Entry_Port);
    m_HBox2.pack_start(m_Button_Accept);
    m_HBox2.pack_start(m_Button_Quit);

    //Add the ComboBox to the window.
    add(m_VBox);

    //Connect signal handler:
    m_Combo.signal_changed().connect( sigc::mem_fun(*this,
        &InitialMenu::on_combo_changed));

    m_Button_Accept.signal_clicked().connect( sigc::mem_fun(*this,
        &InitialMenu::on_button_accept));

    m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
        &InitialMenu::on_button_quit));

    

    names.push_back("Iron-Throne.jpg");
    names.push_back("Winter-is-Coming.jpg");
    names.push_back("Family-Duty-Honor.jpg");
    names.push_back("We-Do-Not-Sow.jpeg");

    this->load_rows();

    show_all_children();
}

InitialMenu::~InitialMenu(){
}

void InitialMenu::on_combo_changed(){
    Gtk::TreeModel::iterator iter = m_Combo.get_active();
    Glib::ustring uname;
    if(iter){
        Gtk::TreeModel::Row row = *iter;
        if(row){
            //Get the data for the selected row, using our knowledge of the tree
            //model:
            uname = row[m_Columns.m_col_name];
            nameimagenaux = uname;
        }
    }
}

void InitialMenu::on_button_accept(){
    std::string ipaux,portaux;

    ipaux = m_Entry_IP.get_text();
    portaux = m_Entry_Port.get_text();

    std::memcpy(ip,ipaux.c_str(),ipaux.size());
    std::memcpy(port,portaux.c_str(),portaux.size());

    imagen = "/usr/bin/" + nameimagenaux;

    run = true;

    hide();
}

void InitialMenu::on_button_quit(){
    
	run = false;

    hide();
}
