#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "initScene.h"
#include "Scene.h"
#include "playScene.h"
#include "endScene.h"

using namespace irrklang;

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//scene.init();
	SceneManager* scene_manager = SceneManager::instance();
	scene_manager->addScene(INIT_SCENE, initScene::create);
	scene_manager->addScene(PLAY_SCENE, playScene::create);
	scene_manager->addScene(END_SCENE, endScene::create);

	scene_manager->setStartScene(INIT_SCENE);

	// start the sound engine with default parameters
	engine = createIrrKlangDevice();
	engine->play2D("/../../../libs\irrKlang-1.6.0\media",true);
}

bool Game::update(int deltaTime)
{
	/*per un exemple que ha fet de una class Input.Read ha fet ->
	enum KEY_STATE { KEY_IDLE, KEY_PRESS, KEY_RELEASE, KEY_REPEAT}
	KEY_STATE key_state[N];
	(...)
	for (i=0..){
		if(keys[i]){
			if(key_state[i]  == KEY_IDLE) key_state[i] = KEY_PRESS;
			else if(key_state[i]  == KEY_PRESS) key_state[i] = KEY_REPEAT;		
			(...)
		}
		else{
		//poden haber moltes opcions, depen del tipus de joc i tot aixo
			if(key_state[i]  == KEY_REPEAT || key_state[i]  == KEY_RELEASE) key_state[i] = KEY_RELEASE;
			(...)
		}
	*/
	SceneManager* scene_manager = SceneManager::instance();
	scene_manager->updateActiveScene(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SceneManager* scene_manager = SceneManager::instance();
	scene_manager->renderActiveScene();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

//tecles especials (f1,f2, arrows....)
void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





