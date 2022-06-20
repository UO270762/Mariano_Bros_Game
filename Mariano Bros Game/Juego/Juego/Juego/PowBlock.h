#pragma once
#include "Actor.h"
#include "Animation.h" 

class PowBlock :
    public Actor
{
public:
    PowBlock(float x, float y, Game* game);
    void draw() override;
    void update();
    void activar();
    bool activado = false;
    bool finActivado = false;
    Animation* aActivado;
    Animation* animation;
};

