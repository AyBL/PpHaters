#include "clienterrorwindow.h"
#include <iostream>

ErrorWindow::ErrorWindow(std::string message)
: m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label(message),
  m_Button_Close("Close"){
      set_size_request(200, 100);
      set_title("Error");

      add(m_VBox);

      m_VBox.pack_start(m_Label);

      m_VBox.pack_start(m_Button_Close);
      
      m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
                  &ErrorWindow::on_button_close) );
      m_Button_Close.set_can_default();
      m_Button_Close.grab_default();

      show_all_children();
}

ErrorWindow::~ErrorWindow(){
}

void ErrorWindow::on_button_close(){
    hide();
}
