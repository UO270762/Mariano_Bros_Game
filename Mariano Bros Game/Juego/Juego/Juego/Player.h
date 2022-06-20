#pragma once
#include "Actor.h"
#include "Animation.h" 
class Player :
    public Actor
{
public:
    Player(float x, float y, Game* game);
    void draw() override;
    void update();
    void moveX(float axis);
    void moveY(float axis);
    void jump();
    void loseLife();
    void congelar();
    void sumaVida();
    bool onAir;
    Animation* aIdleRight;
    Animation* aIdleLeft;
    Animation* aRunningRight;
    Animation* aRunningLeft;
    Animation* aJumpingRight;
    Animation* aJumpingLeft;
    Animation* aCongelado;
    Animation* animation;
    int orientation;
    int lifes = 3;
    int invulnerableTime = 0;
    int tiempoCongelado = 200;
    bool congelado;
    bool saleDePantalla = false;
};



