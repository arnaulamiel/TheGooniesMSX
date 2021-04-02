#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

//Seria millor fer una clase 'entity' i player hereda d'ell, aixi podem fer que 'entity' fos el menu, el background, els players, els enemics........ 
//entity ->instancia qualsevol, una cosa k sha de pintar, té posicio, o si esta viu o mort, i el k vulguem 
class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	void iniPlayerStats(ShaderProgram& shaderPrograma);
	glm::ivec2 getPosPlayer();
	void setState(int sta);
	int getState();
	void calHit();
	void hitByEnemy();
	void setHitAnimation();
	bool isSameAsBefore();

private:
	

	ShaderProgram texProgram;
	int vidasPlayer,expPlayer, timerHit, before;
	bool bJumping,bHitting, bLiana, isHitted;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, estado;
	Texture spritesheet;
	Texture ssheet;
	Texture ssheetexp;
	Sprite *sprite;
	Sprite* vidaSprite;
	Sprite* expSprite;
	TileMap *map;
};


#endif // _PLAYER_INCLUDE


