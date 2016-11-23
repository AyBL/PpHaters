#ifndef ADDVALUEOBJECTACTION_H
#define ADDVALUEOBJECTACTION_H

#include <string>
#include "clientactions.h"

class WindowObject;

class AddValueAction : public Actions{
    public:
        AddValueAction(WindowObject* window, std::string name, std::string value, int x, int y);
        virtual ~AddValueAction();
        virtual void run();

    private:
        WindowObject *window;
        std::string name;
        std::string value;
        int x;
        int y;
};

#endif
