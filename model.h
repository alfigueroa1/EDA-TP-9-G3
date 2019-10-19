#pragma once
#include <iostream>
#include <vector>


using namespace std;

struct tweet {
	string date;
	string content;
};

class model {
public:
	//Getters y Setters
	string getTweetDate();
	string getTweetContent();
	string getUser();

	//Manejo de la lista de Tweets
	void goPrevious();
	void goNext();
	//void repeat();
	void error();

private:
	vector<tweet> tweetList;
	string username;
	vector<tweet>::iterator curr;
	void setDate(string);
	void setUser(string);
	void makeDialogue();

};
