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

	changeMusic("../../../libs/irrKlang-1.6.0/media/titleGoonies.ogg");
}

bool Game::update(int deltaTime)
{
	SceneManager* scene_manager = SceneManager::instance();
	scene_manager->updateActiveScene(deltaTime);
	/*if (a == 14) { 
		engine->drop();
		engine = createIrrKlangDevice();
		engine->play2D("../../../libs/irrKlang-1.6.0/media/gameOverGoonies.ogg", true); 
	}*/
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

void Game::deleteEngine() {
	engine->drop();
}

void Game::changeMusic(char* music) {

	engine = createIrrKlangDevice();
	
	engine->play2D(music, true);
}

void::Game::createSound(char* music) {
	sound = createIrrKlangDevice();
	sound->play2D(music, true);
}
void Game::deleteSound() {
	sound->drop();
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





