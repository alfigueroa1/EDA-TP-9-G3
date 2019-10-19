#pragma once
#include <iostream>
#include <list>
#include <vector>


using namespace std;

struct tweet {
	string date;
	string content;
};

class model {
public:
	//Constructor y destructor
	model();
	//~model();

	//Getters y Setters
	string getTweetDate();
	string getTweetContent();
	string getUser();

	//Manejo de la lista de Tweets
	void parseTweet();
	void goPrevious();
	void goNext();
	//void repeat();
	void error();

private:
	vector<tweet> tweetList;
	//list<tweet, allocator<tweet>> tweetList;
	string username;
	vector<tweet>::iterator curr;
	//list<tweet, allocator<tweet>>::iterator curr;
	void makeDate(tweet&);
	void setUser(string);
	void makeDialogue(tweet&);

};
