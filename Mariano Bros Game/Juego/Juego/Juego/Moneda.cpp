#include "Moneda.h"
Moneda::Moneda(float x, float y, Game* game)
    : Actor("res/moneda_mov.png", x, y, 20, 30, game) {
    aMovimiento = new Animation("res/moneda_mov.png", width, height, 71, 26, 4, 3, true, game);
    animation = aMovimiento;
	vx = 2;

	vxIntelligence = -1;
	vx = vxIntelligence;
}

void Moneda::update() {
    bool end = animation->update();

    x = x + vx;

    if (x - width / 2 <= 0 || x + width / 2 >= WIDTH) {
        vx = vx * -1;
    }
}

void Moneda::draw() {
    animation->draw(x, y);
}