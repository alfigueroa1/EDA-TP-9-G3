#include <iostream>
#include <fstream> 
#include <ctime>
#include <sstream>
#include <locale> 
#include <list>
#include <string>
#include <vector>
#include <exception>
#include "json.hpp"
#include "model.h"
#include <algorithm>

using namespace std;


model::model() {
	if (twitter.getError()) {
		//report error
	}
	currentTransfers = 0;
	state = INIT;
	//downloading = false;
	username = "NASA";
	curr = tweetList.begin();
}

//model::~model() {
//}

void model::notifyAllObservers() {
	for (observer* o : observers) {
		o->update(this);
	}
	return;
}
int model::getMoreTweets() {
	int r = twitter.getTweets(username, maxTweets, &currentTransfers);
	if (currentTransfers == 0) {
		state = FINISHED_DOWNLOAD;
		string userData = twitter.getUserData();
		if (!userData.empty()) {
			ofstream o("twitter.json");
			o << userData;
			o.close();
		}
		parseTweets();
	}
	else
		state = DOWNLOADING;
	return r;
}

//Getters y Setters
string model::getTweetDate() {
	return curr->date;
}

string model::getTweetContent() {
	return curr->content;
}

string model::getUser() {
	return username;
}

void model::setUser(string user) {
	username = user;
}

int model::getSpeed() {
	return speed;
}

void model::setSpeed(int velocidad) {
	speed = velocidad;
}

int model::getMaxTweets() {
	return maxTweets;
}

void model::setMaxTweets(int max) {
	maxTweets = max;
}

stateType model::getState(){
	return state;
};

void model::setState(stateType estado) {
	state = estado;
}

//Manejo de la lista de Tweets
tweet model::getTweet(){
	return *curr;
}

bool model::emptyTweetList() {
	return tweetList.empty();
}


bool model::goPrevious() {
	bool start = false;
	if (state == END)
		state = PREV;
	if (curr != tweetList.begin())
		curr--;
	else {
		start = true;
	}
	return start;
}

bool model::goNext() {
	bool end = false;
	if (curr != tweetList.end() - 1)
		curr++;
	else {
		state = END;
		end = true;
	}
	return end;
}

void model::stop(){
	state = FINISH_EARLY;
	currentTransfers = 0;
}


bool model::parseTweets() {
	json j;
	bool r = true;

	try {

		ifstream i("twitter.json", std::ifstream::in);									//Read JSON file
		i >> j;
		i.close();
	}
	catch (exception& e)
	{
		json::iterator it = j.end();
		it--;

		j.erase(it);
	}

	try
	{
		std::vector<json> statuses = j;
		for (auto& element : statuses) {
			tweet tw;
			tw.date = element["created_at"];
			tw.content = element["text"];
			makeDate(tw);
			makeDialogue(tw);														//Format content
			tweetList.push_back(tw);												//Add tweet to list

		}
		curr = tweetList.begin();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;		//Display the error given by the json library
	}


	/*if (j.empty())
	{
		cout << "No tweets" << 0;
	}
	else if()
	{
		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			if (it.key() == "errors")
			{
				cout << "El usuario no existe" << endl;
				r = false;
			}

			else if (it.key() == "error")
			{
				cout << "La cuenta es privada" << endl;
				r = false;
			}
		}
	}*/

	return r;

	//for (curr = tweetList.begin(); curr != tweetList.end(); curr++) {					//Print tweets (debug)
	//	cout << "Date: " + curr->date << endl;
	//	cout << "Content: " + curr->content << endl;
	//}

	/*ofstream myfile;																	//Pass tweets to a file (debug)
	myfile.open("text.txt");
	for (curr = tweetList.begin(); curr != tweetList.end(); curr++) {
		myfile << "Date: " + curr->date << endl;
		myfile << "Content: " + curr->content << endl;
	}
	myfile.close();*/
}

void model::makeDate(tweet& tw) {
	tm when;
	locale loc;
	auto& tmget = use_facet <time_get<char>>(loc);
	ios::iostate state;
	istringstream iss(tw.date);
	string format("%a %b %d %T +0000 %Y");
	
	tmget.get(iss, std::time_get<char>::iter_type(), iss, state, &when,				//Convert string to tm
		format.data(), format.data() + format.length());

	char buffer[16];
	strftime(buffer, 16, "%x %R", &when);											//Convert tm to string in desired format
	tw.date = buffer;
}

void model::makeDialogue(tweet& tw) {
	string s = tw.content;
	int extended = s.find("https");					//Find the url to erase it
	s = s.substr(0, extended - 4);					//Erase the url
	tw.content = username + ": - " + s + " -";		//Convert to required format
}
