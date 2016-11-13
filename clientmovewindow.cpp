#include "clientmovewindow.h"
#include <iostream>

MoveWindow::MoveWindow(int &x, int &y)
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Label("X: ",true),
  m_Label2("Y: ",true),
  m_Button_Close("Close"),
  m_Button_Move("Move"),
  x(x),y(y)
{
  set_size_request(200, 100);
  set_title("Move Widget");

  add(m_VBox);

  m_VBox.add(m_HBox);
  m_VBox.add(m_HBox2);
  m_VBox.add(m_HBox3);

  m_Entry.set_max_length(20);
  m_Entry.set_text("");

  m_HBox.pack_start(m_Label);
  m_HBox.pack_start(m_Entry);

  m_HBox2.pack_start(m_Label2);
  m_HBox2.pack_start(m_Entry2);

  m_HBox3.pack_start(m_Button_Move);
  m_HBox3.pack_start(m_Button_Close);

  m_Button_Move.signal_clicked().connect( sigc::mem_fun(*this,
              &MoveWindow::on_button_move) );
  m_Button_Move.set_can_default();
  m_Button_Move.grab_default();

  m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
              &MoveWindow::on_button_close) );
  m_Button_Close.set_can_default();
  m_Button_Close.grab_default();

  show_all_children();
}

MoveWindow::~MoveWindow(){
}

void MoveWindow::on_button_move(){
  std::string text(m_Entry.get_text());
  std::string text2(m_Entry2.get_text());
  x = stoi(text);
  y = stoi(text2);
  hide();
}

void MoveWindow::on_button_close(){
  hide();
}
