#pragma once
#ifndef _OBJECT_INCLUDE
#define _OBJECT_INCLUDE

#include "Sprite.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Texture.h"

enum objectType
{
	KEY,
	GOTA,
	DOOR_CHILD, 
	LOCK,
	ROCA,
	FIRE,
	PORTAL,
	BLUE_HEL,
	YELLOW_HEL,
	GREEN_RAIN,
	GRAY_RAIN,
	HYPERSHOES
};

enum gotaAnim {
	INI, DOWN_1, DOWN_2, SPLASH, NUM_GOTA
};
enum rockAnim {
	R_INI, R_DOWN, R_SPLASH, NUM_R
};
enum fireAnim {
	F_INI, FIRE1, FIRE2, NUM_F
};

enum doorChildAnim {
	CLOSED, OPEN_CHILD, OPEN_EMPTY, NUM_DOOR_CHILD
};


class Object
{

public:

	Object(objectType type, glm::ivec2 pos, glm::vec2 size);
	~Object();

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void fin();

	objectType getObjectType();
	glm::vec2 getObjectPosition();
	glm::vec2 getObjectSize();
	
	void changeSpriteAnimation(int anim);
	void destroyObject();
	int getSpriteAnimation();

	void setObjectPosition(glm::ivec2 position);
	void setIniPosition(glm::ivec2 position);
	glm::ivec2 getIniPosition();
	void setPatrolPoints(int pEnd);
	int getPatrolPoints();

private:

	bool objDest;

	objectType oType;

	glm::ivec2 position;
	glm::ivec2 iniposition;
	glm::ivec2 size;
	int yIni, yEnd;

	Texture spritesheet;
	Sprite* sprite;
};

#endif