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
	downloading = false;
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
		downloading = false;
		string userData = twitter.getUserData();
		if (!userData.empty()) {
			ofstream o("twitter.json");
			o << userData;
			o.close();
		}
	}
	else
		downloading = true;
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

void model::setMaxTweets(int max) {
	maxTweets = max;
}

//Manejo de la lista de Tweets
tweet model::getTweet(){
	return *curr;
}


bool model::goPrevious() {
	bool start = false;
	if (curr != tweetList.begin())
		curr--;
	else
		start = true;
	return start;
}

bool model::goNext() {
	bool end = false;
	if (curr != tweetList.end())
		curr++;
	else
		end = true;
	return end;
}

void model::stop(){
	downloading = false;
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

	if (!j.empty() && j.is_array()) {
		for (json::iterator it = j.begin(); it != j.end(); it++) {							//Look for tweets
			if (it.key() == "statuses") {
				for (auto element : *it) {													//Found a tweet
					tweet tw;
					for (json::iterator k = element.begin(); k != element.end(); k++) {
						if (k.key() == "created_at") {										//Found date
							tw.date = k.value().get<string>();
							makeDate(tw);													//Format date
						}
						else if (k.key() == "text") {										//Found content
							tw.content = k.value().get<string>();
							makeDialogue(tw);												//Format content
							break;
						}
					}
					tweetList.push_back(tw);												//Add tweet to list
				}
			}
		}
	}
	else if (j.empty())
	{
		cout << "No tweets" << 0;
	}
	else
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
