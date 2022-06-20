#pragma once
#include "Actor.h"
#include "Audio.h"
#include "Animation.h" 
class EnemySimple :
    public Actor
{
public:
	EnemySimple(float x, float y, Game* game);
    void draw() override;
	void update();
    void die();
    float vxIntelligence;
    Animation* aIdleRight;
    Animation* aIdleLeft;
    Animation* aRunningRight;
    Animation* aRunningLeft;
    Animation* animation;
    Animation* aDying;
    int orientation;
    int state;
    bool onAir;
};

