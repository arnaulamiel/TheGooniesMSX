#ifndef _BAT_INCLUDE
#define _BAT_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

#include "Enemies.h"

class Bat : public Enemies
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	virtual void setTileMap(TileMap* tileMap);
	virtual void setPosition(const glm::vec2& pos);
	virtual void setPlayer(Player* player);
	virtual void setPatrolPoints(int pIni, int pEnd);

	virtual void changeSpriteAnimation(int anim);
	virtual int getSpriteAnimation();

	virtual glm::ivec2 getPosition();
	virtual bool isDead();
	void killedBat();
	void destroyBat();
	

private:

	ShaderProgram texProgram;
	bool bRespawinig, bDead, isUp;
	int spawnAnim, state, xIni, xEnd, endAnim, yPosMax, yPosMin;
	glm::ivec2 tileMapDispl, posBat;
	Player* player;
	Texture spritesheet;
	ISoundEngine* sound;
	Sprite* sprite;
	TileMap* map;

};

#endif _BAT_INCLUDE