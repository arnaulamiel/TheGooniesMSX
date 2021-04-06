#include "playScene.h"
#include "SceneManager.h"
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

//Donde inicia el mapa
#define SCREEN_X 32
#define SCREEN_Y 50

#define BACKGROUND_X 650
#define BACKGROUND_Y 480

// GLUT SPACEBAR code
#define SPACEBAR 32
// GLUT ESC code
#define ESCAPE 27

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

#define INIT_CAL_X_TILES 15
#define INIT_CAL_Y_TILES 15

#define INIT_BAT_X_TILES 10
#define INIT_BAT_Y_TILES 13
//prueba

/* @brief Static member function declaration */
Scene* playScene::create()
{	
	playScene* scene = new playScene();
	return scene;
}

/* @brief Default constructor */
playScene::playScene() : Scene()
{
	this->sceneID = INIT_SCENE;
	map = NULL; 
	player = NULL;
	cal = NULL;
	room = 1;

	c[1] = { cal };
}

/* @brief Default destructor */
playScene::~playScene() {
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (cal != NULL)
		delete cal;
}

/* @brief Overrided init function
 *
 * This function calls to initialize the shaders, and also
 * loads the logo and sets the camera to the perfect position
 * for the scene.
 */
void playScene::init(void)
{
	hasKey = false;
	bgota = false;
	byeRoca = false;
	rocaDown = false;
	timerGota = 0;
	timerRoca = 0;
	timerFuego = 0;
	doorOpen = false;
	hasChild = false;
	timesFireAnim = 0;
	hasSound = false;
	inPortal = false;
	waitToEnd = 0;
	scene = 0;
	
	
	changeMusic("../../../libs/irrKlang-1.6.0/media/playGoonies.ogg");

	hasBlueHel = false;
	hasYellowHel = false;
	hasGreenRain = false;
	hasGrayRain = false;
	hasHyperShoes = false;

	initShaders();

	logoTexture.loadFromFile("images/black.png", TEXTURE_PIXEL_FORMAT_RGBA);
	logoTexture.setMinFilter(GL_NEAREST);
	logoTexture.setMagFilter(GL_NEAREST);

	/*vidaexpTexture.loadFromFile("images/VidaExpSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	vidaexpTexture.setMinFilter(GL_NEAREST);
	vidaexpTexture.setMagFilter(GL_NEAREST);*/

	//TileMap
	initMaps(scene);
	updateRoom();
	loadRoomObjects();
	updateActualObjects();

	logo = Sprite::createSprite(glm::ivec2(BACKGROUND_X, BACKGROUND_Y), glm::vec2(1.f, 1.f), &logoTexture, &texProgram);
	logo->setPosition(glm::vec2(0, 0));

	/*vidaexp = Sprite::createSprite(glm::ivec2(VIDAEXP_X, VIDAEXP_Y), glm::vec2(1.f, 1.f), &vidaexpTexture, &texProgram);
	vidaexp->setPosition(glm::vec2(433, 3));*/

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	
	
	//Calavera
	cal = new Calavera();
	cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
	cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
	cal->setPlayer(player);
	cal->setTileMap(map);

	//Bat
	bat = new Bat();
	bat->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bat->setPosition(glm::vec2(INIT_BAT_X_TILES * map->getTileSize(), INIT_BAT_Y_TILES * map->getTileSize()));
	bat->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
	bat->setPlayer(player);
	bat->setTileMap(map);

	

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

/* @brief Overrided update function
 *
 * This function updates the scene.
 * The count variable is used as a timer so the scene duration is accurate.
 * This scene can be jumped using pressing the spacebar.
 *
 * @param deltaTime
 */
void playScene::update(int deltaTime)
{
	

	if (Game::instance().getKey(ESCAPE)) {
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::END_SCENE);
	}
	currentTime += deltaTime;
	if (hitEnem(cal)) {
		if (!cal->isDead()) {
			createSound("../../../libs/irrKlang-1.6.0/media/hit.mp3",false);
			player->calHit();
			cal->killedCal();
		}
	}
	if (hitEnem(bat)) {
		if (!bat->isDead()) {
			createSound("../../../libs/irrKlang-1.6.0/media/hit.mp3", false);
			player->calHit();
			bat->killedBat();
		}
	}
	if (player->soundXDash()) {
		createSound("../../../libs/irrKlang-1.6.0/media/dash.wav",false);
	}

	for (int i = 0; i < actualRoomObjects.size(); i++) {
		if (!hasKey) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == KEY) {
				
				Object* key = actualRoomObjects[i];
				if (getObject(key)) {
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					hasKey = true;
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == GOTA) {
			++timerGota;
			if (timerGota == 20) {
				Object* gota = actualRoomObjects[i];
				int spriteanim = gota->getSpriteAnimation();
				if (spriteanim < 3) { 
					switch (spriteanim) { 
					case INI :				
						
						gota->changeSpriteAnimation(spriteanim + 1);
						break;
					case DOWN_1:
						gota->changeSpriteAnimation(spriteanim + 1);
						break;
					case DOWN_2:
						calculateDownObstaculo(gota);
						//hasSound = true;
						break;
					case SPLASH:
						
						gota->changeSpriteAnimation(SPLASH);
						break;
					}
				}
				else{
					glm::ivec2 ini = gota->getIniPosition();

					gota->setObjectPosition(ini);
					gota->init(texProgram);
					gota->changeSpriteAnimation(0); 
					bgota = false;
					
				}
				timerGota = 0;

			}
		}
		if (hasKey) {
			if (!doorOpen) {
				if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == LOCK) {
					Object* lock = actualRoomObjects[i];
					if (getObject(lock)) {
						doorOpen = true;
						actualRoomObjects[i]->destroyObject();
						actualRoomObjects[i] = nullptr;
					}
				}
			}
			if (doorOpen) {
				if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == DOOR_CHILD) {
					Object* doorChild = actualRoomObjects[i];
					if (!hasChild && doorChild->getSpriteAnimation() == CLOSED) {
						doorChild->changeSpriteAnimation(OPEN_CHILD);
					}
					else if (!hasChild && getObjDoor(doorChild)) {
						hasChild = true;
						createSound("../../../libs/irrKlang-1.6.0/media/rescuedGoonie.ogg", false);
						player->incrementChild();
						doorChild->changeSpriteAnimation(OPEN_EMPTY);
					}

				}
			}
		}
		if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == ROCA) {
			++timerRoca;
			if (timerRoca == 20) {
				Object* roca = actualRoomObjects[i];
				int spriteanim = roca->getSpriteAnimation();
				
				switch (spriteanim) {
				case R_INI:
					
					calculateDownObstaculo(roca);
					if (rocaDown) {
							roca->changeSpriteAnimation(spriteanim + 1);
					}
					break;
				
				case R_DOWN:
					calculateDownObstaculo(roca);
					break;
				case R_SPLASH:
					if (!byeRoca) {
						roca->changeSpriteAnimation(SPLASH);
						byeRoca = true;
					}
					else {
						roca->destroyObject();
					}
					break;

				}
				
				timerRoca = 0;

			}
		}
		if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == FIRE) {
			++timerFuego;
			
				Object* fire = actualRoomObjects[i];
				int spriteanim = fire->getSpriteAnimation();

				switch (spriteanim) {
				case F_INI:		
					if (timerFuego == 8 && timesFireAnim <= 13) {
						if (fireHitsPlayer(fire)) {
							hitPlayer();
						}
						fire->changeSpriteAnimation(FIRE1);
						timerFuego = 0;
					}
					else if(timerFuego >8) {
						
						if (timerFuego == 200) {
							fire->init(texProgram);
							fire->changeSpriteAnimation(F_INI);
							timerFuego = 0;
							timesFireAnim = 0;
						}
					}					
					break;
				case FIRE1:
					if (timerFuego == 8 ) {
						if (fireHitsPlayer(fire)) {
							hitPlayer();
						}
						fire->changeSpriteAnimation(FIRE2);
						timerFuego = 0;
						++timesFireAnim;
					}
					break;
				case FIRE2:	
					if (timerFuego == 8 && timesFireAnim <=13) {
						if (fireHitsPlayer(fire)) {
							hitPlayer();
						}
						fire->changeSpriteAnimation(FIRE1);
						timerFuego = 0;
						++timesFireAnim;
					}
					else if(timerFuego >8) {
						fire->changeSpriteAnimation(F_INI);
					}
					
					break;
				}
		}
		if (!hasBlueHel) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == BLUE_HEL) {
				Object* blueHel = actualRoomObjects[i];
				if (getObject(blueHel)) {
					hasBlueHel = true;
					player->getBlueHelmet();
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (!hasYellowHel) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == YELLOW_HEL) {
				Object* yellowHel = actualRoomObjects[i];
				if (getObject(yellowHel)) {
					hasYellowHel = true;
					player->getYellowHelmet();
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (!hasGreenRain) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == GREEN_RAIN) {
				Object* greenRain = actualRoomObjects[i];
				if (getObject(greenRain)) {
					hasGreenRain = true;
					player->getGreenRaincoat();
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (!hasGrayRain) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == GRAY_RAIN) {
				Object* grayRain = actualRoomObjects[i];
				if (getObject(grayRain)) {
					hasGrayRain = true;
					player->getGrayRaincoat();
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (!hasHyperShoes) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == HYPERSHOES) {
				Object* hyperShoes = actualRoomObjects[i];
				if (getObject(hyperShoes)) {
					hasHyperShoes = true;
					createSound("../../../libs/irrKlang-1.6.0/media/getKey.wav", false);
					
					player->getHyperShoes();
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
		if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == PORTAL) {
			Object* portal = actualRoomObjects[i];
			if (waitToEnd > 0)++waitToEnd;
			//Si le da a la flecha hacia abajo entra en el portal
			if (playerinPortal(portal)) {
				if (!inPortal) {
					inPortal = true;
					createSound("../../../libs/irrKlang-1.6.0/media/alterntiveScene.ogg", false);
					deleteEngine();
					++waitToEnd;

				}
			}
			else if (inPortal) {
				if (waitToEnd == 50) {
					waitToEnd = 0;
					deleteSound();
					++scene;
					newScene();

				}
			}
		}
	}
	

	if (player->getHealth() == 0) deleteEngine();
	player->update(deltaTime);
	cal->update(deltaTime);
	bat->update(deltaTime);
	
	updateElementsScene();
	
	
}

/* @brief Overrided render function
 *
 * This function renders the scene.
 */
void playScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	logo->render();
	//vidaexp->render();
	map->render();

	for (int i = 0; i < actualRoomObjects.size(); i++)
	{
		if (actualRoomObjects[i] != nullptr)
			actualRoomObjects[i]->render();
	}

	player->render();
	cal->render();
	bat->render();

	
}

/* @brief Overrided function used to finalize scenes*/
void playScene::fin() {}

void playScene::initMaps(int scene) {
	switch (scene) {
	case 0:
		
		mapIni = TileMap::createTileMap("levels/level15.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);		
		mapIni2 = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);		
		mapIni3 = TileMap::createTileMap("levels/level03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		
		break;
	case 1:

		mapIni = TileMap::createTileMap("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);			
		mapIni2 = TileMap::createTileMap("levels/level05.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);			
		mapIni3 = TileMap::createTileMap("levels/level06.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);	

		break;
	case 2:

		mapIni = TileMap::createTileMap("levels/level07.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni2 = TileMap::createTileMap("levels/level08.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni3 = TileMap::createTileMap("levels/level09.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

		break;
	case 3:

		mapIni = TileMap::createTileMap("levels/level10.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni2 = TileMap::createTileMap("levels/level11.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni3 = TileMap::createTileMap("levels/level12.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

		break;
	case 4:

		mapIni = TileMap::createTileMap("levels/level13.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni2 = TileMap::createTileMap("levels/level14.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapIni3 = TileMap::createTileMap("levels/level15.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

		break;
	}
	
	
	

}
void playScene::updateRoom() {
	if (room == 1 || room == 4 || room == 7 || room == 10 || room == 13) {
		map = mapIni;
	}
	else if (room == 2 || room == 5 || room == 6 || room == 11 || room == 14) {
		map = mapIni2;
	}
	else if (room == 3 || room == 6 || room == 7 || room == 12 || room == 15) {
		map = mapIni3;
	}

}

void playScene::updateScene() {

	if (scene == 0) {
		initMaps(scene);
	}
	else if (scene == 1) {
		initMaps(scene);
	}
	else if (scene == 2) {
		initMaps(scene);
	}
	else if (scene == 3) {
		initMaps(scene);
	}
	else if (scene == 4) {
		initMaps(scene);
	}
}

void playScene::updateElementsScene()
{
	switch (scene) {
	case 0:
		if (room == 1 && map->isMapLimitRight(player->getPosPlayer())) {
			saveActualObjects();
			++room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2(2 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
			player->setTileMap(map);

			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);


		}
		else if (room == 2) {
			if (map->isMapLimitRight(player->getPosPlayer())) {
				saveActualObjects();
				++room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2(2 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
				player->setTileMap(map);

				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
			else if (map->isMapLimitLeft(player->getPosPlayer())) {
				saveActualObjects();
				--room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2(30 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
				player->setTileMap(map);
				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
		}
		else if (room == 3 && map->isMapLimitLeft(player->getPosPlayer())) {
			saveActualObjects();
			--room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2(30 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
			player->setTileMap(map);
			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);
		}
		break;
	case 1:
		if (room == 1 && map->isMapLimitRight(player->getPosPlayer())) {
			saveActualObjects();
			++room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2(2 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
			player->setTileMap(map);

			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);


		}
		else if (room == 2) {
			if (map->isMapLimitTop(player->getPosPlayer())) {
				saveActualObjects();
				++room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2((player->getPosPlayer().x) / 18 * map->getTileSize(), 22 * map->getTileSize()));
				player->setTileMap(map);

				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
			else if (map->isMapLimitLeft(player->getPosPlayer())) {
				saveActualObjects();
				--room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2(30 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
				player->setTileMap(map);
				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
		}
		else if (room == 3 && map->isMapLimitDown(player->getPosPlayer())) {
			saveActualObjects();
			--room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2((player->getPosPlayer().x) / 18 * map->getTileSize(), 2 * map->getTileSize()));
			player->setTileMap(map);
			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);
		}
		break;
	case 2:
		if (room == 1 && map->isMapLimitRight(player->getPosPlayer())) {
			saveActualObjects();
			++room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2((map->getMapSize().x -1) * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
			player->setTileMap(map);

			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);


		}
		else if (room == 2) {
			if (map->isMapLimitTop(player->getPosPlayer())) {
				saveActualObjects();
				++room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2((player->getPosPlayer().x) / 18 * map->getTileSize(), 22 * map->getTileSize()));
				player->setTileMap(map);

				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
			else if (map->isMapLimitLeft(player->getPosPlayer())) {
				saveActualObjects();
				--room;
				updateRoom();
				updateActualObjects();
				player->setPosition(glm::vec2(30 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
				player->setTileMap(map);
				cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
				cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
				cal->setPlayer(player);
				cal->setTileMap(map);
			}
		}
		else if (room == 3 && map->isMapLimitDown(player->getPosPlayer())) {
			saveActualObjects();
			--room;
			updateRoom();
			updateActualObjects();
			player->setPosition(glm::vec2((player->getPosPlayer().x) / 18 * map->getTileSize(), 2 * map->getTileSize()));
			player->setTileMap(map);
			cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
			cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
			cal->setPlayer(player);
			cal->setTileMap(map);


		}
		break;
		
	}
}

void playScene::loadRoomObjects() 
{
	//if (objectsRoom1.size() != 0) objectsRoom1.erase(objectsRoom1.begin(), objectsRoom1.end());

	/*switch (scene) { 
	case 0:
		switch (room) { 
		case 1:
			loadSingleRoomObjects("objects/Objects11.txt", objectsRoom1); 
			break;
		case 2:
			loadSingleRoomObjects("objects/Objects12.txt", objectsRoom1);
			break;
		case 3:
			loadSingleRoomObjects("objects/Objects13.txt", objectsRoom1);
			break;
		}
		break;
	case 1:
		loadSingleRoomObjects("objects/Objects12.txt", objectsRoom1);
		break;
	}*/

	loadSingleRoomObjects("objects/Objects1_1.txt", objectsRoom1_1);
	loadSingleRoomObjects("objects/Objects1_2.txt", objectsRoom1_2);
	loadSingleRoomObjects("objects/Objects1_3.txt", objectsRoom1_3);
	/*loadSingleRoomObjects("objects/Objects2_1.txt", objectsRoom2_1);
	loadSingleRoomObjects("objects/Objects2_2.txt", objectsRoom2_2);
	loadSingleRoomObjects("objects/Objects2_3.txt", objectsRoom2_3);
	loadSingleRoomObjects("objects/Objects3_1.txt", objectsRoom3_1);
	loadSingleRoomObjects("objects/Objects3_2.txt", objectsRoom3_2);
	loadSingleRoomObjects("objects/Objects3_3.txt", objectsRoom3_3);
	loadSingleRoomObjects("objects/Objects4_1.txt", objectsRoom4_1);
	loadSingleRoomObjects("objects/Objects4_2.txt", objectsRoom4_2);
	loadSingleRoomObjects("objects/Objects4_3.txt", objectsRoom4_3);
	loadSingleRoomObjects("objects/Objects5_1.txt", objectsRoom5_1);
	loadSingleRoomObjects("objects/Objects5_2.txt", objectsRoom5_2);
	loadSingleRoomObjects("objects/Objects5_3.txt", objectsRoom5_3);*/

}

bool playScene::loadSingleRoomObjects(string levelFile, vector<Object*>& objectsRoom)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	int mapSizeX, mapSizeY;
	int tileSize;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 9, "OBJECTMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSizeX >> mapSizeY;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;

	for (int j = 0; j < mapSizeY; j++)
	{
		for (int i = 0; i < mapSizeX; i++)
		{
			fin >> tile;

			if (tile != 0)
			{
				glm::ivec2 position = glm::ivec2(18 * i + SCREEN_X, 18 * j + SCREEN_Y - 9);
				//glm::ivec2 position = glm::ivec2(tileSize * i, tileSize * j );
				if (tile == 1) {
					Object* key = new Object(KEY, position, glm::vec2(18, 18));
					key->init(texProgram);
					objectsRoom.push_back(key);
				}
				else if (tile == 2) {
					Object* doorChild = new Object(DOOR_CHILD, position, glm::vec2(54, 54));
					doorChild->init(texProgram);
					objectsRoom.push_back(doorChild);
				}
				else if (tile == 3) {
					Object* lock = new Object(LOCK, position, glm::vec2(18, 18));
					lock->init(texProgram);
					objectsRoom.push_back(lock);
				}
				else if (tile == 4) {
					Object* gota = new Object(GOTA, position, glm::vec2(18, 18));
					gota->init(texProgram);
					gota->setIniPosition(position);
					gota->setPatrolPoints(360);
					objectsRoom.push_back(gota);
				}else if (tile == 5) {
					Object* roca = new Object(ROCA, position, glm::vec2(18, 18));
					roca->init(texProgram);
					roca->setIniPosition(position);
					roca->setPatrolPoints(360);
					objectsRoom.push_back(roca);
				}
				else if (tile == 6) {
					Object* fire = new Object(FIRE, position, glm::vec2(18, 42));
					fire->init(texProgram);
					fire->setIniPosition(position);
					objectsRoom.push_back(fire);
				}
				else if (tile == 7) {
					Object* portal = new Object(PORTAL, position, glm::vec2(54, 54));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
				else if (tile == 8) {
					Object* portal = new Object(BLUE_HEL, position, glm::vec2(18, 18));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
				else if (tile == 9) {
					Object* portal = new Object(YELLOW_HEL, position, glm::vec2(18, 18));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
				else if (tile == 10) {
					Object* portal = new Object(GREEN_RAIN, position, glm::vec2(18, 18));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
				else if (tile == 11) {
					Object* portal = new Object(GRAY_RAIN, position, glm::vec2(18, 18));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
				else if (tile == 12) {
					Object* portal = new Object(HYPERSHOES, position, glm::vec2(18, 18));
					portal->init(texProgram);
					objectsRoom.push_back(portal);
				}
			}
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void playScene::updateActualObjects()
{
	if (room == 1) actualRoomObjects = objectsRoom1_1;
	else if (room == 2) actualRoomObjects = objectsRoom1_2;
	else if (room == 3) actualRoomObjects = objectsRoom1_3;
	else if (room == 4) actualRoomObjects = objectsRoom2_1;
	else if (room == 5) actualRoomObjects = objectsRoom2_2;
	else if (room == 6) actualRoomObjects = objectsRoom2_3;
	else if (room == 7) actualRoomObjects = objectsRoom3_1;
	else if (room == 8) actualRoomObjects = objectsRoom3_2;
	else if (room == 9) actualRoomObjects = objectsRoom3_3;
	else if (room == 10) actualRoomObjects = objectsRoom4_1;
	else if (room == 11) actualRoomObjects = objectsRoom4_2;
	else if (room == 12) actualRoomObjects = objectsRoom4_3;
	else if (room == 13) actualRoomObjects = objectsRoom5_1;
	else if (room == 14) actualRoomObjects = objectsRoom5_2;
	else if (room == 15) actualRoomObjects = objectsRoom5_3;
}

void playScene::saveActualObjects() 
{
	if (room == 1) objectsRoom1_1 = actualRoomObjects;
	else if (room == 2) objectsRoom1_2 = actualRoomObjects;
	else if (room == 3) objectsRoom1_3 = actualRoomObjects;
	else if (room == 4) objectsRoom2_1 = actualRoomObjects;
	else if (room == 5) objectsRoom2_2 = actualRoomObjects;
	else if (room == 6) objectsRoom2_3 = actualRoomObjects;
	else if (room == 7) objectsRoom3_1 = actualRoomObjects;
	else if (room == 8) objectsRoom3_2 = actualRoomObjects;
	else if (room == 9) objectsRoom3_3 = actualRoomObjects;
	else if (room == 10) objectsRoom4_1 = actualRoomObjects;
	else if (room == 11) objectsRoom4_2 = actualRoomObjects;
	else if (room == 12) objectsRoom4_3 = actualRoomObjects;
	else if (room == 13) objectsRoom5_1 = actualRoomObjects;
	else if (room == 14) objectsRoom5_2 = actualRoomObjects;
	else if (room == 15) objectsRoom5_3 = actualRoomObjects;
}

void playScene::calculateDownObstaculo(Object* obst) {
	//Entra cuando esta en la animacion de bajar
	glm::ivec2 pos = obst->getObjectPosition();
	if (obst->getObjectType() == GOTA) {
		if (!gotaHitsPlayer(obst) && (pos.y < obst->getPatrolPoints()) && !bgota) {


			if (pos.y > obst->getPatrolPoints()) {
				pos.y += (obst->getPatrolPoints() - pos.y);
				obst->setObjectPosition(pos);
			}
			else {
				pos.y += 25;
				obst->setObjectPosition(pos);
			}


		}
		//Si la gota toca el terra/enemic i no esta en SPLASH
		else if (!bgota) {

			if (!player->hasGrayRaincoat() && gotaHitsPlayer(obst)) hitPlayer();
			obst->setObjectPosition(pos);
			bgota = true;
			
			createSound("../../../libs/irrKlang-1.6.0/media/gota.mp3", false);
			sound->setSoundVolume(0.2);
			obst->changeSpriteAnimation(SPLASH);
		}
	}
	else if (obst->getObjectType() == ROCA) {

		if (rocaDown || ((player->getPosPlayer().x / map->getTileSize()) >= (obst->getObjectPosition().x / map->getTileSize()) - 5 && (player->getPosPlayer().x / map->getTileSize()) <= (obst->getObjectPosition().x / map->getTileSize()) + 3)) {
			rocaDown = true;
			if (!gotaHitsPlayer(obst) && (pos.y < obst->getPatrolPoints())) {


				if (pos.y > obst->getPatrolPoints()) {
					pos.y += (obst->getPatrolPoints() - pos.y);
					obst->setObjectPosition(pos);
				}
				else {
					pos.y += 25;
					obst->setObjectPosition(pos);
				}

			}
			else {

				if (!player->hasYellowHelmet() && gotaHitsPlayer(obst)) hitPlayer();
				obst->setObjectPosition(pos);
				createSound("../../../libs/irrKlang-1.6.0/media/fallground.wav", false);
				obst->changeSpriteAnimation(R_SPLASH);
			}
		}
	}	
	
}

bool playScene::playerinPortal(Object* p)
{
	int tileSize = map->getTileSize();
	glm::vec2 posPlayer = player->getPosPlayer();
	glm::vec2 posPort = p->getObjectPosition();
	
		if (posPlayer.x / tileSize >= (posPort.x / tileSize) - 2 && posPlayer.x / tileSize <= (posPort.x / tileSize)) {
			if (posPlayer.y / tileSize >= (posPort.y / tileSize) - 3 && posPlayer.y / tileSize <= (posPort.y / tileSize) + 4) {
				if(Game::instance().getSpecialKey(GLUT_KEY_DOWN))return true;
			}
		
	}
		return false;
}

void playScene::newScene()
{
	hasKey = false;
	bgota = false;
	byeRoca = false;
	rocaDown = false;
	timerGota = 0;
	timerRoca = 0;
	timerFuego = 0;
	doorOpen = false;
	hasChild = false;
	timesFireAnim = 0;
	hasSound = false;
	inPortal = false;
	waitToEnd = 0;
	room = 1;
	

	changeMusic("../../../libs/irrKlang-1.6.0/media/playGooniesAlternative.ogg");
	
	//se ha invocado que se tiene que ir a la nueva escena

	updateScene();
	updateRoom();
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	

}

bool playScene::hitEnem(Enemies* c ) {
	glm::vec2 posPlayer = player->getPosPlayer();
	glm::vec2 posCal = c->getPosition();
	int tilesize = map->getTileSize();
	if (!c->isDead()) {
		//Si esta pegando izq
		if (player->getState() == 7) {

			if (posPlayer.x / tilesize <= (posCal.x / tilesize) + 2 && posPlayer.x / tilesize >= (posCal.x / tilesize)) {
				if (posPlayer.y / tilesize >= (posCal.y / tilesize) - 1 && posPlayer.y / tilesize <= (posCal.y / tilesize) + 1) {
					if (player->getExp() >= 8) {
						createSound("../../../libs/irrKlang-1.6.0/media/hpUp.wav", false);
					}
					return true;
					
				}
			}

		}//Si esta pegando der
		else if (player->getState() == 8) {
			if (posPlayer.x / tilesize >= (posCal.x / tilesize) - 2 && posPlayer.x / tilesize <= (posCal.x / tilesize)) {
				if (posPlayer.y / tilesize >= (posCal.y / tilesize) - 1 && posPlayer.y / tilesize <= (posCal.y / tilesize) + 1) {
					if (player->getExp() >= 8) {
						createSound("../../../libs/irrKlang-1.6.0/media/hpUp.wav", false);
					}
					return true;

				}
			}
		}
	}
	return false;

}

bool playScene::getObject(Object* k) {
	glm::vec2 posKey = k->getObjectPosition();
	glm::vec2 posPlayer = player->getPosPlayer();
	int tileSize = map->getTileSize();
	if (posPlayer.x / tileSize >= (posKey.x / tileSize) - 4 && posPlayer.x / tileSize <= (posKey.x / tileSize) + 2) {
		if (posPlayer.y / tileSize >= (posKey.y / tileSize) - 4 && posPlayer.y / tileSize <= (posKey.y / tileSize) + 2) {
			return true;
		}
	}
	return false;
}

bool playScene::getObjDoor(Object* door) {
	glm::vec2 posDoor = door->getObjectPosition();
	glm::vec2 posPlayer = player->getPosPlayer();
	int tileSize = map->getTileSize();
	if (posPlayer.x / tileSize >= (posDoor.x / tileSize) - 3 && posPlayer.x / tileSize <= (posDoor.x / tileSize) + 4) {
		if (posPlayer.y / tileSize >= (posDoor.y / tileSize) - 3 && posPlayer.y / tileSize <= (posDoor.y / tileSize) + 4) {
			return true;
		}
	}
	return false;
}

bool playScene::gotaHitsPlayer(Object* g)
{
	glm::vec2 posGota = g->getObjectPosition();
	glm::vec2 posPlayer = player->getPosPlayer();
	//Si la gota coincide con el player return true
	int tileSize = map->getTileSize();
	if (posPlayer.x / tileSize >= (posGota.x / tileSize) - 3 && posPlayer.x / tileSize <= (posGota.x / tileSize) -1) {
		if (posPlayer.y / tileSize >= (posGota.y / tileSize) -5 && posPlayer.y / tileSize <= (posGota.y / tileSize)-1  ) {
			return true;
			
		}
	}
	return false;
}

bool playScene::fireHitsPlayer(Object* f)
{
	if (!player->hasGreenRaincoat()) {
		glm::vec2 posFire = f->getObjectPosition();
		glm::vec2 posPlayer = player->getPosPlayer();
		int tileSize = map->getTileSize();
		if (posPlayer.x / tileSize >= (posFire.x / tileSize) - 3 && posPlayer.x / tileSize <= (posFire.x / tileSize) - 1) {
			if (posPlayer.y / tileSize >= (posFire.y / tileSize) - 5 && posPlayer.y / tileSize <= (posFire.y / tileSize) - 1) {
				return true;
			}
		}
	}
	return false;
}

void playScene::deleteEngine() {
	if(engine)engine->drop();
}

void playScene::changeMusic(char* music) {

	engine = createIrrKlangDevice();

	engine->play2D(music, true);
}

void::playScene::createSound(char* music, bool repeat) {
	sound = createIrrKlangDevice();
	sound->play2D(music, repeat);
}
void playScene::deleteSound() {
	sound->drop();
}

void playScene::hitPlayer() {
	
	if (!player->isHittedPlayer()) {
		createSound("../../../libs/irrKlang-1.6.0/media/hitPlayer.wav", false);
	}
	player->hitByEnemy();
}


/* @brief Function that initializes de shaders*/
void playScene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


