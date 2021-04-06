#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <irrKlang.h>


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

//Seria millor fer una clase 'entity' i player hereda d'ell, aixi podem fer que 'entity' fos el menu, el background, els players, els enemics........ 
//entity ->instancia qualsevol, una cosa k sha de pintar, t� posicio, o si esta viu o mort, i el k vulguem 
class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	void iniChildsInterface(ShaderProgram& shaderPrograma);
	void iniPlayerStats(ShaderProgram& shaderPrograma);
	void iniPowerUpInterface(ShaderProgram& shaderPrograma);
	
	glm::ivec2 getPosPlayer();
	void setState(int sta);
	int getState();
	void calHit();
	void hitByEnemy();
	void setHitAnimation();
	bool isSameAsBefore();
	bool canTP();
	void incrementChild();
	int getHealth();
	bool isHittedPlayer();
	int getExp();
	void getBlueHelmet();
	bool hasBlueHelmet();
	bool soundXDash();
	void getYellowHelmet();
	void getGreenRaincoat();
	void getGrayRaincoat();
	void getHyperShoes();
	bool hasYellowHelmet();
	bool hasGreenRaincoat();
	bool hasGrayRaincoat();
	bool hasHypershoes();


private:
	
	vector<int> powerUps;
	ShaderProgram texProgram;
	int vidasPlayer,expPlayer, timerHit, before;
	bool bJumping,bHitting, bLiana, isHitted, bCooldownX, hasBlueHel, soundDash, flagDash, hasYellowHel, hasGreenRain, hasGrayRain, hasHyperShoes, godMode;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, estado, timerX, stateChild, timerGod;
	Texture spritesheet;
	Texture ssheet;
	Texture ssheetTimer;
	Texture ssheetexp;
	Texture ssheetch;
	Texture godsh;
	Texture powup1sh;
	Texture powup2sh;
	Texture powup3sh;
	Texture powup4sh;
	Texture powup5sh;
	Sprite *sprite;
	Sprite* vidaSprite;
	Sprite* expSprite;
	Sprite* timerSprite;
	Sprite* childSprite;
	Sprite* godSprite;
	Sprite* powUp1Sprite;
	Sprite* powUp2Sprite;
	Sprite* powUp3Sprite;
	Sprite* powUp4Sprite;
	Sprite* powUp5Sprite;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


