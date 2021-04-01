#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Bat.h"

enum BatAnims
{
	BAT_RESPAWN,
	BAT_MOVELEFT,
	BAT_MOVERIGHT,
	BAT_DEAD,
	NUM_ANIMS
};

void Bat::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spawnAnim = 0;
	endAnim = 0;
	isUp = false;
	state = BAT_RESPAWN;
	bDead = false;
	bRespawinig = true;
	spritesheet.loadFromFile("images/SpriteSheetBat.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(26, 16), glm::vec2(0.333f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);

	//Animaciones de respawn
	sprite->setAnimationSpeed(BAT_RESPAWN, 8);
	sprite->addKeyframe(BAT_RESPAWN, glm::vec2(0.f, 0.0f));

	//Animaciones izquierda
	sprite->setAnimationSpeed(BAT_MOVELEFT, 8);
	sprite->addKeyframe(BAT_MOVELEFT, glm::vec2(0.333f, 0.0f));
	sprite->addKeyframe(BAT_MOVELEFT, glm::vec2(0.0f, 0.f));

	//Animaciones derecha
	sprite->setAnimationSpeed(BAT_MOVERIGHT, 8);
	sprite->addKeyframe(BAT_MOVERIGHT, glm::vec2(0.333f, 0.0f));
	sprite->addKeyframe(BAT_MOVERIGHT, glm::vec2(0.0f, 0.0f));

	//Animaciones de muerte
	sprite->setAnimationSpeed(BAT_DEAD, 8);
	sprite->addKeyframe(BAT_DEAD, glm::vec2(0.666f, 0.0f));

	sprite->changeAnimation(BAT_RESPAWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBat.x), float(tileMapDispl.y + posBat.y)));

}

void Bat::update(int deltaTime) {
	sprite->update(deltaTime);

	if (!bDead) {
		switch (state) {
		case BAT_RESPAWN:
			//Para el inicio 
			++spawnAnim;
			if (spawnAnim == 40) {
				bRespawinig = false;
				if (player->getPosPlayer().x <= posBat.x) {
					sprite->changeAnimation(BAT_MOVELEFT);
					posBat.x -= 2;
					state = BAT_MOVELEFT;
				}
				else {
					sprite->changeAnimation(BAT_MOVERIGHT);
					posBat.x += 2;
					state = BAT_MOVERIGHT;
				}
			}
			break;
		case BAT_MOVELEFT:
			posBat.x -= 2;
			if (isUp) 
			{
				if (posBat.y >= yPosMax) {
					posBat.y -= 1;
					isUp = false;
				}
				else posBat.y += 1;
			}
			else {
				if (posBat.y <= yPosMin) {
					posBat.y += 1;
					isUp = true;
				}
				else posBat.y -= 1;
			}
			
			/*if(isUp)posBat.y -= 1;
			else posBat.y += 1;*/
			

			//if (map->collisionMoveLeft(posBat, glm::ivec2(2, 2))) {
			if (posBat.x <= xIni) {
				posBat.x += 2;
				/*if (isUp)posBat.y += 1;
				else posBat.y -= 1;*/

				sprite->changeAnimation(BAT_MOVERIGHT);
				state = BAT_MOVERIGHT;

			}
			
			break;
		case BAT_MOVERIGHT:
			posBat.x += 2;
			if (isUp)
			{
				if (posBat.y >= yPosMax) {
					posBat.y -= 1;
					isUp = false;
				}
				else posBat.y += 1;
			}
			else {
				if (posBat.y <= yPosMin) {
					posBat.y += 1;
					isUp = true;
				}
				else posBat.y -= 1;
			}
			//if (map->collisionMoveRight(posBat, glm::ivec2(2, 2))) {
			if (posBat.x >= xEnd) {
				posBat.x -= 2;
				sprite->changeAnimation(BAT_MOVELEFT);
				state = BAT_MOVELEFT;
			}
			break;
		case BAT_DEAD:
			++endAnim;
			sprite->changeAnimation(BAT_DEAD);
			if (endAnim == 30)destroyBat();

			break;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBat.x), float(tileMapDispl.y + posBat.y)));

}

void Bat::render()
{
	if (!bDead)
		sprite->render();
}

void Bat::setPosition(const glm::vec2& pos) {
	posBat = pos;
	yPosMax = posBat.y + 20;
	yPosMin = posBat.y - 20;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBat.x), float(tileMapDispl.y + posBat.y)));
}
void Bat::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bat::changeSpriteAnimation(int anim)
{
	sprite->changeAnimation(anim);
}

int Bat::getSpriteAnimation()
{
	return sprite->animation();
}

glm::ivec2 Bat::getPosition()
{
	return posBat;
}

void Bat::setPlayer(Player* p)
{
	player = p;
}

void Bat::setPatrolPoints(int pIni, int pEnd) {
	xIni = pIni;
	xEnd = pEnd;
}

void Bat::killedBat() {
	//se murió
	state = BAT_DEAD;
}

void Bat::destroyBat() {
	bDead = true;
}
