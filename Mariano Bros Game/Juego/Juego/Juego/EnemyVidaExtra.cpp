#include "EnemyVidaExtra.h"

EnemyVidaExtra::EnemyVidaExtra(float x, float y, Game* game)
	: Actor("res/crab_idle.png", x, y, 36, 40, game) {
	onAir = false;

	state = game->stateMoving;

	vx = 1;

	vxIntelligence = -1;
	vx = vxIntelligence;

	orientation = game->orientationRight;

	aIdle = new Animation("res/crab_idle.png", width, height,
		39, 34, 6, 1, true, game);
	aRunningRight = new Animation("res/crab_mov_derecha.png", width, height,
		115, 34, 6, 3, true, game);
	aRunningLeft = new Animation("res/crab_mov_izquierda.png", width, height,
		115, 34, 6, 3, true, game);
	aDying = new Animation("res/crab_muriendo.png", width, height,
		77, 34, 6, 2, false, game);
	aTumbado = new Animation("res/crab_tumbado.png", width, height,
		77, 34, 30, 2, false, game);
	animation = aIdle;
}

void EnemyVidaExtra::update() {
	bool endAnimation = animation->update();

	x = x + vx;

	if (timeTumbado > 0 && state == game->stateTumbado) {
		vx = 0;
		timeTumbado--;
	}

	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateTumbado) {
			state = game->stateMoving;
			timeTumbado = 250;
			aTumbado->currentFrame = 0;
			aDying->currentFrame = 0;
			vx = 1;
		}
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}

	if (collisionDown == true) {
		onAir = false;
	}
	else {
		onAir = true;
	}

	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	if (state == game->stateMoving) {
		animation = aRunningRight;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}
	// Selección de animación
	if (vx != 0) {
		if (orientation == game->orientationRight) {
			animation = aRunningRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aRunningLeft;
		}
	}

	if (vx == 0) {
		if (orientation == game->orientationRight) {
			animation = aIdle;
		}
		if (orientation == game->orientationLeft) {
			animation = aIdle;
		}
	}

	if (x - width / 2 <= 0 || x + width / 2 >= WIDTH) {
		vx = vx * -1;
	}

	if (state == game->stateDying) {
		animation = aDying;
	}

	if (state == game->stateTumbado) {
		animation = aTumbado;
	}

	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
	}
	else {
		vx = 0;
	}

}

void EnemyVidaExtra::draw() {
	animation->draw(x, y);
}

void EnemyVidaExtra::die() {
	if (state != game->stateDying && state == game->stateTumbado) {
		state = game->stateDying;
	}
}

void EnemyVidaExtra::tumbar() {
	if (state != game->stateTumbado) {
		state = game->stateTumbado;
	}
}