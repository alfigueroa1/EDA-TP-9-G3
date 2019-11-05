#include <iostream>
#include <stdint.h>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "imgui.h"
#include "imgui_impl_allegro5.h"
#include "controller.h"

#define FPS	50.0

controller::controller(model* model, viewer* viewer) {

	// Setup Allegro
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_primitives_addon();
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	display = al_create_display(1366, 720);
	al_set_window_title(display, "EDA-TP9-G3");
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer bindings
	ImGui_ImplAllegro5_Init(display);

	// Initialize Variables
	m = model;
	v = viewer;
	userBuffer[0] = '@';

}

controller::~controller() {

	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(queue);
	al_destroy_display(display);
}

void controller::update(void* mod) {
	m = (model*)mod;
}

void controller::cycle() {
	ALLEGRO_EVENT ev;
	while (al_get_next_event(queue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			running = false;
		if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			ImGui_ImplAllegro5_InvalidateDeviceObjects();
			al_acknowledge_resize(display);
			ImGui_ImplAllegro5_CreateDeviceObjects();
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();

	switch (m->getState()) {
	case INIT:						//Pide Input del Ususario
		askForTweets();
		break;
	case DOWNLOADING:				//Ventana de que está descargando
		m->getMoreTweets();
		drawDownloading();
		break;
	case ERR:						//Muestra que hubo un error
		drawError();
		break;
	case NO_TWEETS:
		noTweets();
		askForTweets();
		break;
	case NO_USER:
		noUser();
		askForTweets();
		break;
	case PRIVATE_USER:
		privateUser();
		askForTweets();
		break;
	case NEXT: case PREV: case REPEAT:
		drawOptions();
		m->setState(FINISHED_DOWNLOAD);
		break;
	default:						//Dibuja UI
		drawOptions();
		if (v->getTweetState(m->getTweet()))
			m->goNext();
		break;
	}

	// Rendering
	show();
}

void controller::drawOptions() {
	ImGui::Begin("Options");
	ImGui::Text("User: %s", m->getUser().c_str());		ImGui::SameLine();
	ImGui::Text("Last %d tweets", m->getMaxTweets());	ImGui::SameLine();
	if (ImGui::Button("Change"))
		m->setState(INIT);

	ImGui::Text("Date %s", m->getTweetDate().c_str());
	ImGui::NewLine();
	ImGui::Text("%s", m->getTweetContent().c_str());
	ImGui::NewLine();

	if (ImGui::Button("Previous")) {
		m->goPrevious();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reshow")) {
		v->restartTweet();								//MEJOR CAMBIAR EL ESTADO DEL MODELO A RESTART
	}
	ImGui::SameLine();
	if (ImGui::Button("Next"))
		m->goNext();
	if (m->getState() == END)
		ImGui::Text("END");

	ImGui::NewLine();
	ImGui::SliderFloat("LCD Speed", &speed, 0, 100);
	v->changeSpeed((int)speed);

	if (ImGui::Button("More Tweets")) {
		if (m->getMoreTweets())
			m->setState(ERR);
	}

	ImGui::End();
}

void controller::drawDownloading() {
	ImGui::Begin("Downloading Tweets");
	ImGui::NewLine();
	if (ImGui::Button("STOP"))
		m->stop();
	ImGui::End();
}

void controller::drawError() {
	ImGui::Begin("Error");
	ImGui::Text("Error downloading tweets");
	if (ImGui::Button("Retry"))
		m->setState(INIT);
	ImGui::End();
}

void controller::askForTweets() {

	ImGui::Begin("Display tweets on LCD!");

	ImGui::Text("Por favor especifique la cuenta de la cual quiere leer los tweets");
	ImGui::InputText("Username", userBuffer, MAX_USER);
	ImGui::NewLine();

	ImGui::Text("Por favor especifique la cantidad de tweets que quiere cargar");
	ImGui::InputText("Amount of tweets", maxBuffer, 5);
	ImGui::NewLine();

	if (ImGui::Button("Submit")) {
		m->setUser(userBuffer);
		m->setMaxTweets(atoi(maxBuffer));
		if (m->getMoreTweets())
			m->setState(ERR);
	}

	ImGui::End();
}

void controller::noTweets() {
	ImGui::Begin("Display tweets on lcd!");
	ImGui::Text("La cuenta ingresada no tiene tweets");
	ImGui::End();
}

void controller::noUser() {
	ImGui::Begin("Display tweets on lcd!");
	ImGui::Text("El usuario no existe");
	ImGui::End();
}

void controller::privateUser() {
	ImGui::Begin("Display tweets on lcd!");
	ImGui::Text("El usuario es privado");
	ImGui::End();
}

void controller::show() {
	ImVec4 clear_color = ImVec4(0.25f, 0.55f, 0.90f, 1.00f);
	ImGui::Render();
	al_clear_to_color(al_map_rgba_f(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}