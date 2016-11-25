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
    std::map <std::string,SelfObject*>::iterator it2;
    int posxso,posyso,hso,wso; //SelfObject
    int posxvo,posyvo,hvo,wvo; //ValueObject
    unsigned int i;

    for (it = valueobjects.begin(); it != valueobjects.end(); ++it){
        ValueObject *voaux = it->second;
        std::vector<std::string> parents = voaux->GetParents();
        voaux->GetPosition(posxvo,posyvo);
        hvo = voaux->get_height();
        wvo = voaux->get_width();
        for (i = 0; i < parents.size(); i++){
            SelfObject *soaux = selfobjects[parents[i]];
            soaux->GetPosition(posxso,posyso);
            hso = soaux->get_height();
            wso = soaux->get_width();
            cr->set_line_width(2.0);
            cr->set_source_rgb(0.8, 0.0, 0.0);
            cr->move_to(posxso+wso/2,posyso+hso/2);
            cr->line_to(posxvo+wvo/2,posyvo+hvo/2);
            cr->stroke();
        }

        cr->set_source_rgb(1, 1, 1);
        cr->rectangle(posxvo, posyvo, wvo, hvo);
        cr->fill();
    }

    for (it2 = selfobjects.begin(); it2 != selfobjects.end(); ++it2){
        SelfObject *soaux = it2->second;
        soaux->GetPosition(posxso,posyso);
        hso = soaux->get_height();
        wso = soaux->get_width();
        cr->set_source_rgb(1, 1, 1);
        cr->rectangle(posxso, posyso, wso, hso);
        cr->fill();
    }



    return Gtk::DrawingArea::on_draw(cr);
}
