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

#define VIDAEXP_X 175
#define VIDAEXP_Y 22


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB, HIT_LEFT, HIT_RIGHT, DMG_STAND_LEFT, DMG_STAND_RIGHT, DMG_MOVE_LEFT, DMG_MOVE_RIGHT, DMG_JUMP_LEFT, DMG_JUMP_RIGHT, DMG_CLIMB, DMG_HIT_LEFT, DMG_HIT_RIGHT, NUM_ANIMS
};

enum PlayerVidaExp {
	VIDA_1, VIDA_2, VIDA_3, VIDA_4, VIDA_5, VIDA_6, VIDA_7, VIDA_8
};

enum PlayerExp {
	EXP_0, EXP_1, EXP_2, EXP_3, EXP_4, EXP_5, EXP_6, EXP_7, EXP_8
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//load una imatge
	spritesheet.loadFromFile("images/spritesheetPlayerDef.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//classe sprite -> creem el sprite (tamaño del jugador (18 es un tile))
	sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2(0.2, 0.166), &spritesheet, &shaderProgram);
	//aquest sprite te 4 animacions
	sprite->setNumberAnimations(NUM_ANIMS);
	//i son aquestes
		//tants keyframes com vulguem del spritesheet (ja que pillem de l'arxiu del sprite)
		//tb necessitem k entre dos sprites pintats (com x exemp caminant) hi hagi cert temps de retard perk no es vegi k camina super rapid
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.666f/2.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.333f / 2.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.666f / 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.666f / 2.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.666f / 2.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.333f / 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.333f / 2.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.333f / 2.f));

		//Animaciones de salto
		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.6f, 0.666f / 2.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.333f / 2.f));
		
		//Animaciones de subir cuerda
		sprite->setAnimationSpeed(CLIMB, 8);
		sprite->addKeyframe(CLIMB, glm::vec2(0.0f, 0.0f ));
		sprite->addKeyframe(CLIMB, glm::vec2(0.2f, 0.0));

		//Animaciones de pegar
		sprite->setAnimationSpeed(HIT_LEFT, 8);
		sprite->addKeyframe(HIT_LEFT, glm::vec2(0.8f, 0.666f / 2.f));
		sprite->addKeyframe(HIT_LEFT, glm::vec2(0.2f, 0.666f / 2.f));

		sprite->setAnimationSpeed(HIT_RIGHT, 8);
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(0.8f, 0.333f / 2.f));
		sprite->addKeyframe(HIT_RIGHT, glm::vec2(0.2f, 0.333f / 2.f));

		/*
		 * ANIMACIONES CUANDO LE HAN DADO AL PLAYER
		 */
		sprite->setAnimationSpeed(DMG_STAND_LEFT, 8);
		sprite->addKeyframe(DMG_STAND_LEFT, glm::vec2(0.f, 0.8333f));
		sprite->addKeyframe(DMG_STAND_LEFT, glm::vec2(0.f, 0.666f / 2.f));

		sprite->setAnimationSpeed(DMG_STAND_RIGHT, 8);
		sprite->addKeyframe(DMG_STAND_RIGHT, glm::vec2(0.0f, 0.666f));
		sprite->addKeyframe(DMG_STAND_RIGHT, glm::vec2(0.0f, 0.333f / 2.f));

		sprite->setAnimationSpeed(DMG_MOVE_LEFT, 8);
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.f, 0.8333f));
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.f, 0.666f / 2.f));
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.2f, 0.8333f));
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.2f, 0.666f/2.f));
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.4f, 0.8333f) );
		sprite->addKeyframe(DMG_MOVE_LEFT, glm::vec2(0.4f, 0.666f / 2.f));

		sprite->setAnimationSpeed(DMG_MOVE_RIGHT, 8);
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.f, 0.666f));
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.f, 0.333f / 2.f));
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.2f, 0.666f));
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.2f, 0.333f / 2.f));
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.4f, 0.666f));
		sprite->addKeyframe(DMG_MOVE_RIGHT, glm::vec2(0.4f, 0.333f / 2.f));

		//Animaciones de salto
		sprite->setAnimationSpeed(DMG_JUMP_LEFT, 8);
		sprite->addKeyframe(DMG_JUMP_LEFT, glm::vec2(0.6f, 0.8333f));
		sprite->addKeyframe(DMG_JUMP_LEFT, glm::vec2(0.6f, 0.666f / 2.f));

		sprite->setAnimationSpeed(DMG_JUMP_RIGHT, 8);
		sprite->addKeyframe(DMG_JUMP_RIGHT, glm::vec2(0.6f, 0.666f));
		sprite->addKeyframe(DMG_JUMP_RIGHT, glm::vec2(0.6f, 0.333f / 2.f));

		//Animaciones de subir cuerda
		sprite->setAnimationSpeed(DMG_CLIMB, 8);
		sprite->addKeyframe(DMG_CLIMB, glm::vec2(0.0f, 0.5f));
		sprite->addKeyframe(DMG_CLIMB, glm::vec2(0.0f, 0.0f));
		sprite->addKeyframe(DMG_CLIMB, glm::vec2(0.2f, 0.5f));
		sprite->addKeyframe(DMG_CLIMB, glm::vec2(0.2f, 0.0f));

		//Animaciones de pegar
		sprite->setAnimationSpeed(DMG_HIT_LEFT, 8);
		sprite->addKeyframe(DMG_HIT_LEFT, glm::vec2(0.8f, 0.8333f));
		sprite->addKeyframe(DMG_HIT_LEFT, glm::vec2(0.8f, 0.666f / 2.f));
		sprite->addKeyframe(DMG_HIT_LEFT, glm::vec2(0.2f, 0.8333f));
		sprite->addKeyframe(DMG_HIT_LEFT, glm::vec2(0.2f, 0.666f / 2.f));

		sprite->setAnimationSpeed(DMG_HIT_RIGHT, 8);
		sprite->addKeyframe(DMG_HIT_RIGHT, glm::vec2(0.8f, 0.666f));
		sprite->addKeyframe(DMG_HIT_RIGHT, glm::vec2(0.8f, 0.333f / 2.f));
		sprite->addKeyframe(DMG_HIT_RIGHT, glm::vec2(0.2f, 0.666f));
		sprite->addKeyframe(DMG_HIT_RIGHT, glm::vec2(0.2f, 0.333f / 2.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	iniPlayerStats(shaderProgram);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));


	
	
}

//moviment
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	vidaSprite->update(deltaTime);
	expSprite->update(deltaTime);


	//Esta en liana
	if (bLiana) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (map->hayLianaUp(posPlayer, glm::ivec2(32, 20))) {
				posPlayer.y -= 1;
			}
			else {
				bLiana = false;
				//setState(STAND_LEFT);
				if (isSameAsBefore()) {
					sprite->changeAnimation(STAND_LEFT);
					estado = STAND_LEFT;
					setHitAnimation();
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			if (map->hayLianaDown(posPlayer, glm::ivec2(32, 20))) {
				posPlayer.y += 1;
			}
			else {
				bLiana = false;
				sprite->changeAnimation(STAND_LEFT);
				estado = STAND_LEFT;
				setHitAnimation();
			}
		}
	}
	if (expPlayer == 8) {
		++vidasPlayer;
		expPlayer = 0;
	}
	if (vidasPlayer <= 0) {
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::END_SCENE);
	}
	//Si han pegado al jugador hace 20 frames ya no es invulnerable
	if (timerHit == 60) {
		isHitted = false;
		setHitAnimation();
	}

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !bLiana)
	{
		//hem de comprovar si no estavem ja fent el moviment a la esquerra per poderlo fer ara (aixi amb la majoria de comprovacions)
		if (sprite->animation() != MOVE_LEFT) {
			if (!isHitted) {
				sprite->changeAnimation(MOVE_LEFT);
				estado = MOVE_LEFT;
			}
			else if (sprite->animation() != DMG_MOVE_LEFT) {
				sprite->changeAnimation(DMG_MOVE_LEFT);
				estado = DMG_MOVE_LEFT;
			}
		}
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 20)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);

			estado = STAND_LEFT;
			setHitAnimation();
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !bLiana)
	{
		if (sprite->animation() != MOVE_RIGHT) {
			if (!isHitted) {
				sprite->changeAnimation(MOVE_RIGHT);
				estado = MOVE_RIGHT;
			}
			else if (sprite->animation() != DMG_MOVE_RIGHT) {
				sprite->changeAnimation(DMG_MOVE_RIGHT);
				estado = DMG_MOVE_RIGHT;
			}
		}
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 20)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
			estado = STAND_RIGHT;
			setHitAnimation();
		}
		
	}
	else if (Game::instance().getKey(SPACEBAR)) {
		if (!bHitting)
		{
			bHitting = true;
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == (STAND_LEFT)) {
				if (!isHitted) {
					sprite->changeAnimation(HIT_LEFT);
					estado = HIT_LEFT;
				}
				else if (sprite->animation() != DMG_HIT_LEFT) {

					sprite->changeAnimation(DMG_HIT_LEFT);
					estado = DMG_HIT_LEFT;
				}
			}
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == (STAND_RIGHT)) {
				if (!isHitted) {
					sprite->changeAnimation(HIT_RIGHT);
					sprite->changeAnimation(HIT_RIGHT);
					estado = HIT_RIGHT;
				}
				else if (sprite->animation() != DMG_HIT_RIGHT) {
					sprite->changeAnimation(DMG_HIT_RIGHT);
					estado = DMG_HIT_RIGHT;
				}
			}
		}		
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (map->hayLianaDown(posPlayer, glm::ivec2(32, 20)) & !bLiana) {			
			bLiana = true;
			bJumping = false;
			posPlayer.y += 1;
			sprite->changeAnimation(CLIMB);	
			estado = CLIMB;
			setHitAnimation();
		}
		else if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 20), &posPlayer.y)) {
			sprite->changeAnimation(STAND_LEFT);
			estado = STAND_LEFT;
			setHitAnimation();
			bLiana = false;
			posPlayer.y -= 1;
		}
	}//Baixar vides del jugador (k)
	else if (Game::instance().getKey(107)) {
			vidasPlayer = vidasPlayer - 1;
		}		
	else
	{
		if (sprite->animation() == MOVE_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
			estado = STAND_LEFT;
			setHitAnimation();
		}
		else if (sprite->animation() == MOVE_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
			estado = STAND_RIGHT;
			setHitAnimation();
		}
	}

	//Booleano para que deje de hacer la animacion de pegar cuando ya la ha hecho
	if (!bHitting) {
		if (sprite->animation() == HIT_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
			estado = STAND_LEFT;
			setHitAnimation();
		}
		else if (sprite->animation() == HIT_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
			estado = STAND_RIGHT;
			setHitAnimation();
		}
	}
	else bHitting = false;

	

	if (bJumping )
	{
		//en aquest cas, el alpha augmenta de x en x graus, perk si, sha decidit aixi
		jumpAngle += JUMP_ANGLE_STEP;


		if ((sprite->animation() != JUMP_LEFT) && (sprite->animation() == STAND_LEFT || sprite->animation() == MOVE_LEFT)) {
			sprite->changeAnimation(JUMP_LEFT);
			estado = JUMP_LEFT;
			setHitAnimation();
		}
		else if ((sprite->animation() != JUMP_RIGHT) && (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT)) {
			sprite->changeAnimation(JUMP_RIGHT);
			estado = JUMP_RIGHT;
			setHitAnimation();
		}

		if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 20), &posPlayer.y))
		{

			jumpAngle -= JUMP_ANGLE_STEP;
			bJumping = false;
			posPlayer.y += FALL_STEP;
			//startY = posPlayer.y;		

		}
		else {

			if (!map->hayLianaUp(posPlayer, glm::ivec2(32, 20)) && !map->hayLianaDown(posPlayer, glm::ivec2(32, 20))) {
				//si arribem a 180, deixes de saltar i ja no et mous amb el sinus
				if (jumpAngle == 180)
				{
					bJumping = false;
					posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));

					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 20), &posPlayer.y);

					if (!bJumping) {
						if (sprite->animation() == JUMP_LEFT) {
							sprite->changeAnimation(STAND_LEFT);
							estado = STAND_LEFT;
							setHitAnimation();
						}
						else if (sprite->animation() == JUMP_RIGHT) {
							sprite->changeAnimation(STAND_RIGHT);
							estado = STAND_RIGHT;
							setHitAnimation();
						}
					}
				}
			}
			else if (map->hayLianaUp(posPlayer, glm::ivec2(32, 20))) {

				bLiana = true;
				bJumping = false;
				posPlayer.y += 2;
				sprite->changeAnimation(CLIMB);
				estado = CLIMB;
				setHitAnimation();

			}
		}
	}
	else if(!bLiana)
	{
		if (sprite->animation() == JUMP_LEFT) {
			sprite->changeAnimation(STAND_LEFT);
			estado = STAND_LEFT;
			setHitAnimation();
		}
		else if (sprite->animation() == JUMP_RIGHT) {
			sprite->changeAnimation(STAND_RIGHT);
			estado = STAND_RIGHT;
			setHitAnimation();
		}

		//si no estem saltant, depenent si hi ha colisio o no, caiem o no caiem
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 20), &posPlayer.y))
		{
			//No diferencia entre Release o press perk si ja estem saltant no entra en aquest else, nomes si no esta saltant i es clica a saltar
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (!map->hayLianaUp(posPlayer, glm::ivec2(32, 20))) {
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
				else if(!bLiana) {
					bLiana = true;
					sprite->changeAnimation(CLIMB);
					estado = CLIMB;
					setHitAnimation();
					posPlayer.y += 2;
				}
			}
		}
	}
	if (isHitted) {
		++timerHit;
	}
	
	
		
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	vidaSprite->setPosition(glm::vec2(float(330), float(30)));
	expSprite->setPosition(glm::vec2(float(330), float(50)));
}

void Player::render()
{
	sprite->render();
	vidaSprite->render();
	expSprite->render();
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
void Player::setState(int sta) {
	
	estado = sta;
	sprite->changeAnimation(sta);
}
int Player::getState() {
	return estado;
}
void Player::iniPlayerStats(ShaderProgram& shaderProgram)
{
	bJumping = false;
	bHitting = false;
	bLiana = false;
	isHitted = false;

	
	estado = STAND_LEFT;
	vidasPlayer = INI_VIDAS; 
	expPlayer = 0;	
	
	ssheet.loadFromFile("images/VidaExpSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//classe sprite -> creem el sprite (tamaño del jugador (18 es un tile))
	vidaSprite = Sprite::createSprite(glm::ivec2(VIDAEXP_X, VIDAEXP_Y), glm::vec2(0.333f, 0.1666f), &ssheet, &shaderProgram);
	//aquest sprite te 4 animacions
		vidaSprite->setNumberAnimations(8);
	
		vidaSprite->setAnimationSpeed(VIDA_1, 8);
		vidaSprite->addKeyframe(VIDA_1, glm::vec2(0.f, 0.0f));

		vidaSprite->setAnimationSpeed(VIDA_2, 8);
		vidaSprite->addKeyframe(VIDA_2, glm::vec2(0.333f, 0.f));

		vidaSprite->setAnimationSpeed(VIDA_3, 8);
		vidaSprite->addKeyframe(VIDA_3, glm::vec2(0.666f, 0.f));

		vidaSprite->setAnimationSpeed(VIDA_4, 8);
		vidaSprite->addKeyframe(VIDA_4, glm::vec2(0.0f, 0.333f));

		vidaSprite->setAnimationSpeed(VIDA_5, 8);
		vidaSprite->addKeyframe(VIDA_5, glm::vec2(0.333f, 0.333f));

		vidaSprite->setAnimationSpeed(VIDA_6, 8);
		vidaSprite->addKeyframe(VIDA_6, glm::vec2(0.666f, 0.333f));

		vidaSprite->setAnimationSpeed(VIDA_7, 8);
		vidaSprite->addKeyframe(VIDA_7, glm::vec2(0.0f, 0.664f));

		vidaSprite->setAnimationSpeed(VIDA_8, 8);
		vidaSprite->addKeyframe(VIDA_8, glm::vec2(0.333f, 0.664f));

	vidaSprite->changeAnimation(VIDA_8);
	vidaSprite->setPosition(glm::vec2(float(433), float(11)));


	ssheetexp.loadFromFile("images/VidaExpSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	expSprite = Sprite::createSprite(glm::ivec2(VIDAEXP_X, VIDAEXP_Y), glm::vec2(0.333f, 0.1666f), &ssheetexp, &shaderProgram);
	//aquest sprite te 4 animacions
	expSprite->setNumberAnimations(8);


		expSprite->setAnimationSpeed(EXP_0, 8);
		expSprite->addKeyframe(EXP_0, glm::vec2(0.666f, 0.664f));

		expSprite->setAnimationSpeed(EXP_1, 8);
		expSprite->addKeyframe(EXP_1, glm::vec2(0.f, 0.1666f));

		expSprite->setAnimationSpeed(EXP_2, 8);
		expSprite->addKeyframe(EXP_2, glm::vec2(0.333f,  0.1666f));

		expSprite->setAnimationSpeed(EXP_3, 8);
		expSprite->addKeyframe(EXP_3, glm::vec2(0.666f, 0.1666f));

		expSprite->setAnimationSpeed(EXP_4, 8);
		expSprite->addKeyframe(EXP_4, glm::vec2(0.0f, 0.333f + 0.1666f));

		expSprite->setAnimationSpeed(EXP_5, 8);
		expSprite->addKeyframe(EXP_5, glm::vec2(0.333f, 0.333f + 0.1666f));

		expSprite->setAnimationSpeed(EXP_6, 8);
		expSprite->addKeyframe(EXP_6, glm::vec2(0.666f, 0.333f + 0.1666f));

		expSprite->setAnimationSpeed(EXP_7, 8);
		expSprite->addKeyframe(EXP_7, glm::vec2(0.0f, 0.664f + 0.1666f));

		expSprite->setAnimationSpeed(EXP_8, 8);
		expSprite->addKeyframe(EXP_8, glm::vec2(0.333f, 0.664f + 0.1666f));

	expSprite->changeAnimation(EXP_0);
	expSprite->setPosition(glm::vec2(float(433), float(31)));


}

glm::ivec2 Player::getPosPlayer() {
	return posPlayer;
}
void Player::calHit() {
	//ha dado a una calavera, hay que augmentar la exp
	++expPlayer;
	expSprite->changeAnimation(expPlayer);
}

void Player::hitByEnemy() {
	if (!isHitted) {
		--vidasPlayer;
		isHitted = true;
		setHitAnimation();
		timerHit = 0;
		vidaSprite->changeAnimation(vidasPlayer);
	}
}

void Player::setHitAnimation() {
	//STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB, HIT_LEFT, HIT_RIGHT
	// DMG_STAND_LEFT, DMG_STAND_RIGHT, DMG_MOVE_LEFT, DMG_MOVE_RIGHT, DMG_JUMP_LEFT, DMG_JUMP_RIGHT, DMG_CLIMB, DMG_HIT_LEFT, DMG_HIT_RIGHT
	if (isHitted /*&& isSameAsBefore()*/) {
		switch (estado)
		{
		case STAND_LEFT:
			
			sprite->changeAnimation(DMG_STAND_LEFT);
			estado = DMG_STAND_LEFT;
			before = STAND_LEFT;
			break;
		case STAND_RIGHT:
			sprite->changeAnimation(DMG_STAND_RIGHT);
			estado = DMG_STAND_RIGHT;
			before = STAND_RIGHT;
			break;
		case MOVE_LEFT:
			sprite->changeAnimation(DMG_MOVE_LEFT);
			estado = DMG_MOVE_LEFT;
			before = MOVE_LEFT;
			break;
		case MOVE_RIGHT:
			sprite->changeAnimation(DMG_MOVE_RIGHT);
			estado = DMG_MOVE_RIGHT;
			before = MOVE_RIGHT;
			break;
		case JUMP_LEFT:
			sprite->changeAnimation(DMG_JUMP_LEFT);
			estado = DMG_JUMP_LEFT;
			before = JUMP_LEFT;
			break;
		case JUMP_RIGHT:
			sprite->changeAnimation(DMG_JUMP_RIGHT);
			estado = DMG_JUMP_RIGHT;
			before = JUMP_RIGHT;
			break;
		case CLIMB:
			sprite->changeAnimation(DMG_CLIMB);
			estado = DMG_CLIMB;
			before = CLIMB;
			break;
		case HIT_LEFT:
			sprite->changeAnimation(DMG_HIT_LEFT);
			estado = DMG_HIT_LEFT;
			before = HIT_LEFT;
			break;
		case HIT_RIGHT:
			sprite->changeAnimation(DMG_HIT_RIGHT);
			estado = DMG_HIT_RIGHT;
			before = HIT_RIGHT;
			break;
		}
	}
	else if (!isHitted) {
		switch (estado)
		{
		case DMG_STAND_LEFT:
			sprite->changeAnimation(STAND_LEFT);
			estado = STAND_LEFT;
			break;
		case DMG_STAND_RIGHT:
			sprite->changeAnimation(STAND_RIGHT);
			estado = STAND_RIGHT;
			break;
		case DMG_MOVE_LEFT:
			sprite->changeAnimation(MOVE_LEFT);
			estado = MOVE_LEFT;
			break;
		case DMG_MOVE_RIGHT:
			sprite->changeAnimation(MOVE_RIGHT);
			estado = MOVE_RIGHT;
			break;
		case DMG_JUMP_LEFT:
			sprite->changeAnimation(JUMP_LEFT);
			estado = JUMP_LEFT;
			break;
		case DMG_JUMP_RIGHT:
			sprite->changeAnimation(JUMP_RIGHT);
			estado = JUMP_RIGHT;
			break;
		case DMG_CLIMB:
			sprite->changeAnimation(CLIMB);
			estado = CLIMB;
			break;
		case DMG_HIT_LEFT:
			sprite->changeAnimation(HIT_LEFT);
			estado = HIT_LEFT;
			break;
		case DMG_HIT_RIGHT:
			sprite->changeAnimation(HIT_RIGHT);
			estado = HIT_RIGHT;
			break;
		}
	}
}

bool Player::isSameAsBefore() {
	if (estado == before) return true;
	return false;
}

/*void Player::setCalaveras(Calavera* c[])
{
	int size = sizeof(c) / sizeof(c[0]);
	for (int i = 0; i < size; i++) {
		calaveras[i] = c[i];
	}
}
bool Player::hitCal(glm::ivec2 posCal) {
	if (sprite->animation() == HIT_LEFT) {

		if ( posPlayer.x = posCal.x + 1  ) {
			if (posPlayer.y >= posCal.y - 1 && posPlayer.y <= posCal.y + 1) {
				return true;
			}
		}

	}else if (sprite->animation() == HIT_RIGHT) {
		if (posPlayer.x = posCal.x - 1) {
			if (posPlayer.y >= posCal.y - 1 && posPlayer.y <= posCal.y + 1) {
				return true;
			}
		}
	}
	return false;

}*/




