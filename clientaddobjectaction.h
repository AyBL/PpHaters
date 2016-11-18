#ifndef ADDOBJECTACTION_H
#define ADDOBJECTACTION_H

#include <string>
#include "clientactions.h"

class WindowObject;

class AddObjectAction : public Actions{
    public:
        AddObjectAction(WindowObject* window, std::string name, int x, int y);
        virtual ~AddObjectAction();
        virtual void run();

    private:
        WindowObject *window;
        std::string name;
        int x;
        int y;
};

#endif
