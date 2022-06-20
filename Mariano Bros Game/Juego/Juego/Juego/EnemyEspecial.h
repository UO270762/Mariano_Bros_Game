#pragma once
#include "Actor.h"
#include "Animation.h" 
#include "Audio.h"
class EnemyEspecial :
    public Actor
{
public:
    EnemyEspecial(float x, float y, Game* game);
    void draw() override;
    void update();
    void die();
    float vxIntelligence;
    Animation* aIdle;
    Animation* aRunningRight;
    Animation* aRunningLeft;
    Animation* aDying;
    Animation* animation;
    int orientation;
    int state;
    bool onAir;
};

