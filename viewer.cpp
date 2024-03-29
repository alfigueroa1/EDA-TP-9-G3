#include <algorithm>
#include <string>
#include <iostream>
#include <regex>
#include <chrono>
#include "viewer.h"
#include "hitachiLCD.h"

using namespace std;

#define MAX_SPEED	100
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
	chrono::duration<int, milli> dur(300);
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
		initView();
		break;
	case ERR:
		if(!reset)
			displayError();
		reset = true;
		break;
	case DOWNLOADING:
		if (!reset)
			showUser(m->getUser());
		showProcessing();
		reset = true;
		break;
	case PARSING:
		if (reset)
			showParsing();
		reset = false;
		break;
	case NO_TWEETS:
		noTweets();
		break;
	case NO_USER:
		noUser();
		break;
	case PRIVATE_USER:
		if (!reset)
			privateUser();
			reset = true;
		break;
	case END:
		if(!reset){
			showEnd();
			reset = true;
		}
		break;
	case NEXT: case PREV: case REPEAT:
		reset = true;
		break;
	case STOP:
		stop();
		break;
	default:
		if (!m->emptyTweetList()) {
			if (reset) {
				displayDate(m->getTweet().date);
				restartTweet();
				reset = false;
			}
			if (!getTweetState(m->getTweet())) {
				tweet tw = m->getTweet();
				replaceChars(tw);
				displayContent(tw.content + "                ");
			}
			else{
				reset = true;
			}
		}
		break;
	}
}

bool viewer::getTweetState(tweet tw){
	if (iter >= tw.content.length() - 1) {
		reset = true;
	}
	else
		reset = false;
	return reset;
}

void viewer::restartTweet() {
	//Reinicia el tweet que se estaba mostrando
	iter = 0;
	buf = 0;
	display->lcdSetCursorPosition({2, 0});
}

void viewer::changeSpeed(int speed){
	//Ajusta la velocidad a la que se esta mostrando el tweet
	//chrono::duration<int, milli> bar((speed / 100) * MAX_SPEED);
	chrono::duration<int, milli> bar((500 - speed*5));
	this->tick = bar;
}

void viewer::stop() {
	reset = true;
}

void viewer::showEnd(){
	display->lcdClear();
	*display << "END";
	//Muestra que se llegó al final de los tweets
}

void viewer::displayError(){
	display->lcdClear();
	string error = "DOWNLOAD ERROR";
	display->lcdSetCursorPosition({2,1});
	*display << error.c_str();
}

bool viewer::isValid(){
	return valid;
}

void viewer::replaceChars(tweet& tw){
	std::string aux = tw.content;
	aux = regex_replace(aux, regex("á"), "a"); // replace all 'á' to 'a'
	aux = regex_replace(aux, regex("é"), "e");
	aux = regex_replace(aux, regex("í"), "i");
	aux = regex_replace(aux, regex("ó"), "o");
	aux = regex_replace(aux, regex("ú"), "u");
	aux = regex_replace(aux, regex("Á"), "A");
	aux = regex_replace(aux, regex("É"), "E");
	aux = regex_replace(aux, regex("Í"), "I");
	aux = regex_replace(aux, regex("Ó"), "O");
	aux = regex_replace(aux, regex("Ú"), "U");
	aux = regex_replace(aux, regex("ñ"), "ni");
	aux = regex_replace(aux, regex("Ñ"), "Ni");
	aux = regex_replace(aux, regex("\n"), "  ");
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
	if ((chrono::system_clock::now() > clock + tick) && !reset) {
		*display << content.c_str()[iter + buf];
		if (++buf == MAX_BUFFER){
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
}

void viewer::showUser(string username){
	display->lcdClear();
	*display << username.c_str();
}

void viewer::initView() {
	string content = "Tweets in LCD!  ";
	if (chrono::system_clock::now() > clock + tick) {
		*display << content.c_str()[buf];
		if (++buf == MAX_BUFFER) {
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
}

void viewer::noTweets() {
	string content = "   NO  TWEETS   ";
	char buffer[MAX_BUFFER] = { ' ', ' ', ' ', 'N', 'O', ' ', ' ', 'T', 'W', 'E', 'E', 'T', 'S', ' ', ' ', ' ' };
	if (chrono::system_clock::now() > clock + tick) {
		*display << content.c_str()[buf];
		if (++buf == MAX_BUFFER) {
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
}

void viewer::noUser() {
	string content = "   NO    USER   ";
	if (chrono::system_clock::now() > clock + tick) {
		*display << content.c_str()[buf];
		if (++buf == MAX_BUFFER) {
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
}

void viewer::privateUser() {
	string content = " PRIVATE USER ";
	if (chrono::system_clock::now() > clock + tick) {
		*display << content.c_str()[buf];
		if (++buf == MAX_BUFFER) {
			display->lcdSetCursorPosition({ 2, 0 });
			buf = 0;
			iter++;
		}
		clock = chrono::system_clock::now();
	}
}

void viewer::showProcessing(){
	//Muestra animación de que se estan descargando los tweets (no bloqueante)
	display->lcdSetCursorPosition({2, 7});
	if (chrono::system_clock::now() > clock + 4*tick) {
		*display << '|';
		clock = chrono::system_clock::now();
	}
	else if (chrono::system_clock::now() > clock + 3 * tick)
		*display << 0x60;
	else if (chrono::system_clock::now() > clock + 2 * tick)
		*display << '-';
	else if (chrono::system_clock::now() > clock + tick)
		*display << '/';
}

void viewer::showParsing() {
	display->lcdClear();
	*display << "PROCESSING";
	//Muestra que se estan leyendo los tweets
}