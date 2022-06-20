#pragma once
#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "EnemySimple.h"
#include "EnemyEspecial.h"
#include "EnemyVidaExtra.h"
#include "Moneda.h"
#include "PowBlock.h"

#include "Tile.h"

#include "Audio.h" 
#include "Space.h" 
#include "Text.h"
#include <fstream> 
#include <sstream> 

#include <list>
#include <stack>

class GameLayer :
    public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void patronGeneracionActores();
	void generacionActores();
	void win();
	void tpTuberia1();
	void tpTuberia2();
	bool yaEstaEnCola1(Actor* actor);
	bool yaEstaEnCola2(Actor* actor);
	void golpearEnemigos();
	void nextWindow();
	void getTiempo();
	void lose();

	Space* space;
	int mapWidth;
	list<Tile*> tiles;
	Player* player;
	Background* background;
	int controlMoveY = 0;
	int controlMoveX = 0;
	Actor* lifeIcon;
	Text* lifePoints;

	int newActorTime = 500;

	stack<char> listaActores;

	list<EnemySimple*> enemieSimple;
	list<EnemyEspecial*> enemieEspecial;
	list<EnemyVidaExtra*> enemieVidaExtra;
	list<Moneda*> monedas;

	Audio* audioMoneda;
	Audio* audioMuerteEnemigo;
	Audio* audioPerderVida;
	Audio* audioCongelar;
	Audio* audioPow;

	PowBlock* pow;

	stack<Actor*> colaTuberia1;
	stack<Actor*> colaTuberia2;

	int tpTimeTuberia1 = 30;
	int tpTimeTuberia2 = 30;

	int nMonedas = 0;

	bool pausa = false;
	bool continuar = false;

	bool winner = false;
	bool looser = false;

	time_t tiempo = 0;
	time_t tStart = 0;
	Text* reloj;
	Actor* iconoReloj;

	Actor* tuberia1;
	Actor* tuberia2;
	Actor* tuberia3;
	Actor* tuberia4;

	Actor* mensajeSiguienteNivel;
	Actor* mensajeGameOver;

	Audio* audioSiguienteNivel;
	Audio* audioGameOver;

	int puntos = 0;
};

