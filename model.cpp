#include <iostream>
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

#define TWEET_MAX

using json = nlohmann::json;

using namespace std;

model::model() {

	username = "NASA";
	curr = tweetList.begin();

}

//model::~model() {
//}

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

//Manejo de la lista de Tweets
void model::goPrevious() {
	if (curr != tweetList.begin())
		curr--;
}

void model::goNext() {
	if (curr != tweetList.end())
		curr++;
}

//void model::repeat() {}

void model::error() {

}

void model::setUser(string user) {
	username = user;
}

void model::parseTweet() {
	std::ifstream i("twitter.json", std::ifstream::in);									//Read JSON file
	json j;
	i >> j;
	i.close();

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
	
	tmget.get(iss, std::time_get<char>::iter_type(), iss, state, &when,
		format.data(), format.data() + format.length());

	char buffer[16];
	strftime(buffer, 16, "%x %R", &when);
	tw.date = buffer;
}

void model::makeDialogue(tweet& tw) {
	string s = tw.content;
	int extended = s.find("https");					//Find the url to erase it
	s = s.substr(0, extended - 4);					//Erase the url
	tw.content = username + ": - " + s + " -";		//Convert to required format
}
