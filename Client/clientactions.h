#ifndef ACTIONS_H
#define ACTIONS_H

class Actions{
    public:
        Actions();
        virtual ~Actions();
        virtual void run();

    private:
    	//Objeto no copiable
        Actions(const Actions&) = delete;
        Actions& operator=(const Actions&) = delete;
        //asignacion por movimiento
        Actions(Actions&& other);
        Actions& operator=(Actions&& other);

};

#endif
