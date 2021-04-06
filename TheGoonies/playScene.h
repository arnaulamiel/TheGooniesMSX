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
	void initMaps(int scene) ;
	void updateRoom();
	void loadRoomObjects();
	void updateActualObjects();
	void saveActualObjects();

	bool hitEnem(Enemies* c);
	bool getObject(Object* k);
	bool getObjDoor(Object* door);
	bool gotaHitsPlayer(Object* g);
	bool fireHitsPlayer(Object* f);
	//void initChildsInterface(ShaderProgram& shaderPrograma);

	virtual void deleteEngine();
	virtual void changeMusic(char* music);
	virtual void createSound(char* music, bool repeat);
	virtual void deleteSound();

	void hitPlayer();
	void updateScene();
	void updateElementsScene();


private:
	/* Function to initialize the shaders */
	void initShaders();

	bool loadSingleRoomObjects(string levelFile, vector<Object*>& objectsRoom);
	void calculateDownObstaculo(Object* obs);
	bool playerinPortal(Object* p);
	void newScene();

	/* Variable to control the time that the logo is shown */
	int count;
	int room, scene;
	int timerGota;
	int timerRoca;
	int timerFuego, timesFireAnim, waitToEnd;
	/* Texture of the logo */
	Texture logoTexture, vidaexpTexture;

	ISoundEngine* engine;
	ISoundEngine* sound;


	bool hasKey,doorOpen, hasChild, hasBlueHel, hasYellowHel, hasGreenRain, hasGrayRain, hasHyperShoes;
	bool bgota;
	bool byeRoca;
	bool rocaDown;
	bool hasSound;
	bool inPortal;

	TileMap* map;
	TileMap* mapIni;
	TileMap* mapIni2;
	TileMap* mapIni3;

	float currentTime;

	/* Objects */
	vector<Object*> actualRoomObjects;
	vector<Object*> objectsRoom1_1;
	vector<Object*> objectsRoom1_2;
	vector<Object*> objectsRoom1_3;
	vector<Object*> objectsRoom2_1;
	vector<Object*> objectsRoom2_2;
	vector<Object*> objectsRoom2_3;
	vector<Object*> objectsRoom3_1;
	vector<Object*> objectsRoom3_2;
	vector<Object*> objectsRoom3_3;
	vector<Object*> objectsRoom4_1;
	vector<Object*> objectsRoom4_2;
	vector<Object*> objectsRoom4_3;
	vector<Object*> objectsRoom5_1;
	vector<Object*> objectsRoom5_2;
	vector<Object*> objectsRoom5_3;

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



