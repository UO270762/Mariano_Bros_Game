#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	bool collisionDown;
	bool collisionUp;
	virtual void draw();
	bool isOverlap(Actor* actor);
	bool isUpPlaforma(Actor* actor);
	bool isActivandoPow(Actor* pow);
	bool containsPoint(int pointX, int pointY); // contiene punto
	bool jugadorSeSalePantalla();

	SDL_Texture* texture;
	int x;
	int y;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	Game* game; // referencia al juego
};

