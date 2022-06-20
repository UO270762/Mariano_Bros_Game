#pragma once
#include "Actor.h"
class Tuberia :
    public Actor
{
public:
    Tuberia(float x, float y, Game* game);
    void draw() override;
    void update();
};

