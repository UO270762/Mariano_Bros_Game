#include "FinalLayer.h"

FinalLayer::FinalLayer(Game* game, string puntos)
	: Layer(game, "res/sounds/victoria.mp3") {
	init();
	res = puntos;
}

void FinalLayer::init() {
	background = new Background("res/fondo3.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	button = new Actor("res/boton_jugar.png", WIDTH * 0.5, HEIGHT * 0.8, 232, 72, game);
	resultados = new Text("hola", WIDTH * 0.27, HEIGHT * 0.50, game);
}

void FinalLayer::draw() {
	background->draw();
	button->draw();
	resultados->content = game->points;
	resultados->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

void FinalLayer::processControls() {
	// obtener controles
	SDL_Event event;
  	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}

		//procesar controles, solo tiene uno
		if (controlContinue) {
			// Cambia la capa
			game->layer = game->menuLayer;
			game->layer->play();
			controlContinue = false;
		}
	}
}

void FinalLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_SPACE:
			controlContinue = true;
			break;
		}
	}
}

void FinalLayer::mouseToControls(SDL_Event event) {
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->containsPoint(motionX, motionY)) {
			controlContinue = true;
		}
	}
}
