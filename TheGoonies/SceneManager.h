#ifndef _SCENEMANAGER_INCLUDE
#define _SCENEMANAGER_INCLUDE


enum SceneID
{
	NONE_SCENE = -1,
	INIT_SCENE,	
	PLAY_SCENE,
	END_SCENE,
	NUM_SCENES
};

// Definition of all scenes
class Scene;

class SceneManager
{

	using FactoryMethod = Scene*(*)();

public:

	// Get the singleton class of the SceneManager
	static SceneManager* instance();

	// Destructor
	virtual ~SceneManager();

	void addScene(SceneID scene_id, FactoryMethod factory_method); // Add scene
	void setStartScene(SceneID scene_id); // Start scene settings

	void updateActiveScene(int deltaTime); // Update an active scene
	void renderActiveScene(); // Render an active scene

	void requestScene(SceneID scene_id); // Request a change of scene
	void requestScene(SceneID scene_id, int bank);

	void setKeyPressed(int key, SceneID sceneId);
	void setKeyReleased(int key, SceneID sceneId);

private:

	Scene* m_active_scene; // Active scene
	SceneID m_next_scene_id; // ID of the next scene to be changed
	FactoryMethod m_scene_factory_methods[NUM_SCENES]; // Factory method for each scene

	// Constructor
	SceneManager();

	int startBank;

	// Internal implementation
	void changeScene(SceneID scene_id); // Change the scene

	// Replication prohibition and delete because it is made internally
	SceneManager(const SceneManager&) = delete; // Delete copy constructor
	SceneManager& operator = (const SceneManager&) = delete; // Remove assignment operator
	SceneManager(SceneManager&&) = delete; // Remove move operator
	SceneManager& operator = (SceneManager&&) = delete; // Remove the move assignment operator
};
#endif // _SCENEMANAGER_INCLUDE


