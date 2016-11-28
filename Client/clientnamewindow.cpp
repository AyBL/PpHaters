#include "clientnamewindow.h"
#include <iostream>

NameWindow::NameWindow(std::string &name, bool &create)
: m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label("Name: ",true),
m_Button_Accept("Accept"),m_Button_Cancel("Cancel"),
name(name),create(create){
    set_size_request(200, 30);
    set_title("New SelfObject");

    add(m_VBox);
    m_VBox.pack_start(m_HBox);
    m_VBox.pack_start(m_HBox2);
    m_HBox.pack_start(m_Label);
    m_HBox.pack_start(m_Entry);
    m_HBox2.pack_start(m_Button_Accept);
    m_HBox2.pack_start(m_Button_Cancel);

    m_Entry.set_max_length(50);
    m_Entry.set_text("");

    m_Button_Accept.signal_clicked().connect( sigc::mem_fun(*this,
              &NameWindow::on_button_accept) );

    m_Button_Cancel.signal_clicked().connect( sigc::mem_fun(*this,
              &NameWindow::on_button_cancel) );

    show_all_children();
}

NameWindow::~NameWindow(){
}

void NameWindow::on_button_accept(){
    name = m_Entry.get_text();
    if (!name.empty()){
        create = true;
        hide();
    }
}

void NameWindow::on_button_cancel(){
    create = false;
    hide();
}