#include <iostream>
#include <vector>
#include "model.h"

using namespace std;




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

void model::setDate(string _date) {
	curr->date = _date;
}
void model::setUser(string user) {
	username = user;
}
void model::makeDialogue() {

}