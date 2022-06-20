#pragma once

#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Layer.h"

// Valores generales
#define WIDTH 536
#define HEIGHT 700

#include "Layer.h"
class Layer;

class Game
{
public:
	Game();
	void loop();
	SDL_Window* window; 
	SDL_Renderer* renderer; 
	TTF_Font* font;
	bool loopActive; 

	bool scaledToMax = false;
	float scaleLower = 1;

	Layer* layer;
	Layer* menuLayer;
	Layer* gameLayer;
	Layer* finalLayer;

	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateTumbado = 5;
	int const orientationRight = 1;
	int const orientationLeft = 2;

	int input;
	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

	int currentLevel = 1;
	int finalLevel = 3;

	string points;
};
