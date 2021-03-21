#include "playScene.h"
#include "SceneManager.h"
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
}

/* @brief Default destructor */
playScene::~playScene() {
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}

/* @brief Overrided init function
 *
 * This function calls to initialize the shaders, and also
 * loads the logo and sets the camera to the perfect position
 * for the scene.
 */
void playScene::init(void)
{
	initShaders();

	logoTexture.loadFromFile("images/black.png", TEXTURE_PIXEL_FORMAT_RGBA);
	logoTexture.setMinFilter(GL_NEAREST);
	logoTexture.setMagFilter(GL_NEAREST);
	//TileMap
	map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	logo = Sprite::createSprite(glm::ivec2(BACKGROUND_X, BACKGROUND_Y), glm::vec2(1.f, 1.f), &logoTexture, &texProgram);
	logo->setPosition(glm::vec2(0, 0));
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

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
	player->update(deltaTime);
	//count += 1;

	// 210 -> 3.5 seconds (60 frames/s)
	/*if (count > 210 || Game::instance().getKey(SPACEBAR) || Game::instance().getKey(ESCAPE))
	{
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::C_SCENE);

		count = 0;
	}*/
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
	map->render();
	player->render();
}

/* @brief Overrided function used to finalize scenes*/
void playScene::fin() {}

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


