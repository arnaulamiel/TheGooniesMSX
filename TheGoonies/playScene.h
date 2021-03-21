#pragma once
#include "Game.h"
#include "Scene.h"
#include "Sprite.h"
#include <iostream>

/* @file playScene.h
*
* @brief This class represents the first scene the will be shown when booting the game.
*
* It is basically the scene where the logo is shown.
* It is inherited from the Scene class.
*/
class playScene : public Scene
{
public:

	/* Static member function declaration */
	static Scene* create();

	/* Constructor */
	playScene();
	/* Destructor */
	virtual ~playScene();

	/* Inherited functions */
	virtual void init() override;
	virtual void update(int deltaTime) override;
	virtual void render() override;
	virtual void fin() override;

private:
	/* Function to initialize the shaders */
	void initShaders();

	/* Variable to control the time that the logo is shown */
	int count;

	/* Texture of the logo */
	Texture logoTexture;

	TileMap* map;
	float currentTime;

	/* Logo sprite*/
	Sprite* logo;
	Player* player;

	/* Projection matrix */
	glm::mat4 projection;
	/* Shader program*/
	ShaderProgram texProgram;
};



