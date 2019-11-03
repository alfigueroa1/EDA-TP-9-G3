#include <algorithm>
#include <string>
#include <iostream>
#include <regex>
#include <chrono>
#include "viewer.h"
#include "hitachiLCD.h"

using namespace std;

#define MAX_SPEED	500
#define MAX_BUFFER	16

viewer::viewer(void* mod){
	valid = true;
	display = new hitachiLCD;			//UNRESOLVED EXTERNAL
	if (!display->lcdInitOk())
		valid = false;
	iter = 0;
	buf = 0;
	m = (model*) mod;
	clock = chrono::system_clock::now();
	chrono::duration<int, milli> dur(100);
	tick = dur;
	reset = false;
}

viewer::~viewer(){
	display->lcdClear();
	delete display;
}

void viewer::update(void*mod){
	m = (model*)mod;					//Recibe model y lo castea a (model*)
}

void viewer::cycle() {
	//Ciclo adentro del while de lo que hace
	switch (m->getState()) {
	case INIT:
		display->lcdClear();
		*display << "Tweets in LCD!";
		break;
	case ERR:
		displayError();
		reset = true;
		break;
	case DOWNLOADING:
		showUser(m->getUser());
		showProcessing();
		reset = true;
		break;
	case NO_TWEETS:
		noTweets();
		break;
	case NO_USER:
		noUser();
		break;
	case PRIVATE_USER:
		privateUser();
		break;
	case END:
		showEnd();
		reset = true;
		break;
	case NEXT: case PREV: case REPEAT:
		reset = true;
		break;
	default:
		if (!m->emptyTweetList()) {
			if (reset) {
				displayDate(m->getTweet().date);
				reset = false;
			}
			if (!getTweetState(m->getTweet())) {
				tweet tw = m->getTweet();
				replaceChars(tw);
				displayContent(tw.content);
			}
			else{
				reset = true;
			}
		}
		break;
	}
}

bool viewer::getTweetState(tweet tw){
	if (iter >= tw.content.length() - MAX_BUFFER - 1) {
		restartTweet(tw);
		reset = true;
	}
	else
		reset = false;
	return reset;
}

void viewer::restartTweet(tweet tw){
	//Reinicia el tweet que se estaba mostrando
	iter = 0;
	buf = 0;
}

void viewer::changeSpeed(int speed){
	//Ajusta la velocidad a la que se esta mostrando el tweet
	chrono::duration<int, milli> bar((speed / 100) * MAX_SPEED);
	tick = bar;
}

void viewer::showEnd(){
	display->lcdClear();
	*display << "END";
	//Muestra que se llegó al final de los tweets
}

void viewer::displayError(){
	display->lcdClear();
	string error = "ERROR EN LA DESCARGA";
	display->lcdSetCursorPosition({2,1});
	*display << error.c_str();
}

bool viewer::isValid(){
	return valid;
}

void viewer::replaceChars(tweet& tw){
	std::string aux = tw.content;
	aux = regex_replace(aux, std::regex("á"), "a"); // replace all 'á' to 'a'
	aux = regex_replace(aux, std::regex("é"), "e");
	aux = regex_replace(aux, std::regex("í"), "i");
	aux = regex_replace(aux, std::regex("ó"), "o");
	aux = regex_replace(aux, std::regex("ú"), "u");
	tw.content = aux;
}

void viewer::displayDate(string date) {
	display->lcdSetCursorPosition({ 1, 1 });
	display->lcdClearToEOL();
	*display << date.c_str();
	display->lcdSetCursorPosition({ 2, 0 });
}

void viewer::displayContent(string content) {
	//display->lcdClearToEOL();
	if (chrono::system_clock::now() > clock + tick) {
		*display << content.c_str()[iter + buf];
		if (++buf == MAX_BUFFER){
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
	/*if (scrollTweet(content, buffer))
		*display << buffer;*/
}



//bool viewer::scrollTweet(string content, char buffer[MAX_BUFFER]) {
//	//Mueve el tweet por el display
//	if (chrono::system_clock::now() > clock + tick) {
//		for (int i = 0; i < MAX_BUFFER; i++)
//			buffer[i] = content.c_str()[iter + i];
//		iter++;
//		clock = chrono::system_clock::now();
//		return true;
//	}
//	return false;
//}
/*void viewer::displayContent(string content){
	display->lcdSetCursorPosition({ 2, 0 });
 	display->lcdClearToEOL();
	if(char* buffer = scrollTweet(content))
		*display << buffer;
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
}*/
void viewer::showUser(string username){
	display->lcdClear();
	*display << username.c_str();
}

void viewer::noTweets() {
	display->lcdClear();
	char buffer[MAX_BUFFER] = { ' ', ' ', ' ', 'N', 'O', ' ', ' ', 'T', 'W', 'E', 'E', 'T', 'S', ' ', ' ', ' ' };
	*display << buffer;
}

void viewer::noUser() {
	display->lcdClear();
	char buffer[MAX_BUFFER] = { 'N', 'O', 'N', 'E', 'X', 'I', 'S', 'T', 'E', 'N', 'T', ' ', 'U', 'S', 'E', 'R' };
	*display << buffer;
}

void viewer::privateUser() {
	display->lcdClear();
	char buffer[MAX_BUFFER] = { ' ', ' ', 'P', 'R', 'I', 'V', 'A', 'T', 'E', ' ', 'U', 'S', 'E', 'R', ' ', ' ' };
	*display << buffer;
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