#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include <cassert>

using namespace std;


SceneManager* SceneManager::instance()
{
	static SceneManager instance;
	return &instance;
}

SceneManager::SceneManager()
{
	m_active_scene = nullptr;
	m_next_scene_id = SceneID::NONE_SCENE;

	for (int i = 0; i < NUM_SCENES; i++)
	{
		m_scene_factory_methods[i] = nullptr;
	}
}

SceneManager::~SceneManager()
{
	delete m_active_scene;
}

void SceneManager::addScene(SceneID scene_id, FactoryMethod factory_method)
{
	//assert(scene_id >= 0 && scene_id < NUM_SCENES && "The scene ID is invalid.");
	//assert(factory_method && "No generation function is specified.");
	//Añadimos la escena en la factory.
	m_scene_factory_methods[scene_id] = factory_method;
}

void SceneManager::setStartScene(SceneID scene_id)
{
	//assert(scene_id >= 0 && scene_id < NUM_SCENES && "The scene ID is invalid.");

	changeScene(scene_id);
}

void SceneManager::updateActiveScene(int deltaTime)
{
	assert(m_active_scene && "There are no active scenes.");

	if (m_next_scene_id != NONE_SCENE)
	{
		changeScene(m_next_scene_id);
		m_next_scene_id = NONE_SCENE;
	}

	m_active_scene->update(deltaTime);
}

void SceneManager::renderActiveScene()
{
	assert(m_active_scene && "There are no active scenes.");

	m_active_scene->render();
}

void SceneManager::requestScene(SceneID scene_id)
{
	assert(scene_id >= 0 && scene_id < NUM_SCENES && "The scene ID is invalid.");

	m_next_scene_id = scene_id;
}

void SceneManager::requestScene(SceneID scene_id, int bank)
{
	assert(scene_id >= 0 && scene_id < NUM_SCENES && bank >= 1 && bank < 17 && "The scene ID is invalid.");

	m_next_scene_id = scene_id;
	startBank = bank;
}

void SceneManager::changeScene(SceneID scene_id)
{
	//assert(scene_id >= 0 && scene_id < NUM_SCENES && "The scene ID is invalid.");
	//assert(m_scene_factory_methods[scene_id] && "The generation function is not registered.");

	//Si ya tenemos una escena activa (no es el caso de la primera vez)
	if (m_active_scene != nullptr)
	{
		m_active_scene->fin(); //acabamos con la escena y la borramos
		delete m_active_scene;
		m_active_scene = nullptr;
	}

	// seleccionamos la siguiente escena con el id que hemos pasado
	m_active_scene = m_scene_factory_methods[scene_id]();
	m_active_scene->setSceneID(scene_id);

	/*if (scene_id == PLAY_SCENE && startBank != NULL)
	{
		m_active_scene->setBank(startBank);
		startBank = NULL;
	}*/
	
	//iniciamos la escena nueva
	m_active_scene->init();
}

void SceneManager::setKeyPressed(int key, SceneID sceneId)
{
	if (m_active_scene->getSceneID() == sceneId)
	{
		//m_active_scene->possibleCharacter(key);
	}
}

void SceneManager::setKeyReleased(int key, SceneID sceneId)
{
	if (m_active_scene->getSceneID() == sceneId)
	{
		//m_active_scene->confirmCharacter(key);
	}
}





