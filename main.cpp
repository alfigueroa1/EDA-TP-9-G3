#include <iostream>
#include "model.h"
#include "controller.h"
#include "viewer.h"


int main(void) {
	model model;
	viewer viewer(&model);
	controller controller(&model, &viewer);
	controller.running = true;
	
	while (controller.running) {
		controller.cycle();
		viewer.cycle();
	}

	return 0; //Fin del programa
}
