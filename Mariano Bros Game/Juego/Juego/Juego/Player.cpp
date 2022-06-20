#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/mario.png", x, y, 34, 54, game) {
	onAir = false;
	congelado = false;
	orientation = game->orientationRight;

	aIdleRight = new Animation("res/mario_idle_derecha.png", width, height,
		34, 54, 6, 1, true, game);
	aIdleLeft = new Animation("res/mario_idle_izquierda.png", width, height,
		34, 54, 6, 1, true, game);
	aRunningRight = new Animation("res/mario_mov_derecha.png", width, height,
		104, 56, 6, 3, true, game);
	aRunningLeft = new Animation("res/mario_mov_izquierda.png", width, height,
		104, 56, 6, 3, true, game);
	aJumpingRight = new Animation("res/mario_saltando_der.png", width, height,
		34, 54, 6, 1, true, game);
	aJumpingLeft = new Animation("res/mario_saltando_izq.png", width, height,
		34, 54, 6, 1, true, game);

	animation = aIdleRight;
}

void Player::jump() {
	if (!onAir) {
		if (!congelado) {
			vy = -18;
		}
		else {
			vy = -8;
		}
		onAir = true;
	}
}

void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	if (tiempoCongelado > 0 && congelado) {
		tiempoCongelado--;
	} 
	bool endAnimation = animation->update();

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

	if (onAir) {
		if (orientation == game->orientationRight) {
			animation = aJumpingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aJumpingLeft;
		}
	}

	if (tiempoCongelado <= 0 && congelado) {
		congelado = false;
		tiempoCongelado = 200;
	}
}

void Player::moveX(float axis) {
	if (jugadorSeSalePantalla()) {
		if (orientation == game->orientationLeft) {
			x = x + 1;
		}
		else {
			x = x - 1;
		}
		
		vx = 0;
	}
	else {
		if (congelado) {
			vx = axis * 2;
		}
		else {
			vx = axis * 4;
		}
	}
}

void Player::moveY(float axis) {
	vy = axis * 4;
}

void Player::draw() {
	if (invulnerableTime == 0) {
		animation->draw(x, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x, y);
		}
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 50;
		}
	}
}

void Player::congelar() {
	if (!congelado) {
		congelado = true;
		vx = 4;
	}
}

void Player::sumaVida() {
	lifes++;
}


