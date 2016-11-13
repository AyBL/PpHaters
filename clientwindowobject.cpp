#include <iostream>
#include "clientwindowobject.h"

WindowObject::WindowObject(int argc, char **argv,std::string name): argc(argc),argv(argv) {

    auto item = Gtk::manage(new Gtk::MenuItem("_Create Object", true));
    item->signal_activate().connect(
        sigc::mem_fun(*this, &WindowObject::on_menu_file_popup_create) );
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

    //Call base class, to allow normal handling,
    //such as allowing the row to be selected by the right-click:
    return_value = Window::on_button_press_event(button_event);

    //Then do our custom stuff:
    if ((button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3)){
        m_Menu_Popup.popup(button_event->button, button_event->time);
    }

    return return_value;
}

void WindowObject::on_menu_file_popup_create(){
    int x,y;
    std::string name;
    auto newapp = Gtk::Application::create(argc, argv,"Message");
    NameWindow namewindow(name);
    get_pointer(x,y);
    newapp->run(namewindow);


    	// slots.push_back(std::make_tuple("Que","10",false));
    	// slots.push_back(std::make_tuple("hijo","10",false));
    	// slots.push_back(std::make_tuple("de","10",true));
    	// slots.push_back(std::make_tuple("puta","10",false));
    	// slots.push_back(std::make_tuple("el","10",true));

    selfobjects[name] = new SelfObject(name, argc,argv);
    fix.put(*selfobjects[name],x,y);
    selfobjects[name]->show();

}

void WindowObject::AddObject(std::string name){
    int x,y;
    srand(time(NULL));
    x = rand()%WIDTH;
    y = rand()%HEIGHT;
    selfobjects[name] = new SelfObject(name, argc,argv);
    fix.put(*selfobjects[name],x,y);
    selfobjects[name]->show();
}

void WindowObject::AddSlot(std::string name, std::tuple<std::string, std::string,bool,std::string> newslot){
    selfobjects[name]->AddSlot(newslot);
}

void WindowObject::RemoveSlot(std::string nameobject,std::string nameslot){
    selfobjects[nameobject]->RemoveSlot(nameslot);
}
