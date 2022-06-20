#include "PowBlock.h"

PowBlock::PowBlock(float x, float y, Game* game)
	: Actor("res/pow.png", x, y, 36, 40, game) {

	animation = new Animation("res/pow.png", width, height, 40, 40, 1, 1, true, game);
	aActivado = new Animation("res/pow_mov.png", width, height, 120, 40, 3, 3, false, game);
}

void PowBlock::update() {
	bool endAnimation = animation->update();
	if (endAnimation) {
		finActivado = true;
	}
}

void PowBlock::draw() {
	animation->draw(x, y);
}

void PowBlock::activar() {
	if (animation != aActivado) {
		animation = aActivado;
	}
}
