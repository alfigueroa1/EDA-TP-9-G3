#include <algorithm>
#include <string>
#include <iostream>
#include <regex>
#include "viewer.h"
#include "hitachiLCD.h"

viewer::viewer(){
	bool valid = true;
	display = new hitachiLCD;
	if (!display->lcdInitOk())
		valid = false;
}

viewer::~viewer(){
	delete display;
}

void viewer::update(void*m){
	m = (model*)m; //Recibe model y lo castea a (model*) wut
}

void viewer::cycle(){
	//Ciclo adentro del while de lo que hace
}

bool viewer::getTweetState(){
	return state;
}

void viewer::restartTweet(tweet tw){
	//Reinicia el tweet que se estaba mostrando
}

void viewer::changeSpeed(int){
	//Ajusta la velocidad a la que se esta mostrando el tweet
}

void viewer::showEnd(){
	//Muestra que se llegó al final de los tweets
}

void displayError(){
	std::string error = "ERROR EN LA DESCARGA";
	display->lcdClear();
	display << error.c_str();
}

bool viewer::isValid()
{
	return valid;
}

void viewer::replaceChars(tweet& tw){
	std::string aux = tw.content;
	aux = std::regex_replace(aux, std::regex("á"), "a"); // replace all 'á' to 'a'
	aux = std::regex_replace(aux, std::regex("é"), "e");
	aux = std::regex_replace(aux, std::regex("í"), "i");
	aux = std::regex_replace(aux, std::regex("ó"), "o");
	aux = std::regex_replace(aux, std::regex("ú"), "u");
}

void displayDate(string date){
	display->lcdClear();
	display << date.c_str();
}
void displayContent(string content){
	display->lcdSetCursorPosition({ SECOND_ROW, 0 });
 	display->lcdClearToEOL();
	display << content.c_str();
}
void scrollTweet(string content){
	//Mueve el tweet por el display
}
void showUser(string username){
	display->lcdClear();
	display << username.c_str();
}
void showProcessing(){
	//Muestra animación de que se estan descargando los tweets (no bloqueante)}
}