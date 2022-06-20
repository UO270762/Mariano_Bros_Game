#pragma once
#include "Actor.h"
#include "Animation.h" 
class EnemyVidaExtra :
    public Actor
{
public:
    EnemyVidaExtra(float x, float y, Game* game);
    void draw() override;
    void update();
    void die();
    void tumbar();
    float vxIntelligence;
    int timeTumbado = 200;
    Animation* aIdle;
    Animation* aRunningRight;
    Animation* aRunningLeft;
    Animation* animation;
    Animation* aDying;
    Animation* aTumbado;
    int orientation;
    int state;
    bool onAir;
};

