#pragma once

#include "Game.h"
#include "Audio.h" 
class Game;

class Layer
{

public:
	Layer(Game* game, string fileName);
	virtual void init() {};
	virtual void processControls() {};
	virtual void update() {};
	virtual void draw() {};
	virtual void play() { audioBackground->play(); };
	Game* game;
	Audio* audioBackground;
};


