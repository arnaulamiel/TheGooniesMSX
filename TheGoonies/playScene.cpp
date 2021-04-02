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

	initShaders();

	logoTexture.loadFromFile("images/black.png", TEXTURE_PIXEL_FORMAT_RGBA);
	logoTexture.setMinFilter(GL_NEAREST);
	logoTexture.setMagFilter(GL_NEAREST);

	/*vidaexpTexture.loadFromFile("images/VidaExpSprite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	vidaexpTexture.setMinFilter(GL_NEAREST);
	vidaexpTexture.setMagFilter(GL_NEAREST);*/

	//TileMap
	initMaps();
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
		player->calHit();
		cal->killedCal();

	}
	if (hitEnem(bat)) {
		player->calHit();
		bat->killedBat();
	}

	if (!hasKey) {
		bool found = false;
		for (int i = 0; i < actualRoomObjects.size() && !found; i++) {
			if (actualRoomObjects[i] != nullptr && actualRoomObjects[i]->getObjectType() == KEY) {
				found = true;
				Object* key = actualRoomObjects[i];
				if (getKey(key)) {
					hasKey = true;
					actualRoomObjects[i]->destroyObject();
					actualRoomObjects[i] = nullptr;
				}
			}
		}
	}

	player->update(deltaTime);
	cal->update(deltaTime);
	bat->update(deltaTime);
	
	
	if (map->isMapLimitRight(player->getPosPlayer())) {
		++room;
		updateRoom();
		player->setPosition(glm::vec2(2 * map->getTileSize(), (player->getPosPlayer().y)/18 * map->getTileSize()));
		player->setTileMap(map);

		cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		cal->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
		cal->setPatrolPoints(10 * map->getTileSize(), 30 * map->getTileSize());
		cal->setPlayer(player);
		cal->setTileMap(map);
		
		
	}

	if (room != 1 && map->isMapLimitLeft(player->getPosPlayer())) {
		--room;
		updateRoom();
		player->setPosition(glm::vec2(30 * map->getTileSize(), (player->getPosPlayer().y) / 18 * map->getTileSize()));
		player->setTileMap(map);

		cal->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		cal->setPosition(glm::vec2(INIT_CAL_X_TILES * map->getTileSize(), INIT_CAL_Y_TILES * map->getTileSize()));
		cal->setPatrolPoints(10 * map->getTileSize(), 20 * map->getTileSize());
		cal->setPlayer(player);
		cal->setTileMap(map);

		
	}
	
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
	player->render();
	cal->render();
	bat->render();

	for (int i = 0; i < actualRoomObjects.size(); i++)
	{
		if (actualRoomObjects[i] != nullptr)
			actualRoomObjects[i]->render();
	}
}

/* @brief Overrided function used to finalize scenes*/
void playScene::fin() {}

void playScene::initMaps() {
	mapIni = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	mapIni2 = TileMap::createTileMap("levels/level04.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
}

void playScene::updateRoom() {
	if (room == 1) {
		map = mapIni;
	}
	else if (room == 2) {
		map = mapIni2;
	}
	
}

void playScene::loadRoomObjects() 
{
	//if (objectsRoom1.size() != 0) objectsRoom1.erase(objectsRoom1.begin(), objectsRoom1.end());

	loadSingleRoomObjects("objects/prueba.txt", objectsRoom1);
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
				glm::ivec2 position = glm::ivec2(18 * i + SCREEN_X, 18 * j + SCREEN_Y);
				//glm::ivec2 position = glm::ivec2(tileSize * i, tileSize * j );
				if (tile == 1) {
					Object* key = new Object(KEY, position, glm::vec2(18, 18));
					key->init(texProgram);
					objectsRoom.push_back(key);
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
	actualRoomObjects = objectsRoom1;
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
					return true;
				}
			}

		}//Si esta pegando der
		else if (player->getState() == 8) {
			if (posPlayer.x / tilesize >= (posCal.x / tilesize) - 2 && posPlayer.x / tilesize <= (posCal.x / tilesize)) {
				if (posPlayer.y / tilesize >= (posCal.y / tilesize) - 1 && posPlayer.y / tilesize <= (posCal.y / tilesize) + 1) {
					return true;
				}
			}
		}
	}
	return false;

}

bool playScene::getKey(Object* k) {
	glm::vec2 posKey = k->getObjectPosition();
	glm::vec2 posPlayer = player->getPosPlayer();
	int tileSize = map->getTileSize();
	if (posPlayer.x / tileSize >= (posKey.x / tileSize) - 4 && posPlayer.x / tileSize <= (posKey.x / tileSize) + 4) {
		if (posPlayer.y / tileSize >= (posKey.y / tileSize) - 4 && posPlayer.y / tileSize <= (posKey.y / tileSize) + 4) {
			return true;
		}
	}
	return false;
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


