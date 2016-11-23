#ifndef REMOVEOBJECTACTION_H
#define REMOVEOBJECTACTION_H

#include <string>
#include "clientactions.h"

class WindowObject;

class RemoveObjectAction : public Actions{
    public:
        RemoveObjectAction(WindowObject *window, std::string name);
        virtual ~RemoveObjectAction();
        virtual void run();

    private:
        WindowObject *window;
        std::string name;
};

#endif
