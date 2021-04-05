#include "initScene.h"
#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>

// SCREEN_X and SCREEN_Y define the size of the logo image
#define SCREEN_X 272.f
#define SCREEN_Y 240.f

// GLUT SPACEBAR code
#define SPACEBAR 32
// GLUT ESC code
#define ESCAPE 27


/* @brief Static member function declaration */
Scene* initScene::create()
{
	initScene* scene = new initScene();
	return scene;
}

/* @brief Default constructor */
initScene::initScene() : Scene()
{
	this->sceneID = INIT_SCENE;
}

/* @brief Default destructor */
initScene::~initScene() {}

/* @brief Overrided init function
 *
 * This function calls to initialize the shaders, and also
 * loads the logo and sets the camera to the perfect position
 * for the scene.
 */
void initScene::init(void)
{
	initShaders();
	changeMusic("../../../libs/irrKlang-1.6.0/media/titleGoonies.ogg");
	logoTexture.loadFromFile("images/initGoonies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	logoTexture.setMinFilter(GL_NEAREST);
	logoTexture.setMagFilter(GL_NEAREST);

	logo = Sprite::createSprite(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(1.f, 1.f), &logoTexture, &texProgram);
	logo->setPosition(glm::vec2(0, 0));

	projection = glm::ortho(0.f, SCREEN_X, SCREEN_Y, 0.f);
}

/* @brief Overrided update function
 *
 * This function updates the scene.
 * The count variable is used as a timer so the scene duration is accurate.
 * This scene can be jumped using pressing the spacebar.
 *
 * @param deltaTime
 */
void initScene::update(int deltaTime)
{//ENTER
	if (Game::instance().getKey(13)) {
		deleteEngine();
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::PLAY_SCENE);
	}

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
void initScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	logo->render();
}

/* @brief Overrided function used to finalize scenes*/
void initScene::fin() {}

void initScene::deleteEngine() {
	engine->drop();
}

void initScene::changeMusic(char* music) {

	engine = createIrrKlangDevice();

	engine->play2D(music, true);
}

void::initScene::createSound(char* music, bool repeat) {
	sound = createIrrKlangDevice();
	sound->play2D(music, repeat);
}
void initScene::deleteSound() {
	sound->drop();
}
/* @brief Function that initializes de shaders*/
void initScene::initShaders()
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


