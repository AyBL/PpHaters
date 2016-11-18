#include <iostream>
#include "clientwindowobject.h"
#include "clientServerproxy.h"
#include <gtkmm/window.h>

WindowObject::WindowObject(int argc, char **argv,std::string name,Serverproxy &proxy):
argc(argc),argv(argv),proxy(proxy) {

    auto item = Gtk::manage(new Gtk::MenuItem("_Create Object", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_create) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Refresh App", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_refresh) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Close App", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_close) );
    m_Menu_Popup.append(*item);

    set_title("Morphic: " + name);
    set_border_width(1);
    set_default_size(WIDTH, HEIGHT);

    fix.set_border_width(5);

    add(fix);

    m_Menu_Popup.accelerate(*this);
    m_Menu_Popup.show_all(); //Show all menu items when the menu pops up

    show_all_children();
}

WindowObject::~WindowObject(){
}

bool WindowObject::on_button_press_event(GdkEventButton* button_event){
    bool return_value = false;

    return_value = Window::on_button_press_event(button_event);

    if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3)){
        m_Menu_Popup.popup(button_event->button, button_event->time);
    }

    return return_value;
}

void WindowObject::on_menu_file_popup_create(){
    int x,y;

    std::string name,buffer;
    char sendbuffer[200];
    auto newapp = Gtk::Application::create(argc, argv,"name.objeto");
    NameWindow namewindow(name);
    get_pointer(x,y);

    newapp->run(namewindow);

    buffer = "O"+std::string(1,name.size())+name;

    memset(sendbuffer,0,200);
	memcpy(sendbuffer, buffer.c_str(), buffer.size() );


    proxy.SendCommand(sendbuffer);

    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &x, sizeof(int));
    proxy.SendPositions(sendbuffer);
    memset(sendbuffer,0,200);
    memcpy(sendbuffer, &y, sizeof(int) );
    proxy.SendPositions(sendbuffer);

    // AddObject(name,x,y);

}

void WindowObject::on_menu_file_popup_refresh(){
    unsigned int i,size;
    size = actions.size();
    for (i = 0; i < size; i++)
        actions[i]->run();

    actions.clear();
}

void WindowObject::on_menu_file_popup_close(){

    std::map <std::string,SelfObject*>::iterator it;
    std::string name;
    for (it = selfobjects.begin(); it != selfobjects.end();++it){
        name = it->first;
        selfobjects[name]->hide();
        delete(selfobjects[name]);
        selfobjects.erase(name);
    }
    hide();
}

void WindowObject::AddObject(std::string name,int x, int y){
    selfobjects[name] = new SelfObject(name,argc,argv,proxy);
    fix.put(*selfobjects[name],x,y);
    selfobjects[name]->show();

}

void WindowObject::AddAction(Actions *action){
    actions.push_back(action);
}


void WindowObject::RemoveObject(std::string name){
    selfobjects[name]->hide();
    delete(selfobjects[name]);
    selfobjects.erase(name);
}

void WindowObject::AddValueObject(std::string name,std::string value,int x, int y){
    if (valueobjects.count(value) > 0){
        valueobjects[value]->AddParent(name);
    } else {
        valueobjects[value] = new ValueObject(value);
        valueobjects[value]->AddParent(name);
        fix.put(*valueobjects[value],x,y);
        valueobjects[value]->show();
    }
}

void WindowObject::RemoveValueObject(std::string name){
    valueobjects[name]->hide();
    delete(valueobjects[name]);
    valueobjects.erase(name);
}


void WindowObject::AddSlot(std::string name, std::tuple<std::string, std::string,char,std::string> newslot){
    selfobjects[name]->AddSlot(newslot);
}

void WindowObject::RemoveSlot(std::string nameobject,std::string nameslot){
    selfobjects[nameobject]->RemoveSlot(nameslot);
}

void WindowObject::Move(std::string nameobject,int x, int y){
    fix.move(*selfobjects[nameobject],x,y);
}



// bool WindowObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
//
//     if (!cola.empty()){
//         AddObject("name",0,0);
//
//     }
//
//
//     return Gtk::Window::on_draw(cr);
// }
