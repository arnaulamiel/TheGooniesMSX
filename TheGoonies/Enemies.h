#ifndef _ENEMIES_INCLUDE
#define _ENEMIES_INCLUDE
#include "TileMap.h"
#include "Player.h"


class Enemies
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

	virtual void setTileMap(TileMap* tileMap) = 0;
	virtual void setPosition(const glm::vec2& pos) = 0;
	virtual void setPlayer(Player* player) = 0;
	virtual void setPatrolPoints(int pIni, int pEnd) = 0;

	virtual void changeSpriteAnimation(int anim) = 0;
	virtual int getSpriteAnimation() = 0;

	virtual glm::ivec2 getPosition() = 0;
	virtual bool isDead() = 0;
};

#endif _ENEMIES_INCLUDE