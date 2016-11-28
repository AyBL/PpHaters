#ifndef ERRORACTION_H
#define ERRORACTION_H

#include <string>
#include "clientactions.h"

class WindowObject;

class ErrorAction : public Actions{
    public:
        ErrorAction(WindowObject* window, std::string message);
        virtual ~ErrorAction();
        virtual void run();

    private:
        WindowObject *window;
        std::string message;
};

#endif
