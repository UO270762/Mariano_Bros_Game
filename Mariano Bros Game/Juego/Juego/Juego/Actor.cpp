#include "Actor.h"

Actor::Actor(string filename, float x, float y, int width, int height, Game* game) {
	this->game = game;
	SDL_Surface* surface = IMG_Load(filename.c_str());
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	this->x = x;
	this->y = y;
	// lo que mide el fichero
	this->fileWidth = width;
	this->fileHeight = height;
	// lo que va a medir en el juego
	this->width = width;
	this->height = height;
}

Actor::~Actor() {
	SDL_DestroyTexture(texture);
}

void Actor::draw() {
	// Recorte en el fichero de la imagen
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = fileWidth; // texture.width;
	source.h = fileHeight; // texture.height;

	// Donde se va a pegar en el renderizador
	SDL_Rect destination;
	destination.x = x - width / 2;
	destination.y = y - height / 2;
	destination.w = width;
	destination.h = height;
	// Modificar para que la referencia sea el punto central

	SDL_RenderCopyEx(game->renderer,
		texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
}

bool Actor::isOverlap(Actor* actor) {
	bool overlap = false;
	if (actor->x - actor->width / 2 <= x + width / 2
		&& actor->x + actor->width / 2 >= x - width / 2
		&& actor->y + actor->height / 2 >= y - height / 2
		&& actor->y - actor->height / 2 <= y + height / 2) {

		overlap = true;
	}
	return overlap;
}

bool Actor::isUpPlaforma(Actor* actor) {
	bool overlap = false;
	if (actor->x + actor->width / 2 >= x - width / 2
		&& actor->x - actor->width / 2 <= x + width / 2
		&& actor->y - actor->height / 2 - 32 <= y + height / 2 
		&& actor->y - actor->height / 2 - 32 >= y + height / 2
		&& actor->collisionUp) {
		overlap = true;
	}
	return overlap;
}

bool Actor::isActivandoPow(Actor* pow)
{
	bool overlap = false;

	if (pow->x -pow->width/2 <= x + width /2
		&& pow->x + pow->width / 2 >= x - width / 2
		&& pow->y + pow->height / 2 >= y - height / 2
		&& collisionUp) {
		overlap = true;
	}

	return overlap;
}

bool Actor::containsPoint(int pointX, int pointY) {
	if (pointY >= y - height / 2 &&
		pointY <= y + height / 2 &&
		pointX <= x + width / 2 &&
		pointX >= x - width / 2) {
		return true;
	}
	return false;
}

bool Actor::jugadorSeSalePantalla()
{
	if (x - width / 2  <= 0 || x + width / 2 >= WIDTH) {
		return true;
	}
	else {
		return false;
	}
}

