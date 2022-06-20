#include "Layer.h"

Layer::Layer(Game* game, string fileName) {
	this->game = game;
	this->audioBackground = new Audio(fileName, true);
}

