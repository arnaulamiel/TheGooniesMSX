#include "endScene.h"
#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>

// SCREEN_X and SCREEN_Y define the size of the logo image
#define SCREEN_X 272.f
#define SCREEN_Y 240.f

#define C 99
// GLUT SPACEBAR code
#define SPACEBAR 32
// GLUT ESC code
#define ESCAPE 27


/* @brief Static member function declaration */
Scene* endScene::create()
{
	endScene* scene = new endScene();
	return scene;
}

/* @brief Default constructor */
endScene::endScene() : Scene()
{
	this->sceneID = INIT_SCENE;
}

/* @brief Default destructor */
endScene::~endScene() {}

/* @brief Overrided init function
 *
 * This function calls to initialize the shaders, and also
 * loads the logo and sets the camera to the perfect position
 * for the scene.
 */
void endScene::init(void)
{
	initShaders();
	changeMusic("../../../libs/irrKlang-1.6.0/media/gameOverGoonies.ogg");


	logoTexture.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
void endScene::update(int deltaTime)
{

	if (Game::instance().getKey(C)) {
		deleteEngine();
		SceneManager* scene_manager = SceneManager::instance();
		scene_manager->requestScene(SceneID::INIT_SCENE);
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
void endScene::render()
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

void endScene::deleteEngine() {
	engine->drop();
}

void endScene::changeMusic(char* music) {

	engine = createIrrKlangDevice();

	engine->play2D(music, true);
}

void::endScene::createSound(char* music, bool repeat) {
	sound = createIrrKlangDevice();
	sound->play2D(music, true);
}
void endScene::deleteSound() {
	sound->drop();
}

/* @brief Overrided function used to finalize scenes*/
void endScene::fin() {}

/* @brief Function that initializes de shaders*/
void endScene::initShaders()
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


