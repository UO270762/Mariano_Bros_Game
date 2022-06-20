#include "GameLayer.h"
#include <ctime>
#include "FinalLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game, "res/sounds/theme1.mp3") {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	space = new Space(1);
	tiles.clear();

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);

	enemieSimple.clear(); // Vaciar por si reiniciamos el juego
	enemieEspecial.clear();
	enemieVidaExtra.clear();
	monedas.clear();

	lifeIcon = new Actor("res/vidas_icono.png",
		WIDTH * 0.07, HEIGHT * 0.958, 38, 38, game);

	loadMap("res/levels/" + to_string(game->currentLevel) + ".txt");

	lifePoints = new Text("hola", WIDTH * 0.17, HEIGHT * 0.96, game);
	lifePoints->content = "t0" + to_string(player->lifes);

	
	tuberia1 = new Actor("res/tuberia_1.png", WIDTH * 0.073, HEIGHT * 0.085, 80, 63, game);
	tuberia2 = new Actor("res/tuberia_2.png", WIDTH * 0.926, HEIGHT * 0.085, 80, 63, game);
	tuberia3 = new Actor("res/tuberia_3.png", WIDTH * 0.955, HEIGHT * 0.83, 50, 50, game);
	tuberia4 = new Actor("res/tuberia_4.png", WIDTH * 0.048, HEIGHT * 0.83, 50, 50, game);
	
	tpTimeTuberia1 = 30;
	tpTimeTuberia2 = 30;

	newActorTime = 500;

	audioCongelar = new Audio("res/sounds/freeze.wav", false);
	audioMoneda = new Audio("res/sounds/coin.wav", false);
	audioMuerteEnemigo = new Audio("res/sounds/enemy_die.wav", false);
	audioPerderVida = new Audio("res/sounds/hit.wav", false);
	audioPow = new Audio("res/sounds/pow.wav", false);

	audioGameOver = new Audio("res/sounds/gameover.wav", false);
	audioSiguienteNivel = new Audio("res/sounds/nextLevel.wav", false);

	patronGeneracionActores();
	
	tStart = time(0);
	reloj = new Text("hola", WIDTH * 0.9, HEIGHT * 0.96, game);
	iconoReloj = new Actor("res/reloj.png",
		WIDTH * 0.78, HEIGHT * 0.958, 38, 38, game);
	reloj->content = "00:00";
	
	nMonedas = 0;
}

void GameLayer::processControls() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		
	}
	else if (controlMoveY < 0) {
		player->jump();
	}
	else {
		
	}
	if (continuar) {
		pausa = false;
		continuar = false;
		looser = false;
		winner = false;
	}

}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		if (pausa) {
			continuar = true;
		}
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		}

	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		}
	}

}

void GameLayer::update() {
	if (pausa) {
		return;
	}

	space->update();
	player->update();
	pow->update();

	lifePoints->content = "t0" + to_string(player->lifes);

	getTiempo();

	win();

	newActorTime--;

	if (newActorTime <= 0 && !listaActores.empty()) {
		generacionActores();
		newActorTime = 150;
	}

	for (auto const& enemy : enemieSimple) {
		enemy->update();
	}
	for (auto const& enemy : enemieEspecial) {
		enemy->update();
	}
	for (auto const& enemy : enemieVidaExtra) {
		enemy->update();
	}
	for (auto const& moneda : monedas) {
		moneda->update();
	}

	list<EnemySimple*> deleteEnemieSimple;
	list<EnemyEspecial*> deleteEnemieEspecial;
	list<EnemyVidaExtra*> deleteEnemieVidaExtra;
	list<Moneda*> deleteMonedas;

	// Colisiones negativas jugador-Enemigos Simple y VidaExtra
	for (auto const& enemy : enemieSimple) {
		if (player->isOverlap(enemy)) {
			audioPerderVida->play();
			player->loseLife();
		}
	}

	for (auto const& enemy : enemieVidaExtra) {
		if (player->isOverlap(enemy) && enemy->state != game->stateTumbado 
			&& enemy->state != game->stateDying 
			&& enemy->state != game->stateDead) {
			player->loseLife();
		}
	}


	//Colision congelar
	for (auto const& enemy : enemieEspecial) {
		if (player->isOverlap(enemy)) {
			audioCongelar->play();
			player->congelar();
			enemy->die();
		}
	}

	//Colision moneda
	for (auto const& moneda : monedas) {
		if (player->isOverlap(moneda)) {
			audioMoneda->play();
			bool pInList = std::find(deleteMonedas.begin(),
				deleteMonedas.end(),
				 moneda) != deleteMonedas.end();

			if (!pInList) {
				deleteMonedas.push_back(moneda);
			}
			nMonedas++;
		}
	}

	//Colision Pow 
	
	if (pow != NULL) {
		if (player->isActivandoPow(pow)) {
			audioPow->play();
			pow->activar();
			golpearEnemigos();
		}
	}
	

	//Colisiones positivas jugador-Enemigo Simple y Vida extra
	for (auto const& enemy : enemieSimple) {
		bool overlap = false;
		if (enemy->isUpPlaforma(player)) {
			audioMuerteEnemigo->play();
			enemy->die();
		}
	}
	//Tumbar: vidaExtra
	for (auto const& enemy : enemieVidaExtra) {
		bool overlap = false;
		if (enemy->isUpPlaforma(player)) {
			enemy->tumbar();
		}
	} 
	//Matar: vidaExtra
	for (auto const& enemy : enemieVidaExtra) {
		if (player->isOverlap(enemy) && enemy->state == game->stateTumbado) {
			audioMuerteEnemigo->play();
			enemy->die();
			player->sumaVida();
		}
	}
	for (auto const& enemy : enemieSimple) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemieSimple.begin(),
				deleteEnemieSimple.end(),
				enemy) != deleteEnemieSimple.end();

			if (!eInList) {
				deleteEnemieSimple.push_back(enemy);
			}
		}
	}
	for (auto const& enemy : enemieEspecial) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemieEspecial.begin(),
				deleteEnemieEspecial.end(),
				enemy) != deleteEnemieEspecial.end();

			if (!eInList) {
				deleteEnemieEspecial.push_back(enemy);
			}
		}
	}

	for (auto const& enemy : enemieVidaExtra) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemieVidaExtra.begin(),
				deleteEnemieVidaExtra.end(),
				enemy) != deleteEnemieVidaExtra.end();

			if (!eInList) {
				deleteEnemieVidaExtra.push_back(enemy);
			}
		}
	}

	//Colisiones transladar tuberias
	
	//TP desde tuberia4 a tuberia1
	for (auto const& enemy : enemieSimple) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia4) && !enemy->onAir) {
			if (!yaEstaEnCola1(enemy)) {
				colaTuberia1.push(enemy);
			}
		}
	}

	for (auto const& enemy : enemieEspecial) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia4) && !enemy->onAir) {
			if (!yaEstaEnCola1(enemy)) {
				colaTuberia1.push(enemy);
			}
		}
	}

	for (auto const& enemy : enemieVidaExtra) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia4) && !enemy->onAir) {
			if (!yaEstaEnCola1(enemy)) {
				colaTuberia1.push(enemy);
			}
		}
	}

	for (auto const& moneda : monedas) {
		bool overlap = false;
		if (moneda->isOverlap(tuberia4)) {
			if (!yaEstaEnCola1(moneda)) {
				colaTuberia1.push(moneda);
			}
		}
	}

	tpTimeTuberia1--;
	tpTuberia1();

	//TP desde tuberia3 a tuberia2
	for (auto const& enemy : enemieSimple) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia3) && !enemy->onAir) {
			if (!yaEstaEnCola2(enemy)) {
				colaTuberia2.push(enemy);
			}
		}
	}

	for (auto const& enemy : enemieEspecial) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia3) && !enemy->onAir) {
			if (!yaEstaEnCola2(enemy)) {
				colaTuberia2.push(enemy);
			}
		}
	}

	for (auto const& enemy : enemieVidaExtra) {
		bool overlap = false;
		if (enemy->isOverlap(tuberia3) && !enemy->onAir) {
			if (!yaEstaEnCola2(enemy)) {
				colaTuberia2.push(enemy);
			}
		}
	}

	for (auto const& moneda : monedas) {
		bool overlap = false;
		if (moneda->isOverlap(tuberia3)) {
			if (!yaEstaEnCola2(moneda)) {
				colaTuberia2.push(moneda);
			}
		}
	}

	tpTimeTuberia2--;
	tpTuberia2();

	for (auto const& delEnemy : deleteEnemieSimple) {
		enemieSimple.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	for (auto const& delEnemy : deleteEnemieEspecial) {
		enemieEspecial.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	for (auto const& delEnemy : deleteEnemieVidaExtra) {
		enemieVidaExtra.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	for (auto const& delMoneda : deleteMonedas) {
		monedas.remove(delMoneda);
		space->removeDynamicActor(delMoneda);
	}

	if (pow->finActivado) {
		space->removeStaticActor(pow);
	}

	deleteEnemieSimple.clear();
	deleteEnemieEspecial.clear();
	deleteEnemieVidaExtra.clear();
	deleteMonedas.clear();

	lose();

	if (winner) {
		audioSiguienteNivel->play();
		mensajeSiguienteNivel->draw();
	} 
	if (looser) {
		audioGameOver->play();
		mensajeGameOver->draw();
	}
	cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	background->draw();
	
	for (auto const& tile : tiles) {
		tile->draw();
	}
	tuberia1->draw();
	tuberia2->draw();
	tuberia3->draw();
	tuberia4->draw();
	player->draw();
	pow->draw();
	iconoReloj->draw();
	for (auto const& enemy : enemieSimple) {
		enemy->draw();
	}
	for (auto const& enemy : enemieEspecial) {
		enemy->draw();
	}
	for (auto const& enemy : enemieVidaExtra) {
		enemy->draw();
	}
	for (auto const& moneda : monedas) {
		moneda->draw();
	}
	lifeIcon->draw();
	lifePoints->draw();
	reloj->draw();
	if (pausa && winner) {
		mensajeSiguienteNivel->draw();
	}
	else if (pausa && looser) {
		audioGameOver->play();
		mensajeGameOver->draw();
		
	}
	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; 
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; 
				cout << character;
				float x = 40 / 2 + j * 40; 
				float y = 32 + i * 32; 
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
		switch (character) {
			case 'K': {
				EnemySimple* enemy = new EnemySimple(x, y, game);
				// modificación para empezar a contar desde el suelo.
				enemy->y = enemy->y - enemy->height / 2;
				enemieSimple.push_back(enemy);
				space->addDynamicActor(enemy);
				break;
			}
			case 'H': {
				EnemyEspecial* enemy = new EnemyEspecial(x, y, game);
				// modificación para empezar a contar desde el suelo.
				enemy->y = enemy->y - enemy->height / 2;
				enemieEspecial.push_back(enemy);
				space->addDynamicActor(enemy);
				break;
			}
			case 'C': {
				EnemyVidaExtra* enemy = new EnemyVidaExtra(x, y, game);
				// modificación para empezar a contar desde el suelo.
				enemy->y = enemy->y - enemy->height / 2;
				enemieVidaExtra.push_back(enemy);
				space->addDynamicActor(enemy);
				break;
			}
			case 'P': {
				player = new Player(x, y, game);
				// modificación para empezar a contar desde el suelo.
				player->y = player->y - player->height / 2;
				space->addDynamicActor(player);
				break;
			}
			case 'M': {
				Moneda* moneda = new Moneda(x, y, game);
				// modificación para empezar a contar desde el suelo.
				moneda->y = moneda->y - moneda->height / 2;
				monedas.push_back(moneda);
				space->addDynamicActor(moneda);
				break;
			}
			case 'W': {
				pow = new PowBlock(x,y,game);
				// modificación para empezar a contar desde el suelo.
				pow->y = pow->y - pow->height / 2;
				space->addStaticActor(pow);
				break;
			}
			case '#': {
				Tile* tile = new Tile("res/plataforma1.png", x, y, game);
				// modificación para empezar a contar desde el suelo.
				tile->y = tile->y - tile->height / 2;
				tiles.push_back(tile);
				space->addStaticActor(tile);
				break;
			}
		}
}

void GameLayer::patronGeneracionActores()
{
	//Vacia la cola
	while (!listaActores.empty()) {
		listaActores.pop();
	}
	//Llenarla
	listaActores.push('M');
	listaActores.push('K');
	listaActores.push('C');
	listaActores.push('M');
	listaActores.push('K');
	listaActores.push('M');
	listaActores.push('K');
	listaActores.push('C');
	listaActores.push('M');
	listaActores.push('K');
	listaActores.push('K');
	listaActores.push('K');
	listaActores.push('M');
	listaActores.push('M');
	listaActores.push('K');
	listaActores.push('K');
	listaActores.push('K');
	listaActores.push('M');
}

void GameLayer::generacionActores() {
	if (!listaActores.empty()) {

		switch (listaActores.top()) {
			case 'K': {
				EnemySimple* enemy = new EnemySimple(WIDTH * 0.8, HEIGHT * 0.83, game);
				if (listaActores.size() % 2 == 0) {
					enemy->vx = enemy->vx * -1;
				}
				enemieSimple.push_back(enemy);
				space->addDynamicActor(enemy);
				listaActores.pop();
				break;
			} case 'H': {
				EnemyEspecial* enemy = new EnemyEspecial(WIDTH * 0.8, HEIGHT * 0.83, game);
				if (listaActores.size() % 2 == 0) {
					enemy->vx = enemy->vx * -1;
				}
				enemieEspecial.push_back(enemy);
				space->addDynamicActor(enemy);
				listaActores.pop();
				break;
			} case 'C': {
				EnemyVidaExtra* enemy = new EnemyVidaExtra(WIDTH * 0.8, HEIGHT * 0.83, game);
				if (listaActores.size() % 2 == 0) {
					enemy->vx = enemy->vx * -1;
				}
				enemieVidaExtra.push_back(enemy);
				space->addDynamicActor(enemy);
				listaActores.pop();
				break;
			} case 'M': {
				Moneda* moneda = new Moneda(WIDTH * 0.8, HEIGHT * 0.83, game);
				if (listaActores.size() % 2 == 0) {
					moneda->vx = moneda->vx * -1;
				}
				monedas.push_back(moneda);
				space->addDynamicActor(moneda);
				listaActores.pop();
				break;
			}
		}
	}
}

void GameLayer::tpTuberia1() {
	if (tpTimeTuberia1 <= 0 && colaTuberia1.size() > 0) {
		Actor* actor = colaTuberia1.top();
		actor->x = tuberia1->x + tuberia1->width / 2;
		actor->y = tuberia1->y;
		actor->vx = actor->vx * -1;
		tpTimeTuberia1 = 30;
		colaTuberia1.pop();
	}
}

void GameLayer::tpTuberia2() {
	if (tpTimeTuberia2 <= 0 && colaTuberia2.size() > 0) {
		Actor* actor = colaTuberia2.top();
		actor->x = tuberia2->x - tuberia2->width / 2;
		actor->y = tuberia2->y;
		actor->vx = actor->vx * -1;
		tpTimeTuberia2 = 30;
		colaTuberia2.pop();
	}
}

bool GameLayer::yaEstaEnCola1(Actor* actor)
{
	if (colaTuberia1.size() > 0) {
		stack<Actor*> cola = colaTuberia1;
		while (cola.size() > 0) {
			Actor* actorCola = cola.top();
			if (actorCola == actor) {
				return true;
			}
			cola.pop();
		}
	}
	return false;
}

bool GameLayer::yaEstaEnCola2(Actor* actor)
{
	if (colaTuberia2.size() > 0) {
		stack<Actor*> cola = colaTuberia2;
		while (cola.size() > 0) {
			Actor* actorCola = cola.top();
			if (actorCola == actor) {
				return true;
			}
			cola.pop();
		}
	}
	return false;
}

void GameLayer::golpearEnemigos()
{
	for (auto const& enemy : enemieSimple) {
		enemy->die();
	}
	for (auto const& enemy : enemieEspecial) {
		enemy->die();
	}
	for (auto const& enemy : enemieVidaExtra) {
		enemy->tumbar();
	}
}

void GameLayer::nextWindow()
{
	game->currentLevel++;

	if (game->currentLevel > game->finalLevel) {
		game->points = to_string(puntos);
		init();
		game->currentLevel = 1;
		nMonedas = 0;
		puntos = 0;
		winner = false;
		looser = false;
		game->layer = game->finalLayer;
		game->layer->play();

	}
	else {
		init();
	}
}

void GameLayer::getTiempo()
{
	time_t tActual = time(0);

	tiempo = tActual - tStart;
	time_t minutos = tiempo / 60;
	time_t segundos = tiempo - (minutos * 60);
	string formato = "";

	if (minutos >= 60) {
		formato = to_string(minutos / 60) + "H";
	}
	else {
		if (minutos >= 10) {
			formato = to_string(minutos);
			if (segundos >= 10) {
				formato += ":" + to_string(segundos);
			}
			else {
				formato += ":0" + to_string(segundos);
			}
		}
		else {
			formato = "0" + to_string(minutos);
			if (segundos >= 10) {
				formato += ":" + to_string(segundos);
			}
			else {
				formato += ":0" + to_string(segundos);
			}
		}
	}

	reloj->content = formato;
}

void GameLayer::lose()
{
	if (player->lifes <= 0) {
		mensajeGameOver = new Actor("res/mensaje_gameover.png", WIDTH * 0.5, HEIGHT * 0.5, 500, 292, game);
		pausa = true;
		looser = true;
		puntos = 0;
		nMonedas = 0;
		game->currentLevel = 1;
		init();
		return;
	}
}

void GameLayer::win() {
	if (listaActores.empty() 
		&& enemieSimple.empty() 
		&& enemieVidaExtra.empty() 
		&& monedas.empty()) {
		mensajeSiguienteNivel = new Actor("res/mensaje_nivel.png", WIDTH * 0.5, HEIGHT * 0.5, 500, 274, game);
		winner = true;
		pausa = true;
		int puntosVidaMonedas = (player->lifes * 50) + (nMonedas * 5);
		int puntosTiempo = 500;
		if (tiempo - 120 > 0) {
			puntosTiempo = puntosTiempo - (((tiempo - 120) / 10) * 50);
		}
		puntos += puntosVidaMonedas + puntosTiempo;
		nextWindow();
	}
}



