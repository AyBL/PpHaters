#include <iostream>
#include "clientselfobject.h"

SelfObject::SelfObject(std::string name, int posx, int posy, int argc,
char **argv,Serverproxy &proxy):m_VBox(Gtk::ORIENTATION_VERTICAL),m_Label("Name: "),
m_CheckButton_Editable("Edit"),
m_Button_Message("Message"),m_Button_AddSlot("Add Slot"),m_Button_Quit("Quit"),
x(posx),y(posy),m_TreeView(slots,name,proxy,x,y),name(name),argc(argc),
argv(argv),proxy(proxy),ismoving(false){

    // set_size_request (259, 164);

    set_border_width(1);

    add(m_VBox);

    //Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);

    m_ScrolledWindow.set_min_content_width(10);
    m_ScrolledWindow.set_min_content_height(100);

    m_TreeView.set_size_request(10,1);

    m_Entry.set_text(name);
    m_Entry.set_editable(false);

    m_CheckButton_Editable.signal_toggled().connect( sigc::mem_fun(*this,
              &SelfObject::on_checkbox_editable_toggled) );

    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

    m_Button_Move.signal_pressed().connect( sigc::mem_fun(*this,
              &SelfObject::set_moving_state) );
    m_Button_Move.signal_released().connect( sigc::mem_fun(*this,
              &SelfObject::set_idle_state) );

    m_Button_Move.add_pixlabel("edit-move.ico","Move");

    m_VBox.pack_start(m_Button_Move);
    m_VBox.pack_start(m_HBox);
    m_HBox.pack_start(m_Label);
    m_HBox.pack_start(m_Entry);
    m_HBox.pack_start(m_CheckButton_Editable);

    m_VBox.pack_start(m_ScrolledWindow, Gtk::PACK_SHRINK);
    m_VBox.pack_start(m_HBox2, Gtk::PACK_SHRINK);

    m_HBox2.pack_start(m_Button_Message);
    m_Button_Message.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_message) );

    m_HBox2.pack_start(m_Button_AddSlot);
    m_Button_AddSlot.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_addslot) );

    m_HBox2.pack_start(m_Button_Quit);
    m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
                &SelfObject::on_button_quit) );

    show_all_children();
}

SelfObject::~SelfObject(){
}

void SelfObject::on_button_quit(){
    char sendbuffer[500];
    std::string buffer;
    buffer = "E" + std::string(1,name.size()) + name;

    std::memset(sendbuffer, 0, 500);

	memcpy( sendbuffer, buffer.c_str(), buffer.size() );

    // proxy.Send(sendbuffer,strlen(sendbuffer));
}

void SelfObject::on_button_message(){
    int posx,posy;
    auto newapp = Gtk::Application::create(argc, argv,"one.message");
    std::string newmessage,buffer;
    char sendbuffer[500];
    char type = '0';
    MessageWindow message(newmessage);
    newapp->run(message);

    buffer = "M" + std::string(1,type) + std::string(1,name.size()) + name;

    buffer = buffer + std::string(1,newmessage.size()) + newmessage;

    std::memset(sendbuffer, 0, 500);

	memcpy( sendbuffer, buffer.c_str(), buffer.size() );

    proxy.Send(sendbuffer,strlen(sendbuffer));

    // std::cout<<"Tamanio: "<< this->get_width() << " " << this->get_height()<<std::endl;

    posx = x + this->get_width()/2;
    posy = y + this->get_height() + 50;

    memcpy(sendbuffer, &posx, sizeof(int));
    proxy.Send(sendbuffer,sizeof(int));

    memcpy(sendbuffer, &posy, sizeof(int));
    proxy.Send(sendbuffer,sizeof(int));
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

        buffer = "A"+std::string(1,name.size())+name;
        buffer = buffer +std::string(1,newslot.size())+newslot;

        std::memset(sendbuffer, 0, 500);

        memcpy( sendbuffer, buffer.c_str(), buffer.size() );

        proxy.Send(sendbuffer,strlen(sendbuffer));
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
    proxy.Send(sendbuffer,strlen(sendbuffer));

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &xx, sizeof(int) );
    proxy.Send(sendbuffer,sizeof(int));

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &yy, sizeof(int) );
    proxy.Send(sendbuffer,sizeof(int));
}

void SelfObject::on_checkbox_editable_toggled(){
  bool edit = m_CheckButton_Editable.get_active();

  m_Entry.set_editable(edit);

  if ((!edit) && (m_Entry.get_text() == name)){

  }
}


//Decorator
bool SelfObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    if (ismoving){
        int xx,yy;
        Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
        Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
        win->get_pointer(xx,yy);
        x = xx - dx;
        y = yy - dy;
        fix->move(*this,x,y);
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

void SelfObject::ChangeName(std::string newname){
    name = newname;
    m_Label.set_label(newname);
}

void SelfObject::ChangeNameSlot(std::string nameslot,std::string newname){
    slots[newname] = slots[nameslot];
    slots.erase(nameslot);
    m_TreeView.load_rows();
}
