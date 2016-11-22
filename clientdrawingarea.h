#ifndef MYDRAWINGAREA_H
#define MYDRAWINGAREA_H

#include <gtkmm/drawingarea.h>
#include <map>
#include <string>
#include "clientselfobject.h"
#include "clientvaluewindow.h"

class MyDrawingArea : public Gtk::DrawingArea{
    public:
        MyDrawingArea(std::map <std::string,SelfObject*> &selfobjects,
        std::map <std::string,ValueObject*> &valueobjects);
        virtual ~MyDrawingArea();

    protected:
        //Override default signal handler:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        std::map <std::string,SelfObject*> &selfobjects;
        std::map <std::string,ValueObject*> &valueobjects;
};

#endif
