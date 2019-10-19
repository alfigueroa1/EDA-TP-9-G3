#pragma once
#include <iostream>
#include <list>


using namespace std;

class model {
public:
	//Getters y Setters
	string getTweetDate();
	string getTweetContent();
	string getUser();
	void goPrevious();
	void goNext();
	void repeat();
	void error();

private:
	list<tweet, allocator<tweet>> tweetList;
	string username;
	void setDate(string);
	void setUser(string);
	void makeDialogue();

};

struct tweet {

	string date;
	string content;

};