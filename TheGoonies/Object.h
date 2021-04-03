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
	LOCK
};

enum gotaAnim {
	INI, DOWN_1, DOWN_2, SPLASH, NUM_GOTA
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

	void setObjectPosition(glm::vec2 position);

private:

	bool objDest;

	objectType oType;

	glm::vec2 position;
	glm::ivec2 size;

	Texture spritesheet;
	Sprite* sprite;
};

#endif