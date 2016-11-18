#ifndef CLIENT_H
#define CLIENT_H

#include "clientsocket.h"
#include "clientThread.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include <cstring>
#include <iostream>
#include <tuple>
#include <arpa/inet.h>
#include "clientaddobjectaction.h"
#include "clientremoveobjectaction.h"
#include "clientaddvalueaction.h"

#define MAXSIZE 2000
#define COMMANDSIZE 1

class WindowObject;

class Serverproxy: public Thread{
    public:
        Serverproxy(MenuWindow **menu, WindowObject **window);
        virtual void run();
        int SendCommand(char* stringbuffer);
        int SendPositions(char* stringbuffer);
        //Destruye los elementos de la estructura
        virtual ~Serverproxy();
        int Connect(const char *hostname,const char *port);
        int ReceiveAnswer();

        void Stop();
    protected:

    private:
        Socket socket;
        MenuWindow **menu;
        WindowObject **window;

        char bufferanswer[MAXSIZE];
        char buffercommand[MAXSIZE];
        char command;

        bool must_be_run;

        void CommandA();
        void CommandD();
        void CommandE();
        void CommandL();
        void CommandM();
        void CommandO();
        void CommandP();
        void CommandS();

};

#endif
