#ifndef _CALAVERA_INCLUDE
#define _CALAVERA_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

#include "Enemies.h"

class Calavera : public Enemies
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
	void killedCal();
	void destroyCal();

private:

	ShaderProgram texProgram;
	bool bRespawinig, bDead;
	int spawnAnim, state, xIni, xEnd, endAnim;
	glm::ivec2 tileMapDispl, posCal;
	Player* player;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif // !_CALAVERA_INCLUDE
