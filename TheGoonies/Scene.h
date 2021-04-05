#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

#include "SceneManager.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{
protected:
	SceneID sceneID;
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void init() = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;
	virtual void fin() = 0;

	virtual void setSceneID(SceneID sceneId)
	{
		sceneID = sceneId;
	};

	virtual SceneID getSceneID()
	{
		return sceneID;
	};
	virtual void deleteEngine() = 0;
	virtual void changeMusic(char* music) = 0;
	virtual void createSound(char* music, bool repeat) = 0;
	virtual void deleteSound() = 0;





};


#endif // _SCENE_INCLUDE
