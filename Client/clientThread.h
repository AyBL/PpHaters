#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <thread>

class Thread {
    private:
        std::thread thread;

    public:
        Thread();//Constructor
        void start();//Inicializa el thread
        void join();//Joinea el thread
        virtual void run() = 0;//Metodo abstracto
        virtual ~Thread();//Destructor

    private:
        //Objeto no copiable
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        //asignacion por movimiento
        Thread(Thread&& other);
        Thread& operator=(Thread&& other);
};

#endif
