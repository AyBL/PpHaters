#include <iostream>
#include "clientselfobject.h"

SelfObject::SelfObject(std::string name, int argc, char **argv)
: m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label(name), m_Button_Message("Message"),
m_Button_AddSlot("Add Slot"), m_Button_Quit("Quit"),m_TreeView(slots),argc(argc),argv(argv),x(0),y(0),
ismoving(false){

    set_border_width(1);

    add(m_VBox);

    //Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);

    m_ScrolledWindow.set_min_content_width(10);
    m_ScrolledWindow.set_min_content_height(100);

    m_TreeView.set_size_request(10,1);

    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_Button_Move.signal_pressed().connect( sigc::mem_fun(*this,
              &SelfObject::set_moving_state) );
    m_Button_Move.signal_released().connect( sigc::mem_fun(*this,
              &SelfObject::set_idle_state) );

    m_Button_Move.add_pixlabel("edit-move.ico","");

    m_VBox.pack_start(m_HBox);
    m_HBox.pack_start(m_Label);
    m_HBox.pack_start(m_Button_Move);

    m_VBox.pack_start(m_ScrolledWindow, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

    m_ButtonBox.pack_start(m_Button_Message, Gtk::PACK_SHRINK);
    m_Button_Message.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_message) );

    m_ButtonBox.pack_start(m_Button_AddSlot, Gtk::PACK_SHRINK);
    m_Button_AddSlot.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_addslot) );

    m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
    m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_quit) );

    show_all_children();
}

SelfObject::~SelfObject(){
}

void SelfObject::on_button_quit(){
    hide();
}

void SelfObject::on_button_message(){
    auto newapp = Gtk::Application::create(argc, argv,"Message");
    MessageWindow message;
    newapp->run(message);
}

void SelfObject::on_button_move(){
    ismoving = true;
}

void SelfObject::on_button_addslot(){
    auto newapp = Gtk::Application::create(argc, argv,"AddSlot");
    AddSlotWindow addslot;
    newapp->run(addslot);
}

void SelfObject::set_moving_state(){
    Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
    Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
    ismoving = true;
    win->get_pointer(x,y);
    x = x - fix-> child_property_x(*this);
    y = y - fix-> child_property_y(*this);
}

void SelfObject::set_idle_state(){
    ismoving = false;
}


//Decorator
bool SelfObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    if (ismoving){
        Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
        Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
        int xx,yy;

        win->get_pointer(xx,yy);
        fix->move(*this,xx - x,yy - y);
    }
    return Gtk::Frame::on_draw(cr);

}

void SelfObject::AddSlot(std::tuple<std::string, std::string,bool,std::string> newslot){
    std::tuple<std::string,bool,std::string> aux;
    aux = std::make_tuple(std::get<1>(newslot),std::get<2>(newslot),std::get<3>(newslot));
    slots[std::get<0>(newslot)] = aux;

    m_TreeView.load_rows();
}

void SelfObject::RemoveSlot(std::string name){
    slots.erase(name);
    m_TreeView.load_rows();
}
