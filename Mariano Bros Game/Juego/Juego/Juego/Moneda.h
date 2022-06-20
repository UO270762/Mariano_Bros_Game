#pragma once
#include "Actor.h"
#include "Animation.h" 
class Moneda :
    public Actor
{
public:
    Moneda(float x, float y, Game* game);
    Animation* animation;
    Animation* aMovimiento;
    void draw() override;
    void update();
    float vxIntelligence;
};

