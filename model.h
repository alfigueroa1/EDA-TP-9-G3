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
	tweet getTweet();				//Devuelve el tweet actual
	bool goPrevious();				//Va el tweet anterior, devuelve true si esta en el primer tweet
	bool goNext();					//Va el siguiente tweet, devuelve true si esta en el ultimo tweet
	//void repeat();
	void error();

private:
	vector<tweet> tweetList;
	string username;
	vector<tweet>::iterator curr;
	void parseTweets();
	void makeDate(tweet&);
	void makeDialogue(tweet&);

};
