#pragma once
#include "Game.h"
#include "Scene.h"
#include "Sprite.h"
#include <iostream>
#include "Calavera.h"
#include "Bat.h"
#include "Object.h"
#include "Texture.h"

/* @file playScene.h
*
* @brief This class represents the first scene the will be shown when booting the game.
*
* It is basically the scene where the logo is shown.
* It is inherited from the Scene class.
*/
class playScene : public Scene
{
public:

	/* Static member function declaration */
	static Scene* create();

	/* Constructor */
	playScene();
	/* Destructor */
	virtual ~playScene();

	/* Inherited functions */
	virtual void init() override;
	virtual void update(int deltaTime) override;
	virtual void render() override;
	virtual void fin() override;
	void initMaps() ;
	void updateRoom();
	void loadRoomObjects();
	void updateActualObjects();
	void saveActualObject();

	bool hitEnem(Enemies* c);
	bool getObject(Object* k);
	bool getObjDoor(Object* door);
	bool gotaHitsPlayer(Object* g);
	bool fireHitsPlayer(Object* f);
	void initChildsInterface(ShaderProgram& shaderPrograma);

private:
	/* Function to initialize the shaders */
	void initShaders();

	bool loadSingleRoomObjects(string levelFile, vector<Object*>& objectsRoom);
	void calculateDownObstaculo(Object* obs);

	

	/* Variable to control the time that the logo is shown */
	int count;
	int room;
	int timerGota;
	int timerRoca;
	int timerFuego, timesFireAnim;
	/* Texture of the logo */
	Texture logoTexture, vidaexpTexture;

	bool hasKey,doorOpen, hasChild;;
	bool bgota;
	bool byeRoca;
	bool rocaDown;

	int numChilds;

	TileMap* map;
	TileMap* mapIni;
	TileMap* mapIni2;
	TileMap* mapIni3;

	float currentTime;

	/* Objects */
	vector<Object*> actualRoomObjects;
	vector<Object*> objectsRoom1;

	/* Logo sprite*/
	Sprite* logo;
	Sprite* vidaexp;
	Player* player;
	Calavera* c[1];
	Calavera* cal;
	Bat* bat;

	/* Projection matrix */
	glm::mat4 projection;
	/* Shader program*/
	ShaderProgram texProgram;
};



