#include "clientmessagewindow.h"
#include <iostream>

MessageWindow::MessageWindow(std::string &message)
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Button_Close("Close"),
  m_Button_GetIt("Get It"),
  m_Button_DoIt("Do It"),
  m_CheckButton_Visible("Visible"),
  message(message){
      set_size_request(200, 100);
      set_title("Message");

      add(m_VBox);

      m_Entry.set_max_length(50);
      m_Entry.set_text("");
      m_VBox.pack_start(m_Entry);

      m_VBox.pack_start(m_CheckButton_Visible);
      m_CheckButton_Visible.signal_toggled().connect( sigc::mem_fun(*this,
                  &MessageWindow::on_checkbox_visibility_toggled) );
      m_CheckButton_Visible.set_active(true);

      m_VBox.add(m_HBox);

      m_HBox.pack_start(m_Button_GetIt);
      m_HBox.pack_start(m_Button_DoIt);
      m_HBox.pack_start(m_Button_Close);

      m_Button_GetIt.signal_clicked().connect( sigc::mem_fun(*this,
                  &MessageWindow::on_button_getit) );
      m_Button_GetIt.set_can_default();
      m_Button_GetIt.grab_default();

      m_Button_DoIt.signal_clicked().connect( sigc::mem_fun(*this,
                  &MessageWindow::on_button_doit) );
      m_Button_DoIt.set_can_default();
      m_Button_DoIt.grab_default();

      m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this,
                  &MessageWindow::on_button_close) );
      m_Button_Close.set_can_default();
      m_Button_Close.grab_default();

      show_all_children();
}

MessageWindow::~MessageWindow(){
}

void MessageWindow::on_checkbox_visibility_toggled(){
    m_Entry.set_visibility(m_CheckButton_Visible.get_active());
}

void MessageWindow::on_button_getit(){
    message = m_Entry.get_text();
    hide();
}

void MessageWindow::on_button_doit(){
    message = m_Entry.get_text();
    hide();
}

void MessageWindow::on_button_close(){
    hide();
}
