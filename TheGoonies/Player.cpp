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

#define TIMER_X 45
#define TIMER_Y 45



enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB, HIT_LEFT, HIT_RIGHT, DMG_STAND_LEFT, DMG_STAND_RIGHT, DMG_MOVE_LEFT, DMG_MOVE_RIGHT, DMG_JUMP_LEFT, DMG_JUMP_RIGHT, DMG_CLIMB, DMG_HIT_LEFT, DMG_HIT_RIGHT, NUM_ANIMS
};

enum PlayerVidaExp {
	VIDA_0, VIDA_1, VIDA_2, VIDA_3, VIDA_4, VIDA_5, VIDA_6, VIDA_7, VIDA_8
};

enum PlayerExp {
	EXP_0, EXP_1, EXP_2, EXP_3, EXP_4, EXP_5, EXP_6, EXP_7, EXP_8
};
enum timerAnims {
	FULL, ALMOST_FULL, HALF, ALMOST_END
};

enum numChilds {
	ZERO_CH, ONE_CH, TWO_CH, TREE_CH, FOUR_CH, FIVE_CH, SIX_CH
};

enum powerUps {
	NONE, BLUE_HEL, YELLOW_HEL, GREEN_RAIN, GRAY_RAIN, HYPERSHOES
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	//load una imatge
	spritesheet.loadFromFile("images/spritesheetPlayerDef.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//classe sprite -> creem el sprite (tama?o del jugador (18 es un tile))
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
	iniChildsInterface(shaderProgram);
	iniPowerUpInterface(shaderProgram);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));


	
	
}

//moviment
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	vidaSprite->update(deltaTime);
	expSprite->update(deltaTime);
	timerSprite->update(deltaTime);
	childSprite->update(deltaTime);
	godSprite->update(deltaTime);
	powUp1Sprite->update(deltaTime);
	powUp2Sprite->update(deltaTime);
	powUp3Sprite->update(deltaTime);
	powUp4Sprite->update(deltaTime);
	powUp5Sprite->update(deltaTime);
	
	++timerGod;
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
		if(vidasPlayer<8)++vidasPlayer;
		expPlayer = 0;
	}
	if (vidasPlayer <= 0) {
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::END_SCENE);
		
	}
	if (flagDash) {
		soundDash = false;
		flagDash = false;
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
		if (hasHyperShoes) posPlayer.x -= 6;
		else posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 20)))
		{
			if (hasHyperShoes) posPlayer.x += 6;
			else posPlayer.x += 2;
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
		if (hasHyperShoes) posPlayer.x += 6;
		else posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 20)))
		{
			if (hasHyperShoes) posPlayer.x -= 6;
			else posPlayer.x -= 2;;
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
		//Al darle a la X, desplazamiento hacia la derecha/izq 2 tiles
	}
	//Baixar vides del jugador (k)
	else if (Game::instance().getKey(107)) {
			vidasPlayer = vidasPlayer - 1;
		}	
	//God mode con la 'G'
	else if (Game::instance().getKey(103)) {
		if (timerGod >= 60) {
			if (!godMode) {
				godMode = true;
				godSprite->changeAnimation(1);
			}
			else {
				godMode = false;
				godSprite->changeAnimation(0);
			}
			timerGod = 0;
		}
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
	if (Game::instance().getKey(120)) {

		if (canTP()) {
			if (estado == STAND_RIGHT || estado == JUMP_RIGHT || estado == MOVE_RIGHT || estado == DMG_STAND_RIGHT || estado == DMG_JUMP_RIGHT || estado == DMG_MOVE_RIGHT) {
				posPlayer.x += 72;
				bCooldownX = true;
				
			}
			else if (estado == STAND_LEFT || estado == JUMP_LEFT || estado == MOVE_LEFT || estado == DMG_STAND_LEFT || estado == DMG_JUMP_LEFT || estado == DMG_MOVE_LEFT) {
				posPlayer.x -= 72;
				bCooldownX = true;
				
			}
		}
	}
	if (bCooldownX) {
		if (timerX == 60) {
			timerSprite->changeAnimation(ALMOST_FULL);
		}
		else if (timerX == 120) {
			timerSprite->changeAnimation(HALF);
		}
		else if (timerX == 180) {
			timerSprite->changeAnimation(ALMOST_END);
		}
		else if (timerX == 240) {
			bCooldownX = false;
			timerX = 0;
			timerSprite->changeAnimation(FULL);
		}
		++timerX;

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

	if (stateChild == 5) {
		
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::END_SCENE);
	}
	
	
		
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	vidaSprite->setPosition(glm::vec2(float(330), float(10)));
	expSprite->setPosition(glm::vec2(float(330), float(30)));
	timerSprite->setPosition(glm::vec2(float(60), float(6)));
	godSprite->setPosition(glm::vec2(float(110), float(6)));
}

void Player::render()
{
	sprite->render();
	vidaSprite->render();
	expSprite->render();
	if (bCooldownX) timerSprite->render();
	childSprite->render();
	godSprite->render();
	powUp1Sprite->render();
	powUp2Sprite->render();
	powUp3Sprite->render();
	powUp4Sprite->render();
	powUp5Sprite->render();
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
	timerSprite = false;
	hasBlueHel = false;
	soundDash = false;
	flagDash = false;
	hasYellowHel = false;
	hasGreenRain = false;
	hasGrayRain = false;
	hasHyperShoes = false;
	godMode = false;
	timerGod = 0;
	
	estado = STAND_LEFT;
	vidasPlayer = INI_VIDAS; 
	expPlayer = 0;	
	
	ssheet.loadFromFile("images/VidaExpSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//classe sprite -> creem el sprite (tama?o del jugador (18 es un tile))
	vidaSprite = Sprite::createSprite(glm::ivec2(VIDAEXP_X, VIDAEXP_Y), glm::vec2(0.333f, 0.1666f), &ssheet, &shaderProgram);
	//aquest sprite te 4 animacions
		vidaSprite->setNumberAnimations(9);
	
		vidaSprite->setAnimationSpeed(VIDA_0, 8);
		vidaSprite->addKeyframe(VIDA_0, glm::vec2(0.666f, 0.664f));

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
		vidaSprite->addKeyframe(VIDA_8, glm::vec2(0.333f, 0.666f));

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
	expSprite->setPosition(glm::vec2(float(433), float(1)));

	ssheetTimer.loadFromFile("images/TimerSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	timerSprite = Sprite::createSprite(glm::ivec2(TIMER_X, TIMER_Y), glm::vec2(0.5, 0.5f), &ssheetTimer, &shaderProgram);
	//aquest sprite te 4 animacions
	timerSprite->setNumberAnimations(4);

		timerSprite->setAnimationSpeed(FULL, 8);
		timerSprite->addKeyframe(FULL, glm::vec2(0.5f, 0.5f));

		timerSprite->setAnimationSpeed(ALMOST_FULL, 8);
		timerSprite->addKeyframe(ALMOST_FULL, glm::vec2(0.f, 0.5f));

		timerSprite->setAnimationSpeed(HALF, 8);
		timerSprite->addKeyframe(HALF, glm::vec2(0.5f, 0.0f));

		timerSprite->setAnimationSpeed(ALMOST_END, 8);
		timerSprite->addKeyframe(ALMOST_END, glm::vec2(0.0f, 0.0f));
		
	timerSprite->changeAnimation(FULL);
	timerSprite->setPosition(glm::vec2(float(20), float(5)));

	godsh.loadFromFile("images/godMode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	godSprite = Sprite::createSprite(glm::ivec2(84, 21), glm::vec2(0.5, 1.f), &godsh, &shaderProgram);
	//aquest sprite te 2 animacions
	godSprite->setNumberAnimations(2);

		godSprite->setAnimationSpeed(0, 8);
		godSprite->addKeyframe(0, glm::vec2(0.5f, 0.0f));

		godSprite->setAnimationSpeed(1, 8);
		godSprite->addKeyframe(1, glm::vec2(0.f, 0.0f));
	
		godSprite->changeAnimation(0);
	godSprite->setPosition(glm::vec2(float(20), float(5)));



}

void Player::iniChildsInterface(ShaderProgram& shaderProgram) {

	stateChild = ZERO_CH;

	ssheetch.loadFromFile("images/childInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	childSprite = Sprite::createSprite(glm::ivec2(90, 36), glm::vec2((1.f/7.f), 1), &ssheetch, &shaderProgram);

	childSprite->setNumberAnimations(7);

	childSprite->setAnimationSpeed(ZERO_CH, 8);
	childSprite->addKeyframe(ZERO_CH, glm::vec2(0, 0));

	childSprite->setAnimationSpeed(ONE_CH, 8);
	childSprite->addKeyframe(ONE_CH, glm::vec2((1.f / 7.f), 0));

	childSprite->setAnimationSpeed(TWO_CH, 8);
	childSprite->addKeyframe(TWO_CH, glm::vec2((2.f / 7.f), 0));

	childSprite->setAnimationSpeed(TREE_CH, 8);
	childSprite->addKeyframe(TREE_CH, glm::vec2((3.f / 7.f), 0));

	childSprite->setAnimationSpeed(FOUR_CH, 8);
	childSprite->addKeyframe(FOUR_CH, glm::vec2((4.f / 7.f), 0));

	childSprite->setAnimationSpeed(FIVE_CH, 8);
	childSprite->addKeyframe(FIVE_CH, glm::vec2((5.f / 7.f), 0));

	childSprite->setAnimationSpeed(SIX_CH, 8);
	childSprite->addKeyframe(SIX_CH, glm::vec2((6.f / 7.f), 0));

	childSprite->changeAnimation(ZERO_CH);
	childSprite->setPosition(glm::vec2(float(450), float(415)));

}

void Player::iniPowerUpInterface(ShaderProgram& shaderProgram) 
{
	//Power-up 1
	powup1sh.loadFromFile("images/powerUpInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	powUp1Sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2((1.f / 6.f), 1), &powup1sh, &shaderProgram);

	powUp1Sprite->setNumberAnimations(6);

		powUp1Sprite->setAnimationSpeed(NONE, 8);
		powUp1Sprite->addKeyframe(NONE, glm::vec2(0, 0));

		powUp1Sprite->setAnimationSpeed(BLUE_HEL, 8);
		powUp1Sprite->addKeyframe(BLUE_HEL, glm::vec2((1.f / 6.f), 0));

		powUp1Sprite->setAnimationSpeed(YELLOW_HEL, 8);
		powUp1Sprite->addKeyframe(YELLOW_HEL, glm::vec2((2.f / 6.f), 0));

		powUp1Sprite->setAnimationSpeed(GREEN_RAIN, 8);
		powUp1Sprite->addKeyframe(GREEN_RAIN, glm::vec2((3.f / 6.f), 0));

		powUp1Sprite->setAnimationSpeed(GRAY_RAIN, 8);
		powUp1Sprite->addKeyframe(GRAY_RAIN, glm::vec2((4.f / 6.f), 0));

		powUp1Sprite->setAnimationSpeed(HYPERSHOES, 8);
		powUp1Sprite->addKeyframe(HYPERSHOES, glm::vec2((5.f / 6.f), 0));

	powUp1Sprite->changeAnimation(NONE);
	powUp1Sprite->setPosition(glm::vec2(float(40), float(415)));


	//Power-up 2
	powup2sh.loadFromFile("images/powerUpInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	powUp2Sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2((1.f / 6.f), 1), &powup2sh, &shaderProgram);

	powUp2Sprite->setNumberAnimations(6);

		powUp2Sprite->setAnimationSpeed(NONE, 8);
		powUp2Sprite->addKeyframe(NONE, glm::vec2(0, 0));

		powUp2Sprite->setAnimationSpeed(BLUE_HEL, 8);
		powUp2Sprite->addKeyframe(BLUE_HEL, glm::vec2((1.f / 6.f), 0));

		powUp2Sprite->setAnimationSpeed(YELLOW_HEL, 8);
		powUp2Sprite->addKeyframe(YELLOW_HEL, glm::vec2((2.f / 6.f), 0));

		powUp2Sprite->setAnimationSpeed(GREEN_RAIN, 8);
		powUp2Sprite->addKeyframe(GREEN_RAIN, glm::vec2((3.f / 6.f), 0));

		powUp2Sprite->setAnimationSpeed(GRAY_RAIN, 8);
		powUp2Sprite->addKeyframe(GRAY_RAIN, glm::vec2((4.f / 6.f), 0));

		powUp2Sprite->setAnimationSpeed(HYPERSHOES, 8);
		powUp2Sprite->addKeyframe(HYPERSHOES, glm::vec2((5.f / 6.f), 0));

	powUp2Sprite->changeAnimation(NONE);
	powUp2Sprite->setPosition(glm::vec2(float(80), float(415)));


	//Power-up 3
	powup3sh.loadFromFile("images/powerUpInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	powUp3Sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2((1.f / 6.f), 1), &powup3sh, &shaderProgram);

	powUp3Sprite->setNumberAnimations(6);

		powUp3Sprite->setAnimationSpeed(NONE, 8);
		powUp3Sprite->addKeyframe(NONE, glm::vec2(0, 0));

		powUp3Sprite->setAnimationSpeed(BLUE_HEL, 8);
		powUp3Sprite->addKeyframe(BLUE_HEL, glm::vec2((1.f / 6.f), 0));

		powUp3Sprite->setAnimationSpeed(YELLOW_HEL, 8);
		powUp3Sprite->addKeyframe(YELLOW_HEL, glm::vec2((2.f / 6.f), 0));

		powUp3Sprite->setAnimationSpeed(GREEN_RAIN, 8);
		powUp3Sprite->addKeyframe(GREEN_RAIN, glm::vec2((3.f / 6.f), 0));

		powUp3Sprite->setAnimationSpeed(GRAY_RAIN, 8);
		powUp3Sprite->addKeyframe(GRAY_RAIN, glm::vec2((4.f / 6.f), 0));

		powUp3Sprite->setAnimationSpeed(HYPERSHOES, 8);
		powUp3Sprite->addKeyframe(HYPERSHOES, glm::vec2((5.f / 6.f), 0));

	powUp3Sprite->changeAnimation(NONE);
	powUp3Sprite->setPosition(glm::vec2(float(120), float(415)));


	//Power-up 4
	powup4sh.loadFromFile("images/powerUpInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	powUp4Sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2((1.f / 6.f), 1), &powup4sh, &shaderProgram);

	powUp4Sprite->setNumberAnimations(6);

		powUp4Sprite->setAnimationSpeed(NONE, 8);
		powUp4Sprite->addKeyframe(NONE, glm::vec2(0, 0));

		powUp4Sprite->setAnimationSpeed(BLUE_HEL, 8);
		powUp4Sprite->addKeyframe(BLUE_HEL, glm::vec2((1.f / 6.f), 0));

		powUp4Sprite->setAnimationSpeed(YELLOW_HEL, 8);
		powUp4Sprite->addKeyframe(YELLOW_HEL, glm::vec2((2.f / 6.f), 0));

		powUp4Sprite->setAnimationSpeed(GREEN_RAIN, 8);
		powUp4Sprite->addKeyframe(GREEN_RAIN, glm::vec2((3.f / 6.f), 0));

		powUp4Sprite->setAnimationSpeed(GRAY_RAIN, 8);
		powUp4Sprite->addKeyframe(GRAY_RAIN, glm::vec2((4.f / 6.f), 0));

		powUp4Sprite->setAnimationSpeed(HYPERSHOES, 8);
		powUp4Sprite->addKeyframe(HYPERSHOES, glm::vec2((5.f / 6.f), 0));

	powUp4Sprite->changeAnimation(NONE);
	powUp4Sprite->setPosition(glm::vec2(float(160), float(415)));


	//Power-up 5
	powup5sh.loadFromFile("images/powerUpInterface.png", TEXTURE_PIXEL_FORMAT_RGBA);

	powUp5Sprite = Sprite::createSprite(glm::ivec2(36, 36), glm::vec2((1.f / 6.f), 1), &powup5sh, &shaderProgram);

	powUp5Sprite->setNumberAnimations(6);

		powUp5Sprite->setAnimationSpeed(NONE, 8);
		powUp5Sprite->addKeyframe(NONE, glm::vec2(0, 0));

		powUp5Sprite->setAnimationSpeed(BLUE_HEL, 8);
		powUp5Sprite->addKeyframe(BLUE_HEL, glm::vec2((1.f / 6.f), 0));

		powUp5Sprite->setAnimationSpeed(YELLOW_HEL, 8);
		powUp5Sprite->addKeyframe(YELLOW_HEL, glm::vec2((2.f / 6.f), 0));

		powUp5Sprite->setAnimationSpeed(GREEN_RAIN, 8);
		powUp5Sprite->addKeyframe(GREEN_RAIN, glm::vec2((3.f / 6.f), 0));

		powUp5Sprite->setAnimationSpeed(GRAY_RAIN, 8);
		powUp5Sprite->addKeyframe(GRAY_RAIN, glm::vec2((4.f / 6.f), 0));

		powUp5Sprite->setAnimationSpeed(HYPERSHOES, 8);
		powUp5Sprite->addKeyframe(HYPERSHOES, glm::vec2((5.f / 6.f), 0));

	powUp5Sprite->changeAnimation(NONE);
	powUp5Sprite->setPosition(glm::vec2(float(200), float(415)));

}

glm::ivec2 Player::getPosPlayer() {
	return posPlayer;
}
void Player::calHit() {
	//ha dado a una calavera, hay que augmentar la exp
	++expPlayer;
	if (expPlayer >= 8) {
		if(vidasPlayer <8)++vidasPlayer;
		expPlayer = 0;
		
	vidaSprite->changeAnimation(vidasPlayer);
	}
	expSprite->changeAnimation(expPlayer);
}

void Player::hitByEnemy() {
	if (!isHitted && !godMode) {
		--vidasPlayer;
		isHitted = true;
		setHitAnimation();
		timerHit = 0;
		vidaSprite->changeAnimation(vidasPlayer);
	}
}

void Player::incrementChild()
{
	++stateChild;
	childSprite->changeAnimation(stateChild);
}

int Player::getHealth()
{
	return vidasPlayer;
}
int Player::getExp() {
	return expPlayer;
}

bool Player::isHittedPlayer()
{
	return isHitted || godMode;
}
bool Player::soundXDash()
{
	return soundDash;
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

bool Player::canTP()
{
	if (!bCooldownX) {
		if (estado == STAND_RIGHT || estado == JUMP_RIGHT || estado == MOVE_RIGHT || estado == DMG_STAND_RIGHT || estado == DMG_JUMP_RIGHT || estado == DMG_MOVE_RIGHT) {
			if (posPlayer.x + 72 <= 532 ) {
				soundDash = true;
				flagDash = true;
				return true;
			}
		}
		else if (estado == STAND_LEFT || estado == JUMP_LEFT || estado == MOVE_LEFT || estado == DMG_STAND_LEFT || estado == DMG_JUMP_LEFT || estado == DMG_MOVE_LEFT) {
			if (posPlayer.x - 72 >= 60) {
				soundDash = true;
				flagDash = true;
				return true;
			}
		}
		
	}
	return false;
}

void Player::getBlueHelmet() 
{
	hasBlueHel = true;
	if (powUp1Sprite->animation() == NONE) powUp1Sprite->changeAnimation(BLUE_HEL);
	else if (powUp2Sprite->animation() == NONE) powUp2Sprite->changeAnimation(BLUE_HEL);
	else if (powUp3Sprite->animation() == NONE) powUp3Sprite->changeAnimation(BLUE_HEL);
	else if (powUp4Sprite->animation() == NONE) powUp4Sprite->changeAnimation(BLUE_HEL);
	else if (powUp5Sprite->animation() == NONE) powUp5Sprite->changeAnimation(BLUE_HEL);
}

void Player::getYellowHelmet()
{
	hasYellowHel = true;
	if (powUp1Sprite->animation() == NONE) powUp1Sprite->changeAnimation(YELLOW_HEL);
	else if (powUp2Sprite->animation() == NONE) powUp2Sprite->changeAnimation(YELLOW_HEL);
	else if (powUp3Sprite->animation() == NONE) powUp3Sprite->changeAnimation(YELLOW_HEL);
	else if (powUp4Sprite->animation() == NONE) powUp4Sprite->changeAnimation(YELLOW_HEL);
	else if (powUp5Sprite->animation() == NONE) powUp5Sprite->changeAnimation(YELLOW_HEL);
}

void Player::getGreenRaincoat()
{
	hasGreenRain = true;
	if (powUp1Sprite->animation() == NONE) powUp1Sprite->changeAnimation(GREEN_RAIN);
	else if (powUp2Sprite->animation() == NONE) powUp2Sprite->changeAnimation(GREEN_RAIN);
	else if (powUp3Sprite->animation() == NONE) powUp3Sprite->changeAnimation(GREEN_RAIN);
	else if (powUp4Sprite->animation() == NONE) powUp4Sprite->changeAnimation(GREEN_RAIN);
	else if (powUp5Sprite->animation() == NONE) powUp5Sprite->changeAnimation(GREEN_RAIN);
}

void Player::getGrayRaincoat()
{
	hasGrayRain = true;
	if (powUp1Sprite->animation() == NONE) powUp1Sprite->changeAnimation(GRAY_RAIN);
	else if (powUp2Sprite->animation() == NONE) powUp2Sprite->changeAnimation(GRAY_RAIN);
	else if (powUp3Sprite->animation() == NONE) powUp3Sprite->changeAnimation(GRAY_RAIN);
	else if (powUp4Sprite->animation() == NONE) powUp4Sprite->changeAnimation(GRAY_RAIN);
	else if (powUp5Sprite->animation() == NONE) powUp5Sprite->changeAnimation(GRAY_RAIN);
}

void Player::getHyperShoes()
{
	hasHyperShoes = true;
	if (powUp1Sprite->animation() == NONE) powUp1Sprite->changeAnimation(HYPERSHOES);
	else if (powUp2Sprite->animation() == NONE) powUp2Sprite->changeAnimation(HYPERSHOES);
	else if (powUp3Sprite->animation() == NONE) powUp3Sprite->changeAnimation(HYPERSHOES);
	else if (powUp4Sprite->animation() == NONE) powUp4Sprite->changeAnimation(HYPERSHOES);
	else if (powUp5Sprite->animation() == NONE) powUp5Sprite->changeAnimation(HYPERSHOES);
}

bool Player::hasBlueHelmet()
{
	return hasBlueHel;
}

bool Player::hasYellowHelmet()
{
	return hasYellowHel;
}

bool Player::hasGreenRaincoat()
{
	return hasGreenRain;
}

bool Player::hasGrayRaincoat()
{
	return hasGrayRain;
}

bool Player::hasHypershoes()
{
	return hasHyperShoes;
}
int Player::getChilds() {
	return stateChild;
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




