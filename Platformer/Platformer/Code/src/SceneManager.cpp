#include "SceneManager.h"

SceneManager::SceneManager(ResourceManager* _resourceManager, GLFWwindow* window)
{
	currentScene = nullptr;
	resourceManager = _resourceManager;
	this->window = window;
}

SceneManager::~SceneManager()
{
	for (std::pair<std::string, Scene*> s : scenes)
	{
		s.second->Unload();
		delete s.second;
	}
	scenes.clear();
}

void SceneManager::CreateScene(std::string id, void (*InitFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager), void (*UpdateFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager))
{
	Scene* scene = new Scene();
	scene->InitFunc = InitFunc;
	scene->UpdateFunc = UpdateFunc;
	scenes[id] = scene;
}

void SceneManager::LoadScene(std::string id)
{
	Scene* scene = FindScene(id);
	if (!scene)
		return;
	if (currentScene)
		currentScene->Unload();
	currentScene = scene;
	currentScene->Load(window, resourceManager);
}

void SceneManager::DeleteScene(std::string id)
{
	Scene* scene = FindScene(id);
	if (!scene)
		return;
	scene->Unload();
	scenes.erase(id);
}

Scene* SceneManager::FindScene(std::string id)
{
	auto it = scenes.find(id);
	if (it == scenes.end()) {
		std::cout << "No scene with ID " << id << " found" << std::endl;
		return nullptr;
	}
	
	return it->second;
}

void MainMenu(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	Camera camera(width, height, 45.f, 4.5f, -4.5f, 8.f, -8.f, 0.1f, 100.f, false, false);
	scene->camera = camera;
	Shader* shader = resourceManager->Get<Shader>("Shader");
	scene->shader = shader;

	Model* cube = resourceManager->Get<Model>("Cube");
	Texture* defaultTexture = resourceManager->Get<Texture>("DefaultTexture");
}

void UpdateMainMenu(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{

}

void FirstLevel(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	Camera camera(width, height, 45.f, 4.5f, -4.5f, 8.f, -8.f, 0.1f, 100.f, false, true);
	scene->camera = camera;
	Shader* shader = resourceManager->Get<Shader>("Shader");
	scene->shader = shader;

	//Create Object/Texture
	Model* cube = resourceManager->Get<Model>("Cube");
	Texture* defaultTexture = resourceManager->Get<Texture>("DefaultTexture");
	Texture* woodTexture = resourceManager->Get<Texture>("WoodTexture");
	Model* penguin = resourceManager->Get<Model>("Penguin");
	Texture* penguinTexture = resourceManager->Get<Texture>("PenguinTexture");
	Model* stadium = resourceManager->Get<Model>("Stadium");
	Texture* stadiumTexture = resourceManager->Get<Texture>("StadiumTexture");
	Model* spiderBot = resourceManager->Get<Model>("SpiderBot");
	Texture* spiderBotTexture = resourceManager->Get<Texture>("SpiderBotTexture");

	//Material init
	Material* material = new Material(Vector4D(1, 1, 1, 1), Vector4D(1.0f, 1.0f, 1.0f, 1), Vector4D(1, 1, 1, 1));
	Material* materialRed = new Material(Vector4D(1, 0, 0, 1), Vector4D(1.0f, 0.0f, 0.0f, 1), Vector4D(1, 0, 0, 1));
	scene->materials.push_back(material);
	scene->materials.push_back(materialRed);

	//Light
	LightingSettings::globalAmbiantColor = Vector4D(0, 0, 0, 1.0f);
	bool enableDirectionalLight = true;
	bool enablePointLight = false;
	bool enableSpotLight = false;

	//Initialise Directional Light
	int directionVisualLightIndex = -1;
	DirectionalLight* directionalLight = nullptr;
	if (enableDirectionalLight)
	{
		directionalLight = new DirectionalLight();
		directionalLight->direction = Vector3D(0, -0.5, -1);
		directionalLight->ambiantColor = Vector4D(0, 0, 0, 1.0f);
		directionalLight->diffuseColor = Vector4D(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight->specularColor = Vector4D(0.8f, 0.8f, 0.8f, 1.0f);
		directionalLight->intensity = 3.0f;
		scene->lights.push_back(directionalLight);
	}

	//Initialise Point Light
	PointLight* pointLight = nullptr;
	if (enablePointLight)
	{
		pointLight = new PointLight();
		pointLight->position = Vector3D(0, 40, 90);
		pointLight->specularColor = Vector4D(0.8f, 0.8f, 0.8f, 1.0f);
		pointLight->linearValue = 0.09f;
		pointLight->intensity = 500.0f;
		pointLight->quadraticValue = 0.032f;
		scene->lights.push_back(pointLight);

		Vector3D position = Vector3D(0.0f, 0, 0.0f);
		Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	//Initialise Spot Light
	int spotLightVisualIndex0 = -1;
	SpotLight* spotLight = nullptr;
	if (enableSpotLight)
	{
		spotLight = new SpotLight();
		spotLight->position = Vector3D(0, 0, -3);
		spotLight->direction = Vector3D(0, 0, -1);
		spotLight->linearValue = 0.09f;
		spotLight->ambiantColor = Vector4D(0.15f, 0.15f, 0.15f, 1.0f);
		spotLight->spotAngle = 20;
		spotLight->intensity = 2.0f;
		spotLight->spotSmoothValue = 0.2f;
		spotLight->quadraticValue = 0.032f;
		scene->lights.push_back(spotLight);
	}

	//Platform boundingBox
	std::vector<Vector3D> cubeVertices = loadObj("Assets/Meshes/cube.obj");
	Box* startPlatform = calculateBoundingBox(cubeVertices);
	Box* wall1 = calculateBoundingBox(cubeVertices);
	Box* wall2 = calculateBoundingBox(cubeVertices);
	Box* wall3 = calculateBoundingBox(cubeVertices);
	Box* platform1 = calculateBoundingBox(cubeVertices);
	Box* platform2 = calculateBoundingBox(cubeVertices);
	Box* platform3 = calculateBoundingBox(cubeVertices);
	Box* platform4 = calculateBoundingBox(cubeVertices);
	Box* platform5 = calculateBoundingBox(cubeVertices);
	Box* platform6 = calculateBoundingBox(cubeVertices);

	//Player collision sphere
	std::vector<Vector3D> playerVertices = loadObj("Assets/Meshes/Penguin.obj");
	Sphere* sphere = calculateBoundingSphere(playerVertices); 

	//Init death zone;
	Plane plane = Plane({ 0.f, -10.f, 0.f }, { 1.f, -10.f, 0.f }, { 0.f, -10.f, 1.f });
	scene->plane = plane;

	//Platform init 
	scene->sceneGraph.CreateObject("StartPlatform", { 0.f, 0.f, 1.f }, { 0.f, 180.f, 0.f }, { 2.f, 1.f, 2.f }, cube, defaultTexture, material, startPlatform, nullptr);
	scene->sceneGraph.CreateObject("Wall1", { 3.f, 2.f, 1.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 2.f }, cube, woodTexture, material, wall1, nullptr);
	scene->sceneGraph.CreateObject("Wall2", { -3.f, 2.f, 1.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 2.f }, cube, woodTexture, material, wall2, nullptr);
	scene->sceneGraph.CreateObject("Wall3", { 0.f, 2.f, 4.f }, { 0.f, 180, 0.f }, { 2.f, 1.f, 1.f }, cube, woodTexture, material, wall3, nullptr);
	scene->sceneGraph.CreateObject("Platform1", { -1.5f, 0.f, -7.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, woodTexture, material, platform1, nullptr);
	scene->sceneGraph.CreateObject("Platform2", { 1.f, 0.f, -14.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, woodTexture, material, platform2, nullptr);
	scene->sceneGraph.CreateObject("Platform3", { -0.5f, 0.f, -21.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, woodTexture, material, platform3, nullptr);
	scene->sceneGraph.CreateObject("Platform4", { 0.4f, 0.f, -28.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, woodTexture, material, platform4, nullptr);
	scene->sceneGraph.CreateObject("Platform5", { -1.f, 0.f, -35.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, woodTexture, material, platform5, nullptr);
	scene->sceneGraph.CreateObject("Platform6", { 2.f, 0.f, -42.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, cube, defaultTexture, materialRed, platform6, nullptr);

	scene->sceneGraph.CreateObject("Stadium", { 0.f, -5.f, -50.f }, { 180.f, 0.f, 0.f }, { 0.1f, 0.1f, 0.1f }, stadium, stadiumTexture, material, nullptr, nullptr);
	scene->sceneGraph.CreateObject("SpiderBot", { 2.f, 1.7f, -42.f }, { 0.f, 0.f, 0.f }, { 0.3f, 0.3f, 0.3f }, spiderBot, spiderBotTexture, material, nullptr, nullptr);

	//Player init
	scene->sceneGraph.CreateObject("penguin", { 0.f, 5.f, 0.f }, { 0.f, 180.f, 0.f }, { 1.f, 1.f, 1.f }, penguin, penguinTexture, material, nullptr, sphere);
	scene->player = new Player(scene->sceneGraph.FindObject("penguin"), &scene->sceneGraph);
	scene->cameraOffset = { 0.f, 3.5f, 5.f };
}

void UpdateFirstLevel(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{ 
	//Init raycast 
	Vector3D positionRaycast = scene->player->position;
	positionRaycast.y += 0.59f;
	positionRaycast.z -= 0.2f;
	Segment raycast(positionRaycast, scene->player->position - Vector3D(0.f, 0.f, 0.2f));

	//Collision DeathZone/Player
	float t;
	bool collision = CheckCollisionSegmentPlane(raycast, scene->plane, t);
	if (collision)
	{
		scene->Unload();
		scene->Load(window, resourceManager);
	}

	//Update player position
	Object* player = scene->sceneGraph.FindObject("penguin");
	player->sphere->position = player->GetPosition() + Vector3D(0.f, player->sphere->radius, 0.f);

	//Collision Sphere/Cube
	bool collide = false;
	for (std::pair<std::string, Object*> o : scene->sceneGraph.objects)
	{
		if (o.second->box == nullptr)
			continue;
		correctSpherePositionAfterCollision(o.second->box, player->sphere);
		Quad q = o.second->box->quad[0];
		float t0;
		if (CheckCollisionSegmentQuad(raycast, q, t0)) {
			collide = true;
		}
	}

	//Verify if player is grounded
	if (collide)
		scene->player->isGrounded = true;
	else
		scene->player->isGrounded = false;

	//Update player mouvement && Camera
	scene->player->object->SetPosition(player->sphere->position + Vector3D(0.f, -player->sphere->radius, 0.f));
	scene->camera.eye = scene->player->position + scene->cameraOffset;
	scene->camera.center = scene->player->position + Vector3D(0.f, 0.75f, 0.f);
	scene->camera.hasMoved = scene->player->hasMoved;
	scene->player->hasMoved = false;
}