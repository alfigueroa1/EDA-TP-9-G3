#include <iostream>
#include "model.h"
#include "controller.h"
#include "viewer.h"


int main(void) {
	model model;
	controller controller(&model);//, viewer);
	viewer viewer(&model);
	controller.running = true;
	
	while (controller.running) {
		controller.cycle();
		viewer.update(&model);
		viewer.cycle();
	}

	return 0; //Fin del programa
}
