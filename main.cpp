#include "model.h"
int main(void) {
	model model;
	controller controller;
	viewer viewer;

	while(running)
		controller.cycle();
}