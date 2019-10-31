#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include "model.h"
#include "viewer.h"

#define MAX_USER 16
#define MAX_TWEETS 5

class controller : public observer{
public:
	//Constructor y destructor
	controller(model*, viewer*);
	~controller();

	void cycle();
	virtual void update(void*);
	void askForTweets();
	void drawDownloading();
	void drawOptions();
	void drawError();
	void noTweets();
	void noUser();
	void privateUser();
	void show();
	bool running;

private:
	model* m;
	viewer* v;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	char userBuffer[MAX_USER];
	char maxBuffer[MAX_TWEETS];
	float speed;

};