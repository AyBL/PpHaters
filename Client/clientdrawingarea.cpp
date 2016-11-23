#include "clientdrawingarea.h"

#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>

MyDrawingArea::MyDrawingArea(std::map <std::string,SelfObject*> &selfobjects,
std::map <std::string,ValueObject*> &valueobjects):selfobjects(selfobjects),
valueobjects(valueobjects){
    set_size_request (1366, 768);

    m_image = Gdk::Pixbuf::create_from_resource("/image/Iron-Throne.jpg");

}

MyDrawingArea::~MyDrawingArea(){
}

bool MyDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
    Gtk::Allocation allocation = get_allocation();

    std::map <std::string,ValueObject*>::iterator it;
    std::map <std::string,SelfObject*>::iterator it2;
    int posxso,posyso,hso,wso; //SelfObject
    int posxvo,posyvo,hvo,wvo; //ValueObject
    unsigned int i;

    if (m_image){
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        // Draw the image in the middle of the drawing area, or (if the image is
        // larger than the drawing area) draw the middle part of the image.
        Gdk::Cairo::set_source_pixbuf(cr, m_image,
          (width - m_image->get_width())/2, (height - m_image->get_height())/2);
        cr->paint();

    }

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
