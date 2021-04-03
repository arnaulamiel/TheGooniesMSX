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
	else if (this->oType == GOTA) {
		spritesheet.loadFromFile("images/SpriteSheetGota.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(this->size, glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);

		sprite->setNumberAnimations(NUM_GOTA);

		sprite->setAnimationSpeed(INI, 8);
		sprite->addKeyframe(INI, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(DOWN_1, 8);
		sprite->addKeyframe(DOWN_1, glm::vec2(0.25f,0.0f));

		sprite->setAnimationSpeed(DOWN_2, 8);
		sprite->addKeyframe(DOWN_2, glm::vec2(0.5f, 0.0f));

		sprite->setAnimationSpeed(SPLASH, 8);
		sprite->addKeyframe(SPLASH, glm::vec2(0.75f, 0.0f));

		sprite->changeAnimation(INI);

	}
	else if (this->oType == DOOR_CHILD) {
		spritesheet.loadFromFile("images/doorChildSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(this->size, glm::vec2(0.333f, 1.f), &spritesheet, &shaderProgram);

		sprite->setNumberAnimations(NUM_DOOR_CHILD);

		sprite->setAnimationSpeed(CLOSED, 8);
		sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(OPEN_CHILD, 8);
		sprite->addKeyframe(OPEN_CHILD, glm::vec2(0.333f, 0.f));

		sprite->setAnimationSpeed(OPEN_EMPTY, 8);
		sprite->addKeyframe(OPEN_EMPTY, glm::vec2(0.666f, 0.f));

		sprite->changeAnimation(CLOSED);

	}
	else if (this->oType == LOCK) {
		spritesheet.loadFromFile("images/lock.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

void Object::setObjectPosition(glm::vec2 position) {
	this->position = position;
	sprite->setPosition(this->position);
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