#include <algorithm>
#include <string>
#include <iostream>
#include <regex>
#include <chrono>
#include "viewer.h"
#include "hitachiLCD.h"

using namespace std;

#define MAX_SPEED 50

viewer::viewer(){
	bool valid = true;
	display = new hitachiLCD;
	if (!display->lcdInitOk())
		valid = false;
	clock = chrono::system_clock::now();
	chrono::duration<int, milli> dur(100);
	tick = dur;
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

void viewer::changeSpeed(int speed){
	//Ajusta la velocidad a la que se esta mostrando el tweet
	chrono::duration<int, milli> bar((speed / 100) * MAX_SPEED);
	tick = bar;
}

void viewer::showEnd(){
	display << "END";
	//Muestra que se llegó al final de los tweets
}

void displayError(){
	string error = "ERROR EN LA DESCARGA";
	display->lcdClear();
	display << error.c_str();
}

bool viewer::isValid()
{
	return valid;
}

void viewer::replaceChars(tweet& tw){
	std::string aux = tw.content;
	aux = regex_replace(aux, std::regex("á"), "a"); // replace all 'á' to 'a'
	aux = regex_replace(aux, std::regex("é"), "e");
	aux = regex_replace(aux, std::regex("í"), "i");
	aux = regex_replace(aux, std::regex("ó"), "o");
	aux = regex_replace(aux, std::regex("ú"), "u");
}

void viewer::displayDate(string date) {
	display->lcdClear();
	display << date.c_str();
}
void viewer::displayContent(string content){
	display->lcdSetCursorPosition({ SECOND_ROW, 0 });
 	display->lcdClearToEOL();
	display << content.c_str();
}
void viewer::scrollTweet(string content){
	//Mueve el tweet por el display
}
void viewer::showUser(string username){
	display->lcdClear();
	display << username.c_str();
}
void viewer::showProcessing(){
	//Muestra animación de que se estan descargando los tweets (no bloqueante)
	if (chrono::system_clock::now() > clock + tick) {
		display << '|';
		clock = chrono::system_clock::now();
	}
	else if (chrono::system_clock::now() > clock + 3 * tick / 4)
		display << '\\';
	else if (chrono::system_clock::now() > clock + tick / 2)
		display << '-';
	else if (chrono::system_clock::now() > clock + tick / 4)
		display << '/';
}