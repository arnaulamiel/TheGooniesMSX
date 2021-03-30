#ifndef _CALAVERA_INCLUDE
#define _CALAVERA_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

class Calavera
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);

	void changeSpriteAnimation(int anim);
	int getSpriteAnimation();

	glm::ivec2 getCalPosition();

private:

	ShaderProgram texProgram;
	bool bRespawinig, bDead;
	int spawnAnim, state;
	glm::ivec2 tileMapDispl, posCal;
	Player* player;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	
};

#endif // !_CALAVERA_INCLUDE