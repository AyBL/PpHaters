#include "clientdrawingarea.h"

MyDrawingArea::MyDrawingArea(std::map <std::string,SelfObject*> &selfobjects,
std::map <std::string,ValueObject*> &valueobjects):selfobjects(selfobjects),
valueobjects(valueobjects){
    set_size_request (1366, 768);
}

MyDrawingArea::~MyDrawingArea(){
}

bool MyDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    std::map <std::string,ValueObject*>::iterator it;
    int posxso,posyso,hso,wso; //SelfObject
    int posxvo,posyvo,hvo,wvo; //ValueObject
    int i;

    for (it = valueobjects.begin(); it != valueobjects.end(); ++it){
        ValueObject *voaux = it->second;
        std::vector<std::string> parents = voaux->GetParents();
        for (i = 0; i < parents.size(); i++){
            SelfObject *soaux = selfobjects[parents[i]];
            soaux->GetPosition(posxso,posyso);
            hso = soaux->get_height();
            wso = soaux->get_width();
            voaux->GetPosition(posxvo,posyvo);
            hvo = voaux->get_height();
            wvo = voaux->get_width();
            cr->set_line_width(2.0);
            cr->set_source_rgb(0.8, 0.0, 0.0);
            cr->move_to(posxso+wso/2,posyso+hso/2);
            cr->line_to(posxvo+wvo/2,posyvo+hvo/2);
            cr->stroke();
        }
    }
    Gtk::DrawingArea::on_draw(cr);
}
