#include <algorithm>
#include <string>
#include <iostream>
#include <regex>
#include <chrono>
#include "viewer.h"
#include "hitachiLCD.h"

using namespace std;

#define MAX_SPEED	50
#define MAX_BUFFER	16

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

void viewer::update(void*mod){
	m = (model*)mod; //Recibe model y lo castea a (model*)
}

void viewer::cycle() {
	//Ciclo adentro del while de lo que hace
	changeSpeed(m->getSpeed());
	if (m->isDownloading()) {
		if (m->getState() == ERR)
			displayError();
		else {
			showUser(m->getUser());
			showProcessing();
		}
	}
	else {
		if (m->getState() == END)
			showEnd();
		else {
			if (getTweetState(m->getTweet()))
				restartTweet(m->getTweet());
			displayDate(m->getTweet().date);
			displayContent(m->getTweet().content);
		}
	}
}

bool viewer::getTweetState(tweet tw){
	bool r = false;
	if(iter == tw.content.length())
		r = true;
	return r;
}

void viewer::restartTweet(tweet tw){
	//Reinicia el tweet que se estaba mostrando
	iter = 0;
}

void viewer::changeSpeed(int speed){
	//Ajusta la velocidad a la que se esta mostrando el tweet
	chrono::duration<int, milli> bar((speed / 100) * MAX_SPEED);
	tick = bar;
}

void viewer::showEnd(){
	*display << (const unsigned char*)"END";
	//Muestra que se llegó al final de los tweets
}

void viewer::displayError(){
	string error = "ERROR EN LA DESCARGA";
	display->lcdClear();
	*display << (const unsigned char*)error.c_str();
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
	*display << (const unsigned char*)date.c_str();
}

void viewer::displayContent(string content){
	display->lcdSetCursorPosition({ 2, 0 });
 	display->lcdClearToEOL();
	if(char* buffer = scrollTweet(content))
		*display << (const unsigned char*)buffer;
}

char* viewer::scrollTweet(string content){
	//Mueve el tweet por el display
	char buffer[MAX_BUFFER];
	if (chrono::system_clock::now() > clock + tick) {
		for (int i = 0; i < MAX_BUFFER; i++)
			buffer[i] = content.c_str()[iter + i];
		iter++;
		clock = chrono::system_clock::now();
		return buffer;
	}
	return NULL;
}
void viewer::showUser(string username){
	display->lcdClear();
	*display << (const unsigned char*)username.c_str();
}

void viewer::showProcessing(){
	//Muestra animación de que se estan descargando los tweets (no bloqueante)
	display->lcdSetCursorPosition({2, 0});
	if (chrono::system_clock::now() > clock + tick) {
		*display << '|';
		clock = chrono::system_clock::now();
	}
	else if (chrono::system_clock::now() > clock + 3 * tick / 4)
		*display << '\\';
	else if (chrono::system_clock::now() > clock + tick / 2)
		*display << '-';
	else if (chrono::system_clock::now() > clock + tick / 4)
		*display << '/';
}