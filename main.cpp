#include <iostream>
#include "model.h"





int main(void) {
	bool running = true;
	model model;
	controller controller;
	viewer viewer;

	while(running)
		controller.cycle();
}

