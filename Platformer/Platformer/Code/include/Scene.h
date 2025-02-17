#pragma once
#include "Graph.h"
#include "Player.h"
#include "Collision.h"

class Scene
{
public:
	Scene();
	~Scene() = default;
	Graph<Object*> sceneGraph;
	Camera camera;
	std::vector<Light*> lights;
	Shader* shader;
	std::vector<Material*> materials;
	Player* player;
	Plane plane; 
	Vector3D cameraOffset;
	void Update(GLFWwindow* window, ResourceManager* resourceManager);
	void Draw();
	void Load(GLFWwindow* window, ResourceManager* resourceManager);
	void Unload();
	void (*InitFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
	void (*UpdateFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
};

