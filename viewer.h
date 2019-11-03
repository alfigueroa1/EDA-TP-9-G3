#pragma once

#include <iostream>
#include <chrono>
#include "basicLCD.h"
#include "hitachiLCD.h"
#include "observer.h"
#include "model.h"

using namespace std;

class viewer : public observer {

public:

	viewer(void*);								//Constructor: Crea con new el basicLCD
	~viewer();									//Destructor
	void update(void*);							//Recibe model y lo castea a (model*)
	void cycle();								//Ciclo adentro del while de lo que hace
	bool getTweetState(tweet);					//Avisa si el tweet terminó de pasar (true)
	void restartTweet();						//Reinicia el tweet que se estaba mostrando
	void changeSpeed(int);						//Ajusta la velocidad a la que se esta mostrando el tweet
	void showEnd();								//Muestra que se llegó al final de los tweets
	void initView();
	void noTweets();
	void noUser();
	void privateUser(); 
	void displayError();						//Avisa que hubo error en la descarga de tweets
	bool isValid(); 							//indica si hubo errores de construccion

private:
	hitachiLCD* display;
	model* m;
	bool valid;
	bool reset;
	unsigned int iter;
	unsigned int buf;
	string username;
	chrono::system_clock::time_point clock;
	chrono::duration<int, milli> tick;
	void replaceChars(tweet& tw);				//Reemplaza los caracteres que no existen por otros parecidos que si
	void displayDate(string date);				//Muestra fecha ya formateada en el primer renglon
	void displayContent(string content);		//Muestra tweet ya formateado en el segundo renglon
	//char* scrollTweet(string content);			//Mueve el tweet por el display
	//bool scrollTweet(string content, char buffer[16]);			//Mueve el tweet por el display
	void showUser(string username);				//Muestra usuario del cual se estan descargando los tweets (no bloqueante)
	void showProcessing();						//Muestra animación de que se estan descargando los tweets (no bloqueante)
};