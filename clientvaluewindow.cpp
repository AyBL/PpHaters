#include <iostream>
#include "clientvaluewindow.h"

ValueObject::ValueObject(std::string name, int x, int y,Serverproxy &proxy):
m_VBox(Gtk::ORIENTATION_VERTICAL), m_Label(name),m_Button_Quit("Quit"),
name(name),x(x),y(y),proxy(proxy),ismoving(false){
    set_border_width(1);
    set_opacity(1);
    add(m_VBox);

    m_Button_Move.signal_pressed().connect( sigc::mem_fun(*this,
              &ValueObject::set_moving_state) );
    m_Button_Move.signal_released().connect( sigc::mem_fun(*this,
              &ValueObject::set_idle_state) );

    m_Button_Move.add_pixlabel("edit-move.ico","");

    m_VBox.pack_start(m_Button_Move);
    m_VBox.pack_start(m_HBox);
    m_HBox.pack_start(m_Label);
    m_HBox.pack_start(m_Button_Quit);

    m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this,
                &ValueObject::on_button_quit) );

    m_VBox.set_opacity(1);
    m_HBox.set_opacity(1);

    show_all_children();
}

ValueObject::~ValueObject(){
}

void ValueObject::on_button_quit(){
    hide();
}

void ValueObject::on_button_move(){
    ismoving = true;
}

void ValueObject::set_moving_state(){
    Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
    Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
    int xx,yy;

    ismoving = true;

    win->get_pointer(xx,yy);
    dx = xx - x;
    dy = yy - y;
}

void ValueObject::set_idle_state(){
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
    // proxy.SendCommand(sendbuffer);

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &xx, sizeof(int) );
    // proxy.SendPositions(sendbuffer);

    // memset(sendbuffer,0,200);
    memcpy(sendbuffer, &yy, sizeof(int) );
    // proxy.SendPositions(sendbuffer);
}

//Decorator
bool ValueObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    if (ismoving){
        int xx,yy;
        Gtk::Fixed *fix = dynamic_cast<Gtk::Fixed *>(this->get_parent());
        Gtk::Window *win = dynamic_cast<Gtk::Window *>(fix->get_parent());
        win->get_pointer(xx,yy);
        fix->move(*this,xx - dx,yy - dy);
    }
    return Gtk::Frame::on_draw(cr);
}

void ValueObject::AddParent(std::string parent){
    if (parent == "")
        parents.clear();
    else
        parents.push_back(parent);
}

void ValueObject::SetPosition(int posx, int posy){
    x = posx;
    y = posy;
}

void ValueObject::GetPosition(int &posx, int &posy){
    posx = x;
    posy = y;
}

std::vector<std::string> ValueObject::GetParents(){
    return parents;
}
