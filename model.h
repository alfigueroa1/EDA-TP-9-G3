#pragma once
#include <iostream>
#include <list>
#include <vector>
#include "observer.h"
#include "twitterClass.h"


using namespace std;

struct tweet {
	string date;
	string content;
};

typedef enum {
	INIT, ERR, DOWNLOADING, FINISHED_DOWNLOAD, FINISH_EARLY, REPEAT, NEXT, PREV
} stateType;

class model {
public:
	//Constructor y destructor
	model();
	//~model();
	void attach(observer& o) { observers.push_back(&o); }
	void detach(vector<observer*>::iterator pos) { observers.erase(pos); };
	//Getters y Setters
	string getTweetDate();
	string getTweetContent();
	string getUser();
	stateType getState() { return state; };
	bool isDownloading() { return downloading; }
	void setUser(string);
	void setMaxTweets(int);
	int getMoreTweets();

	//Manejo de la lista de Tweets
	tweet getTweet();				//Devuelve el tweet actual
	bool goPrevious();				//Va el tweet anterior, devuelve true si esta en el primer tweet
	bool goNext();					//Va el siguiente tweet, devuelve true si esta en el ultimo tweet
	void stop();
	void error();

private:
	TwitterAPI twitter;
	vector<observer*> observers;
	vector<tweet> tweetList;
	stateType state;
	string username;
	int maxTweets;
	int currentTransfers;
	bool downloading;
	vector<tweet>::iterator curr;
	void parseTweets();
	void makeDate(tweet&);
	void makeDialogue(tweet&);

	void notifyAllObservers();

};

