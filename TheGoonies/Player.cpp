#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 40
#define FALL_STEP 4

#define SPACEBAR 32
#define INI_VIDAS 8 //Numero de hits que necesita darte una calavera o las balas del enemigo copia para matarte


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB_1, CLIMB_2, HIT_LEFT, HIT_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//estat de salt -> normalment els bool son dolents perk solen haber mes estats, pero com es un salt pues ja va be
	bJumping = false;
	bHitting = false;
	//load una imatge
	spritesheet.loadFromFile("images/spritesheetPlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//classe sprite -> creem el sprite (tama�o del jugador (18 es un tile))
	sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2(0.2, 0.333), &spritesheet, &shaderProgram);
	//aquest sprite te 4 animacions
	sprite->setNumberAnimations(10);
	//i son aquestes
		//tants keyframes com vulguem del spritesheet (ja que pillem de l'arxiu del sprite)
		//tb necessitem k entre dos sprites pintats (com x exemp caminant) hi hagi cert temps de retard perk no es vegi k camina super rapid
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.666f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.333f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.666f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.666f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.666f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.333f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.333f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.333f));

		//Animaciones de salto
		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.6f, 0.666f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.333f));
		
		//Animaciones de subir cuerda
		sprite->setAnimationSpeed(CLIMB_1, 8);
		sprite->addKeyframe(CLIMB_1, glm::vec2(0.0f, 0.0f));

		sprite->setAnimationSpeed(CLIMB_2, 8);
		sprite->addKeyframe(CLIMB_2, glm::vec2(0.2f, 0.0f));

		//Animaciones de pegar
		sprite->setAnimationSpeed(HIT_LEFT, 8);
		sprite->addKeyframe(HIT_LEFT, glm::vec2(0.8f, 0.666f));
		sprite->addKeyframe(HIT_LEFT, glm::vec2(0.2f, 0.666f));

		sprite->setAnimationSpeed(HIT_RIGHT, 8);
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(0.8f, 0.333f));
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(0.2f, 0.333f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	
	
}

//moviment
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		//hem de comprovar si no estavem ja fent el moviment a la esquerra per poderlo fer ara (aixi amb la majoria de comprovacions)
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 20)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 20)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
		
	}
	else if (Game::instance().getKey(SPACEBAR)) {
		if (!bHitting)
		{
			bHitting = true;
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == (STAND_LEFT))
				sprite->changeAnimation(HIT_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == (STAND_RIGHT))
				sprite->changeAnimation(HIT_RIGHT);
		}		
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	//Booleano para que deje de hacer la animacion de pegar cuando ya la ha hecho
	if (!bHitting) {
		if (sprite->animation() == HIT_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == HIT_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	bHitting = false;

	
	if(bJumping)
	{
		//en aquest cas, el alpha augmenta de x en x graus, perk si, sha decidit aixi
		jumpAngle += JUMP_ANGLE_STEP;
		
		
		if ((sprite->animation() != JUMP_LEFT) && (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT))
			sprite->changeAnimation(JUMP_LEFT);
		else if ((sprite->animation() != JUMP_RIGHT) && (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT))
			sprite->changeAnimation(JUMP_RIGHT);

		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 20), &posPlayer.y))
		{
			
			jumpAngle -= JUMP_ANGLE_STEP ;
			bJumping = false;
			posPlayer.y += FALL_STEP;
			//startY = posPlayer.y;
			
		}
		else {

			//si arribem a 180, deixes de saltar i ja no et mous amb el sinus
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
				/*if (sprite->animation() == JUMP_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMP_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);*/
			}//si no arriba a 180, segueix pujant
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 20), &posPlayer.y);

				//if (jumpAngle > 90) {				
				if (!bJumping) {
					if (sprite->animation() == JUMP_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == JUMP_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
				}
				//}
			}
		}
	}
	else
	{
		if (sprite->animation() == JUMP_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == JUMP_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);

		//si no estem saltant, depenent si hi ha colisio o no, caiem o no caiem
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 20), &posPlayer.y))
		{
			

			//No diferencia entre Release o press perk si ja estem saltant no entra en aquest else, nomes si no esta saltant i es clica a saltar
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//Baixar vides del jugador (k)
	if (Game::instance().getKey(107) ){
		vidasPlayer = vidasPlayer - 1;
		//cout << vidasPlayer << endl;
	}
	if (vidasPlayer == 0) {
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::END_SCENE);
	}
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setVidas()
{
	vidasPlayer = INI_VIDAS; 
}

glm::ivec2 Player::getPosPlayer() {
	return posPlayer;
}




