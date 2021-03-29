#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Calavera.h"

enum CalavAnims
{
	CAL_RESPAWN,
	CAL_LEFT,
	CAL_RIGHT,
	CAL_DEAD,
	NUM_ANIMS
};

void Calavera::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spawnAnim = 0;
	bRespawinig = true;
	bDead = false;
	spritesheet.loadFromFile("images/SpriteSheetCalavera.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(20, 24), glm::vec2(0.50f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);
		
		//Hay que modificarlo cuando tengamos el spritesheet!!!!!!!!!!
		//Animaciones de respawn
		sprite->setAnimationSpeed(CAL_RESPAWN, 8);
		sprite->addKeyframe(CAL_RESPAWN, glm::vec2(0.f, 0.50f));
		sprite->addKeyframe(CAL_RESPAWN, glm::vec2(0.50f, 0.50f));

		//Animaciones izquierda
		sprite->setAnimationSpeed(CAL_LEFT, 8);
		sprite->addKeyframe(CAL_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(CAL_LEFT, glm::vec2(0.5f, 0.f));

		//Animaciones derecha
		sprite->setAnimationSpeed(CAL_RIGHT, 8);
		sprite->addKeyframe(CAL_RIGHT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(CAL_RIGHT, glm::vec2(0.5f, 0.25f));

		//Animaciones de muerte
		sprite->setAnimationSpeed(CAL_DEAD, 8);
		sprite->addKeyframe(CAL_DEAD, glm::vec2(0.f, 0.75f));

	sprite->changeAnimation(CAL_RESPAWN);
	// ??tileMapDispl = tileMapPos;
	//??sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCal.x), float(tileMapDispl.y + posCal.y)));

}
void Calavera::update(int deltaTime) {
	sprite->update(deltaTime);
	//Para el inicio 
	if (bRespawinig) {
		++spawnAnim;
		if (spawnAnim == 80) {
			bRespawinig = false;
			if (player->getPosPlayer().x <= posCal.x) {
				sprite->changeAnimation(CAL_LEFT);
				posCal.x -= 2;
			}
			else {
				sprite->changeAnimation(CAL_RIGHT);
				posCal.x += 2;
			}
		}
	}
	else {
		//Dependiendo de donde mire la calavera, anda hacia un lado u otro
		if (sprite->animation() == CAL_LEFT)posCal.x -= 2;
		else if(sprite->animation() == CAL_RIGHT) posCal.x += 2;

		//Si hay colision a los lados no avanza y cambia de direccion		
		if (map->collisionMoveLeft(posCal, glm::ivec2(36, 20))) {
			if (sprite->animation() == CAL_LEFT) {
				posCal.x += 2;
				sprite->changeAnimation(CAL_RIGHT);
			}
		}
		/*if (map->collisionMoveRight(posCal, glm::ivec2(32, 20))) {
			if (sprite->animation() == CAL_RIGHT) {
				posCal.x -= 2;
				sprite->changeAnimation(CAL_LEFT);
			}
		}*/
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCal.x), float(tileMapDispl.y + posCal.y)));

}

void Calavera::render()
{
	sprite->render();
}

void Calavera::setPosition(const glm::vec2& pos) {
	posCal = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCal.x), float(tileMapDispl.y + posCal.y)));
}
void Calavera::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Calavera::changeSpriteAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

int Calavera::getSpriteAnimation()
{
	return sprite->animation();
}

glm::ivec2 Calavera::getCalPosition()
{
	return posCal;
}

void Calavera::setPlayer(Player* p)
{
	player = p;
}