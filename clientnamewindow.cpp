#include "clientnamewindow.h"
#include <iostream>

NameWindow::NameWindow(std::string &name)
: m_Label("Name: ",true),
  m_Button_Accept("Accept"),
  name(name)
{
  set_size_request(200, 30);
  set_title("New SelfObject");

  add(m_HBox);

  m_HBox.pack_start(m_Label);
  m_HBox.pack_start(m_Entry);
  m_HBox.pack_start(m_Button_Accept);

  m_Entry.set_max_length(50);
  m_Entry.set_text("");

  m_Button_Accept.signal_clicked().connect( sigc::mem_fun(*this,
              &NameWindow::on_button_accept) );
  m_Button_Accept.set_can_default();
  m_Button_Accept.grab_default();

  show_all_children();
}

NameWindow::~NameWindow(){
}

void NameWindow::on_button_accept(){
  name = m_Entry.get_text();
  hide();
}
