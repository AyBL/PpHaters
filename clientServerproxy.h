#ifndef CLIENT_H
#define CLIENT_H

#include "commonsocket.h"
#include "commonThread.h"
#include "clientmenuwindow.h"
#include <gtkmm/application.h>
#include <cstring>
#include <iostream>
#include <tuple>
#include <arpa/inet.h>

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

        void CommandD();
        void CommandE();
        void CommandL();
        void CommandM();
        void CommandO();
        void CommandS();

};

#endif
