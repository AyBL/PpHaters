#include "clientaddslotwindow.h"
#include <iostream>

AddSlotWindow::AddSlotWindow(std::string &slot)
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Label("New Slot: ",true),
  m_Button_Close("Close"),
  m_Button_Create("Create Slot"),
  slot(slot)
{
  set_size_request(200, 100);
  set_title("AddSlot");

  add(m_VBox);

  m_VBox.pack_start(m_Label);
  m_VBox.pack_start(m_Entry);
  m_VBox.add(m_HBox);

  m_Entry.set_max_length(50);
  m_Entry.set_text("");

  m_HBox.pack_start(m_Button_Create);
  m_HBox.pack_start(m_Button_Close);

  m_Button_Create.signal_clicked().connect( sigc::mem_fun(*this,
              &AddSlotWindow::on_button_create) );
  m_Button_Create.set_can_default();
  m_Button_Create.grab_default();

  m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
              &AddSlotWindow::on_button_close) );
  m_Button_Close.set_can_default();
  m_Button_Close.grab_default();

  show_all_children();
}

AddSlotWindow::~AddSlotWindow(){
}

void AddSlotWindow::on_button_create(){
  slot = m_Entry.get_text();
  hide();
}

void AddSlotWindow::on_button_close(){
  hide();
}
