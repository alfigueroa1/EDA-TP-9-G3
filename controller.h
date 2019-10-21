#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include "model.h"

class controller {
public:
	//Constructor y destructor
	controller(model&);
	~controller();

	void cycle();
	//void update(model);
	void askForTweets();
	void drawOptions(model);
	void show();


private:
	model m;
	ALLEGRO_TIMER* timer;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	float speed;
	bool ask;

};