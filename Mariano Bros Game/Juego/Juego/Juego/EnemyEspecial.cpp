#include "EnemyEspecial.h"

EnemyEspecial::EnemyEspecial(float x, float y, Game* game)
	: Actor("res/hielo_idle.png", x, y, 36, 40, game) {
	onAir = false;

	state = game->stateMoving;

	vx = 1;

	vxIntelligence = -1;
	vx = vxIntelligence;

	orientation = game->orientationRight;

	aIdle = new Animation("res/hielo_idle.png", width, height,
		27, 34, 6, 1, true, game);
	aRunningRight = new Animation("res/hielo_mov_derecha.png", width, height,
		91, 34, 3, 3, true, game);
	aRunningLeft = new Animation("res/hielo_mov_izquierda.png", width, height,
		91, 34, 3, 3, true, game);
	aDying = new Animation("res/hielo_muriendo.png", width, height,
		107, 34, 3, 3, false, game);
	animation = aIdle;
}

void EnemyEspecial::update() {
	bool endAnimation = animation->update();

	x = x + vx;

	if (endAnimation) {
		// Estaba muriendo
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
		orientation = game->orientationRight;
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

void EnemyEspecial::draw() {
	animation->draw(x, y);
}

void EnemyEspecial::die() {
	if (state != game->stateDying && state != game->stateDead) {
		state = game->stateDying;
	}
}
