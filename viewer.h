//#pragma once
//
//#include <iostream>
//#include "basicLCD.h"
//#include "observer.h"
//#include "model.h"
//
//using namespace std;
//
//class viewer : public observer {
//
//public:
//
//	viewer();									//Constructor: Crea con new el basicLCD
//	~viewer();									//Destructor
//	void update(void*);							//Recibe model y lo castea a (model*)
//	void cycle();								//Ciclo adentro del while de lo que hace
//	bool getTweetState();						//Avisa si el tweet terminó de pasar (true)
//	void restartTweet(tweet tw);				//Reinicia el tweet que se estaba mostrando
//	void changeSpeed(int);						//Ajusta la velocidad a la que se esta mostrando el tweet
//	void showEnd();								//Muestra que se llegó al final de los tweets
//	void displayError();						//Avisa que hubo error en la descarga de tweets
//	bool isValid(); 							//indica si hubo errores de construccion
//
//private:
//	basicLCD * display;
//	bool valid;
//	stateType state;
//	string username;
//	void replaceChars();						//Reemplaza los caracteres que no existen por otros parecidos que si
//	void displayDate(string date);				//Muestra fecha ya formateada en el primer renglon
//	void displayContent(string content);		//Muestra tweet ya formateado en el segundo renglon
//	void scrollTweet(string content);			//Mueve el tweet por el display
//	void showUser();							//Muestra usuario del cual se estan descargando los tweets (no bloqueante)
//	void showProcessing();						//Muestra animación de que se estan descargando los tweets (no bloqueante)
//}