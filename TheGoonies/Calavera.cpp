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
	endAnim = 0;
	state = CAL_RESPAWN;
	bDead = false;
	bRespawinig = true;
	spritesheet.loadFromFile("images/SpriteSheetCalavera.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(20, 24), glm::vec2(0.50f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);
		
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
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCal.x), float(tileMapDispl.y + posCal.y)));

}
void Calavera::update(int deltaTime) {
	sprite->update(deltaTime);
	int tilesize = map->getTileSize();
	if(!bDead){
		switch (state) {
		case CAL_RESPAWN:
			//Para el inicio 
			++spawnAnim;
			if (spawnAnim == 80) {
				bRespawinig = false;
				if (player->getPosPlayer().x <= posCal.x) {
					sprite->changeAnimation(CAL_LEFT);
					posCal.x -= 2;
					state = CAL_LEFT;
				}
				else {
					sprite->changeAnimation(CAL_RIGHT);
					posCal.x += 2;
					state = CAL_RIGHT;
				}
			}
			break;
		case CAL_LEFT:
			posCal.x -= 2;

			//if (map->collisionMoveLeft(posCal, glm::ivec2(2, 2))) {
			if (posCal.x <= xIni) {
				posCal.x += 2;
				sprite->changeAnimation(CAL_RIGHT);
				state = CAL_RIGHT;
			}
			//Si hay colision con el player
			
			if ((posCal.x / tilesize) == (player->getPosPlayer().x / tilesize) ) {
				if (posCal.y / tilesize >= (player->getPosPlayer().y / tilesize) - 1 && posCal.y / tilesize <= (player->getPosPlayer().y / tilesize) + 1) {
					if (!player->isHittedPlayer()) {
						sound = createIrrKlangDevice();
						sound->play2D("../../../libs/irrKlang-1.6.0/media/hitPlayer.wav", false);
					}
					player->hitByEnemy();
				}
			}
			break;
		case CAL_RIGHT:
			posCal.x += 2;


			//if (map->collisionMoveRight(posCal, glm::ivec2(2, 2))) {
			if (posCal.x >= xEnd) {
				posCal.x -= 2;
				sprite->changeAnimation(CAL_LEFT);
				state = CAL_LEFT;
			}
			if ((posCal.x / tilesize) == (player->getPosPlayer().x / tilesize)) {
				if (posCal.y / tilesize >= (player->getPosPlayer().y / tilesize) - 1 && posCal.y / tilesize <= (player->getPosPlayer().y / tilesize) + 1) {
					//if (posCal.x == player->getPosPlayer().x && ((player->getPosPlayer().y <= (posCal.y + 4) ) && (player->getPosPlayer().y >= posCal.y - 4)) ) {
					if (!player->isHittedPlayer()) {
						sound = createIrrKlangDevice();
						sound->play2D("../../../libs/irrKlang-1.6.0/media/hitPlayer.wav", false);
					}
					player->hitByEnemy();
				}
			}
			break;
		case CAL_DEAD:
			++endAnim;
			sprite->changeAnimation(CAL_DEAD);
			if(endAnim == 30)destroyCal();

			break;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCal.x), float(tileMapDispl.y + posCal.y)));

}

void Calavera::render()
{
	if(!bDead)
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

glm::ivec2 Calavera::getPosition()
{
	return posCal;
}

void Calavera::setPlayer(Player* p)
{
	player = p;
}

void Calavera::setPatrolPoints(int pIni, int pEnd) {
	xIni = pIni;
	xEnd = pEnd;
}

void Calavera::killedCal() {
	//se muri?
	state = CAL_DEAD;
}

void Calavera::destroyCal() {
	bDead = true;
}

bool Calavera::isDead() {
	return (state == CAL_DEAD);

}