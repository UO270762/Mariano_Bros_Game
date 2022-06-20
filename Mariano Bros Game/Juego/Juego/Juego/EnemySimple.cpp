#include "EnemySimple.h"

EnemySimple::EnemySimple(float x, float y, Game* game)
	: Actor("res/koopa_idle_izquierda.png", x, y, 36, 40, game) {
	onAir = false;

	state = game->stateMoving;

	vx = 1;

	vxIntelligence = -1;
	vx = vxIntelligence;
	
	orientation = game->orientationRight;

	aIdleRight = new Animation("res/koopa_idle_derecha.png", width, height,
		36, 34, 6, 1, true, game);
	aIdleLeft = new Animation("res/koopa_idle_izquierda.png", width, height,
		36, 34, 6, 1, true, game);
	aRunningRight = new Animation("res/koopa_mov_derecha.png", width, height,
		108, 34, 6, 3, true, game);
	aRunningLeft = new Animation("res/koopa_mov_izquierda.png", width, height,
		108, 34, 6, 3, true, game);
	aDying = new Animation("res/koopa_muriendo.png", width, height,
		108, 34, 6, 3, false, game);
	animation = aIdleRight;
}

void EnemySimple::update() {
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
			animation = aIdleRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aIdleLeft;
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

void EnemySimple::draw() {
	animation->draw(x, y);
}

void EnemySimple::die() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}
