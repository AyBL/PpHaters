#include <iostream>
#include "clientselfobject.h"

SelfObject::SelfObject(std::string name, int posx, int posy, int argc,
char **argv,Serverproxy &proxy):m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label(name),
m_Button_Message("Message"),m_Button_AddSlot("Add Slot"),m_Button_Quit("Quit"),
x(posx),y(posy),m_TreeView(slots,name,proxy,x,y),name(name),argc(argc),
argv(argv),proxy(proxy),ismoving(false){

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
    Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
    int posx,posy;
    auto newapp = Gtk::Application::create(argc, argv,"one.message");
    std::string newmessage,buffer;
    char sendbuffer[500];
    MessageWindow message(newmessage);
    newapp->run(message);

    buffer = "M" + std::string(1,name.size()) + name;

    buffer = buffer + std::string(1,newmessage.size()) + newmessage;

    std::memset(sendbuffer, 0, 500);

	memcpy( sendbuffer, buffer.c_str(), buffer.size() );

    // proxy.SendCommand(sendbuffer);

    posx = x + this->get_width()/2;
    posy = y + this->get_height() + 10;

    memcpy(sendbuffer, &x, sizeof(int));
    // proxy.SendPositions(sendbuffer);

    memcpy(sendbuffer, &y, sizeof(int));
    // proxy.SendPositions(sendbuffer);
}

void SelfObject::on_button_move(){
    ismoving = true;
}

void SelfObject::on_button_addslot(){
    auto newapp = Gtk::Application::create(argc, argv,"add.slot");
    std::string newslot,buffer;
    char sendbuffer[500];
    AddSlotWindow addslot(newslot);
    newapp->run(addslot);
    if (!newslot.empty()){
        newslot = name + " _addSlot: (| " + newslot +". |).";

        buffer = "A"+std::string(1,newslot.size())+newslot;

        std::memset(sendbuffer, 0, 500);

        memcpy( sendbuffer, buffer.c_str(), buffer.size() );

        proxy.SendCommand(sendbuffer);
    }
}

void SelfObject::set_moving_state(){
    Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
    Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
    int xx,yy;

    ismoving = true;

    win->get_pointer(xx,yy);
    dx = xx - x;
    dy = yy - y;
}

void SelfObject::set_idle_state(){
    Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
    Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
    char sendbuffer[200];
    std::string buffer;
    int xx,yy;

    ismoving = false;

    win->get_pointer(xx,yy);

    xx = xx - dx;
    yy = yy - dy;

    buffer = "P"+std::string(1,name.size())+name;

    std::memset(sendbuffer, 0, 200);
    memcpy( sendbuffer, buffer.c_str(), buffer.size() );
    proxy.SendCommand(sendbuffer);

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &xx, sizeof(int) );
    proxy.SendPositions(sendbuffer);

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &yy, sizeof(int) );
    proxy.SendPositions(sendbuffer);
}

//Decorator
bool SelfObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    if (ismoving){
        int xx,yy;
        Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
        Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
        win->get_pointer(xx,yy);
        fix->move(*this,xx - dx,yy - dy);
    }
    return Gtk::Frame::on_draw(cr);
}

void SelfObject::AddSlot(std::tuple<std::string, std::string,char,std::string> newslot){
    std::tuple<std::string,char,std::string> aux;
    aux = std::make_tuple(std::get<1>(newslot),std::get<2>(newslot),std::get<3>(newslot));
    slots[std::get<0>(newslot)] = aux;

    m_TreeView.load_rows();
}

void SelfObject::RemoveSlot(std::string name){
    slots.erase(name);
    m_TreeView.load_rows();
}

void SelfObject::SetPosition(int posx, int posy){
    x = posx;
    y = posy;
}

void SelfObject::GetPosition(int &posx, int &posy){
    posx = x;
    posy = y;
}
