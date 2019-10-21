#pragma once

#include <iostream>
#include "observer.h"
#include "model.h"

using namespace std;

class viewer : public observer {

public:
	void update(void*);							//Recibe model y lo castea a (model*)
	bool getTweetState();						//Avisa si el tweet terminó de pasar (true)
	void restartTweet(tweet tw);				//Reinicia el tweet que se estaba mostrando
	void changeSpeed(int);						//Ajusta la velocidad a la que se esta mostrando el tweet


private:
	void replaceChars();						//Reemplaza los caracteres que no existen por otros parecidos que si
	void displayDate(string date);				//Muestra fecha ya formateada en el primer renglon
	void displayContent(string content);		//Muestra tweet ya formateado en el segundo renglon
	void scrollTweet(string content);			//Mueve el tweet por el display
	void displayError();						//Avisa que hubo error en la descarga de tweets
	void showUser();							//Muestra usuario del cual se estan descargando los tweets (no bloqueante)
	void showProcessing();						//Muestra animación de que se estan descargando los tweets (no bloqueante)

	//agregar datos miembro a gusto

};












