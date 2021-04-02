#include "Object.h"

Object::Object(objectType type, glm::ivec2 pos, glm::vec2 size)
{
	this->oType = type;
	this->position = pos;
	this->size = size;
}

Object::~Object()
{

}

void Object::init(ShaderProgram& shaderProgram)
{
	objDest = false;

	if (this->oType == KEY) {
		spritesheet.loadFromFile("images/keySprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(this->size, glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	}

	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite->setPosition(this->position);
}
void Object::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Object::render()
{
	if (!objDest) {
		sprite->render();
	}
}

void Object::fin()
{

}

objectType Object::getObjectType()
{
	return this->oType;
}

glm::vec2 Object::getObjectPosition()
{
	return this->position;
}

glm::vec2 Object::getObjectSize()
{
	return this->size;
}

void Object::changeSpriteAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

int Object::getSpriteAnimation()
{
	return sprite->animation();
}

void Object::destroyObject()
{
	objDest = true;
}