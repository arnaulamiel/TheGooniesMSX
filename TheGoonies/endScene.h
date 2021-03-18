#pragma once
#include "Game.h"
#include "Scene.h"
#include "Sprite.h"
#include <iostream>

/* @file endScene.h
*
* @brief This class represents the first scene the will be shown when booting the game.
*
* It is basically the scene where the logo is shown.
* It is inherited from the Scene class.
*/
class endScene : public Scene
{
public:

	/* Static member function declaration */
	static Scene* create();

	/* Constructor */
	endScene();
	/* Destructor */
	virtual ~endScene();

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

	/* Logo sprite*/
	Sprite* logo;

	/* Projection matrix */
	glm::mat4 projection;
	/* Shader program*/
	ShaderProgram texProgram;
};



