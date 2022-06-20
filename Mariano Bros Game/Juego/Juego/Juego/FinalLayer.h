#pragma once
#include "Layer.h"
#include "Actor.h"
#include "Background.h"
#include "Audio.h" 
#include "Text.h" 
class FinalLayer :
    public Layer
{
public:
	FinalLayer(Game* game, string puntos);
	void init() override;
	void draw() override;
	Background* background;
	Actor* button;
	Text* resultados;
	void processControls() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);
	bool controlContinue;
	string res;
};

