#include "mybutton.h"

MyButton::MyButton():button("Close"){
	//Agregamos el boton a la ventana
	add(button);

	//Conectamos el boton con la señal
	button.signal_clicked().connect( sigc::mem_fun(*this,
              &MyButton::button_close) );

	//Hacemos visibles todos los widgets
	show_all_children();
}


MyButton::~MyButton(){
}

void MyButton::button_close(){
	hide();
}