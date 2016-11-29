#include <iostream>
#include "clientwindowobject.h"
#include "clientServerproxy.h"
#include <gtkmm/window.h>

WindowObject::WindowObject(std::string imagen, int argc, char **argv,std::string name,Serverproxy &proxy):
m_Image(imagen),area(selfobjects,valueobjects),argc(argc),argv(argv),proxy(proxy) {

    auto item = Gtk::manage(new Gtk::MenuItem("_Create Object", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_create) );
    m_Menu_Popup.append(*item);

    // item = Gtk::manage(new Gtk::MenuItem("_Move Object", true));
    // item->signal_activate().connect(
    //     sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_move) );
    // m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem("_Close App", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_close) );
    m_Menu_Popup.append(*item);

    set_title("Morphic: " + name);
    set_border_width(1);
    set_default_size(WIDTH, HEIGHT);

    add(fix);

    fix.put(m_Image,0,0);

    m_Image.show();

    fix.put(area,0,0);

    area.show();

    m_Menu_Popup.accelerate(*this);
    m_Menu_Popup.show_all(); //Show all menu items when the menu pops up

    show_all_children();
}

void WindowObject::CloseAppWithoutHide(){
    std::map <std::string,SelfObject*>::iterator it;
    std::string name;
    for (it = selfobjects.begin(); it != selfobjects.end();++it){
        name = it->first;
        delete(selfobjects[name]);
        selfobjects.erase(name);
    }   

    std::map <std::string,ValueObject*>::iterator it2;

    for (it2 = valueobjects.begin(); it2 != valueobjects.end();++it2){
        name = it->first;
        delete(valueobjects[name]);
        valueobjects.erase(name);
    }

}

WindowObject::~WindowObject(){
    CloseAppWithoutHide();
}

bool WindowObject::on_button_press_event(GdkEventButton* button_event){
    bool return_value;

    return_value = Window::on_button_press_event(button_event);

    if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3)){
        m_Menu_Popup.popup(button_event->button, button_event->time);
    }

    return return_value;
}

void WindowObject::on_menu_file_popup_create(){
    int x,y;
    bool create;
    std::string name,buffer;
    char sendbuffer[200];
    auto newapp = Gtk::Application::create(argc, argv,"name.objeto");
    NameWindow namewindow(name,create);
    get_pointer(x,y);

    x = htonl(x);
    y = htonl(y);

    newapp->run(namewindow);

    if (create){
        buffer = "O"+std::string(1,name.size())+name;

        memset(sendbuffer,0,200);
        memcpy(sendbuffer, buffer.c_str(), buffer.size() );


        proxy.Send(sendbuffer,strlen(sendbuffer));

        memset(sendbuffer,0,200);
        memcpy(sendbuffer, &x, sizeof(int));
        proxy.Send(sendbuffer,sizeof(int));
        memcpy(sendbuffer, &y, sizeof(int) );
        proxy.Send(sendbuffer,sizeof(int));
    }
}

void WindowObject::on_menu_file_popup_close(){
    hide();
}

void WindowObject::on_refresh(){
    unsigned int i,size;
    size = actions.size();
    for (i = 0; i < size; i++){
        actions[i]->run();
        delete(actions[i]);
    }

    actions.clear();
}

void WindowObject::Invalidate(){
    queue_resize();
}

void WindowObject::AddAction(Actions *action){
    actions.push_back(action);
    Invalidate();
}

void WindowObject::AddObject(std::string name,int x, int y){
    selfobjects[name] = new SelfObject(name,x,y,argc,argv,proxy);
    fix.put(*selfobjects[name],x,y);
    selfobjects[name]->show();

}

void WindowObject::RemoveObject(std::string name){
    if (selfobjects.count(name) > 0){
        selfobjects[name]->hide();
        delete(selfobjects[name]);
        selfobjects.erase(name);
    } else {
        if (valueobjects.count(name) > 0){
            valueobjects[name]->hide();
            delete(valueobjects[name]);
            valueobjects.erase(name);
        }
    }
}

void WindowObject::AddValueObject(std::string name,std::string value,int x, int y){
    if (valueobjects.count(value) > 0){
        valueobjects[value]->AddParent(name);
    } else {
        valueobjects[value] = new ValueObject(value,x,y,proxy);
        valueobjects[value]->AddParent(name);
        fix.put(*valueobjects[value],x,y);
        valueobjects[value]->show();
    }
}

void WindowObject::AddSlot(std::string name, std::tuple<std::string, std::string,char,std::string> newslot){
    selfobjects[name]->AddSlot(newslot);
}

void WindowObject::RemoveSlot(std::string nameobject,std::string nameslot){
    selfobjects[nameobject]->RemoveSlot(nameslot);
}

void WindowObject::Move(std::string nameobject,int x, int y){
    if (selfobjects.count(nameobject) > 0){
        fix.move(*selfobjects[nameobject],x,y);
        selfobjects[nameobject]->SetPosition(x,y);
    }

    if (valueobjects.count(nameobject) > 0){
        fix.move(*valueobjects[nameobject],x,y);
        valueobjects[nameobject]->SetPosition(x,y);
    }
}

void WindowObject::ChangeNameObject(std::string nameobject, std::string newname){
    selfobjects[newname] = selfobjects[nameobject];
    selfobjects[newname]->ChangeName(newname);
    selfobjects.erase(nameobject);
}

void WindowObject::ChangeNameSlot(std::string nameobject,std::string nameslot,std::string newname){
    selfobjects[nameobject]->ChangeNameSlot(nameslot,newname);
}

void WindowObject::CloseApp(){
    hide();
}

bool WindowObject::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    on_refresh();

    return Gtk::Window::on_draw(cr);
}

bool WindowObject::on_delete_event(GdkEventAny* any_event){
    return Gtk::Window::on_delete_event(any_event);    
}

void WindowObject::ErrorMessage(std::string message){
    auto newapp = Gtk::Application::create(argc, argv,"error.message");
    ErrorWindow error(message);
    newapp->run(error);    
}

//void WindowObject::AddListLobby(std::tuple<unsigned char,std::string> newlobbies){
//     moveobject->AddLobbies(newlobbies);
//}

//void WindowObject::AddListObject(){
//     std::map <std::string,SelfObject*> ::iterator it;
//     unsigned char i = 0;

//     for (it = selfobjects.begin(); it != selfobjects.end(); ++it){
//         moveobject->AddObjects(std::make_tuple(i,it->first));
//         i++;    
//     }
//}

//void WindowObject::on_menu_file_popup_move(){
    // std::string name = "";
    // std::string buffer;
    // char sendbuffer[200];
    // unsigned char id;
    // char command = 'D'; 
    // auto newapp = Gtk::Application::create(argc, argv,"move.object");
    // moveobject = new MoveObject(id,name);
    // AddListObject();
    // proxy.Send(&command,sizeof(char));
    // newapp->run(*moveobject);

    // if (!name.empty()){
    //     buffer = std::string(1,id)+std::string(1,name.size())+name;
    //     memset(sendbuffer,0,200);
    //     memcpy(sendbuffer, buffer.c_str(), buffer.size() );
    //     proxy.Send(sendbuffer,strlen(sendbuffer));
    // }

    // hide();
    // delete(moveobject);
//}