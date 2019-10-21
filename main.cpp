#include <iostream>
#include "model.h"
#include "controller.h"
#include "viewer.h"




int main(void) {
	bool running = true;
	model model;
	viewer viewer;
	
	controller controller(model);
	controller.cycle();
	viewer.cycle();


	////while (running)
	////	controller.cycle();

	//eventType ev = NO_EVENT;	//Inicializa la variable "evento" en "NO EVENTO"
	//while (ev != QUIT) {		//Hasta que el usuario decida salir del programa
	//	ev = eventHandler.getNextEventType();	//Consigue el proximo evento
	//	if (ev != NO_EVENT && ev != QUIT) {		//Si no es el evento "nulo" o el evento de salida...
	//		dispatch(ev, &front, &worm1, &worm2);	//...lo deriva a la funcion correspondiente
	//	}
	//}

	return 0; //Fin del programa
}

/*******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 ******************************************************************************/

//static void dispatch(eventType ev, allegroClass* frontP, Worm* wormP1, Worm* wormP2) {
//	switch (ev) { //Segun el evento recibido, emplea los metodos de la clase indicada
//
//		//Inputs para el "jugador 1" (flechas del teclado)
//	case UP_PRESSED:
//		wormP1->goJump();
//		break;
//	case UP_RELEASED:
//		wormP1->stopJump();
//		break;
//	case LEFT_PRESSED:
//		wormP1->goLeft();
//		break;
//	case LEFT_RELEASED:
//		wormP1->stopLeft();
//		break;
//	case RIGHT_PRESSED:
//		wormP1->goRight();
//		break;
//	case RIGHT_RELEASED:
//		wormP1->stopRight();
//		break;
//
//		//Inputs para el "jugador 2" (teclas A,W,D)
//	case W_PRESSED:
//		wormP2->goJump();
//		break;
//	case W_RELEASED:
//		wormP2->stopJump();
//		break;
//	case A_PRESSED:
//		wormP2->goLeft();
//		break;
//	case A_RELEASED:
//		wormP2->stopLeft();
//		break;
//	case D_PRESSED:
//		wormP2->goRight();
//		break;
//	case D_RELEASED:
//		wormP2->stopRight();
//		break;
//
//		//Input del timer: actualiza el estado de los Worms y grafica
//	case REFRESH:
//		wormP1->update();
//		wormP2->update();
//		frontP->graph(*wormP1, *wormP2);
//		break;
//	}
//}