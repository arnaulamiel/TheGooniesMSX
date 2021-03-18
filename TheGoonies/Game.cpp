#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
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
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
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





