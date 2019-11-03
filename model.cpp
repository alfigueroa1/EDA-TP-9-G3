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
	if (curr != tweetList.begin()) {
		curr--;
		state = PREV;
	}
	else {
		start = true;
	}
	return start;
}

bool model::goNext() {
	bool end = false;
	if (curr != tweetList.end() - 1) {
		curr++;
		state = NEXT;
	}
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
	catch (exception & e)
	{
		json::iterator it = j.end();
		it--;

		j.erase(it);
	}

	if(!j.empty() && j.is_array()){
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
	}
	else if (j.empty()){
		state = NO_TWEETS;
	}
	else{
		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			if (it.key() == "errors")
			{
				state = NO_USER;
				r = false;
			}
			else if (it.key() == "error")
			{
				state = PRIVATE_USER;
				r = false;
			}
		}
	}

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

//El de tupapasitoGian
/*void Client::getTweets()
{
	tweets.clear();
	ofstream o("prueba1.json");
	o << aux;
	o.close();

	try {

		ifstream i("prueba1.json");
		i >> file;
		i.close();
	}

	catch (exception & e)
	{
		json::iterator it = file.end();
		it--;

		file.erase(it);
	}

	if (!file.empty() && file.is_array())
	{
		for (auto& element : file)
		{
			auto twt = element["text"];
			auto date = element["created_at"];

			tweets.push_back(twt);
			dates.push_back(date);
		}
	}

	else if (file.empty())
	{
		//ERROR NO HAY TWEETS
		cout << "No hay tweets" << endl;
	}

	else
	{
		for (json::iterator it = file.begin(); it != file.end(); ++it)
		{
			if (it.key() == "errors")
			{
				cout << "Persona no existe" << endl;
			}

			else if (it.key() == "error")
			{
				cout << "Persona candadito" << endl;
			}
		}
	}

	return;
}*/

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
	s = s.substr(0, extended);						//Erase the url
	tw.content = username + ": - " + s + " -";		//Convert to required format
}
