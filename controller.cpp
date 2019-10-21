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
#define MAX_USER 15

controller::controller(model mainM) {

	// Setup Allegro
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_primitives_addon();
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	display = al_create_display(1280, 860);
	al_set_window_title(display, "EDA-TP8-G2");
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	timer = al_create_timer(1.0/FPS);

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
	m = mainM;
	ask = true;

	// Start Timer
	al_start_timer(timer);
}

controller::~controller() {

	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(queue);
	al_destroy_display(display);
}


void controller::cycle() {
	bool running = true;
	model m;
	while (running) {
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

		//Pide Input del Ususario
		askForTweets();
		
		//Dibuja UI
		drawOptions(m);

		// Rendering
		show();

	}
}

void controller::drawOptions(model m) {
	ImGui::Begin("Options");
	ImGui::Text("User: %s", m.getUser().c_str());

	ImGui::Text("Date %s", m.getTweetDate());
	ImGui::NewLine();
	ImGui::Text("%s", m.getTweetContent());
	ImGui::NewLine();

	if (ImGui::Button("Previous")) {
		m.goPrevious();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reshow")) {
		m.reshow();
	}
	ImGui::SameLine();
	if (ImGui::Button("Next")) {
		m.goNext();
	}

	ImGui::NewLine();
	ImGui::SliderFloat("LCD Speed", &speed, 0, 100);



}

void controller::askForTweets() {
	char userBuffer[MAX_USER];
	char maxBuffer[50];
	ImGui::Begin("Display tweets on LCD!", &ask);

	ImGui::Text("Por favor especifique la cuenta de la cual quiere leer los tweets");
	ImGui::InputText("Username", userBuffer, MAX_USER);
	ImGui::NewLine();

	ImGui::Text("Por favor especifique la cantidad de twets que quiere cargar");
	ImGui::InputText("Amount of tweets", maxBuffer, 50);
	ImGui::NewLine();

	if (ImGui::Button("Submit")) {
		m.setUser(userBuffer);
		m.setMaxTweets(atoi(maxBuffer));
		ask = false;
		if (ImGui::Button("STOP"))
			m.stop();
	}

	ImGui::End();
}

void controller::show() {
	ImVec4 clear_color = ImVec4(0.25f, 0.55f, 0.90f, 1.00f);
	ImGui::Render();
	al_clear_to_color(al_map_rgba_f(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
	al_flip_display();
}