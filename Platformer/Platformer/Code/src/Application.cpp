#include "Application.h"
#include <tracy/Tracy.hpp>
void Application::Initialize(uint16_t width, uint16_t height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		return;

	window = glfwCreateWindow(m_width, m_height, "Platformer3D_Shadow", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, InputManager::KeyCallback);
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);
	glfwSetCursorPosCallback(window, InputManager::MousePositionCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
}

void Application::Update()
{
	SceneManager _sceneManager(&resourceManager, window);
	sceneManager = _sceneManager;
	uint32_t hardwareThreads = std::thread::hardware_concurrency();
	LoadResourcesAsync(hardwareThreads);
	//LoadResourcesAsync(std::min(hardwareThreads, 10u));
	//LoadResources();

	sceneManager.CreateScene("MainMenu", MainMenu, UpdateMainMenu);
	sceneManager.CreateScene("FirstLevel", FirstLevel, UpdateFirstLevel);
	sceneManager.LoadScene("FirstLevel");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	
	while (!glfwWindowShouldClose(window))
	{
		MyTime::Get().UpdateDeltaTime();
		CloseWindowInput();

		glClearColor(0.467f, 0.71f, 1.f, 0.996f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager.currentScene->Update(window, &resourceManager);
		sceneManager.currentScene->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	this->Terminate();
}


void Application::Terminate()
{
	glfwTerminate();
	Log::Get().Destroy();
	MyTime::Get().Destroy();
	InputManager::Destroy();
}

void Application::CloseWindowInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Application::LoadResources()
{
	FrameMark;
	auto startTime = std::chrono::high_resolution_clock::now();
	Shader* shader = resourceManager.Create<Shader>("Shader");
	shader->SetVertexShader("Assets/Shaders/VertexShader.glsl");
	shader->SetFragmentShader("Assets/Shaders/FragmentShader.glsl");
	shader->Load();

	Model* penguin = resourceManager.Create<Model>("Penguin", "Assets/Meshes/Penguin.obj");
	penguin->Load();

	Texture* penguinTexture = resourceManager.Create<Texture>("PenguinTexture", "Assets/Textures/Penguin.png");
	penguinTexture->Load();

	Model* cube = resourceManager.Create<Model>("Cube", "Assets/Meshes/cube.obj");
	cube->Load();

	Texture* defaultTexture = resourceManager.Create<Texture>("DefaultTexture", "Assets/Textures/Default.png");
	defaultTexture->Load();

	Texture* woodTexture = resourceManager.Create<Texture>("WoodTexture", "Assets/Textures/wood.png");
	woodTexture->Load();

	Model* city = resourceManager.Create<Model>("city", "Assets/Meshes/City.obj");
	city->Load();

	Texture* cityTexture = resourceManager.Create<Texture>("cityTexture", "Assets/Textures/City.png");
	cityTexture->Load();

	Texture* highResImage = resourceManager.Create<Texture>("HighResImage", "Assets/Textures/highResImage.jpg");
	highResImage->Load();

	Model* spaceship = resourceManager.Create<Model>("Spaceship", "Assets/Meshes/spaceship.obj");
	spaceship->Load();

	Model* train = resourceManager.Create<Model>("Train", "Assets/Meshes/train.obj");
	train->Load();

	Model* spiderBot = resourceManager.Create<Model>("SpiderBot", "Assets/Meshes/SpiderBot.obj");
	spiderBot->Load();

	Texture* spiderBotTexture = resourceManager.Create<Texture>("SpiderBotTexture", "Assets/Textures/SpiderBot.png");
	spiderBotTexture->Load();

	Model* stadium = resourceManager.Create<Model>("Stadium", "Assets/Meshes/stadium.obj");
	stadium->Load();

	Texture* stadiumTexture = resourceManager.Create<Texture>("StadiumTexture", "Assets/Textures/stadium.jpg");
	stadiumTexture->Load();

	Model* ornithopter = resourceManager.Create<Model>("Ornithopter", "Assets/Meshes/ornithopter.obj");
	ornithopter->Load();

	Texture* ornithopterTexture = resourceManager.Create<Texture>("OrnithopterTexture", "Assets/Textures/ornithopter.png");
	ornithopterTexture->Load();

	Model* earth = resourceManager.Create<Model>("Earth", "Assets/Meshes/earth.obj");
	earth->Load();

	Texture* earthTexture = resourceManager.Create<Texture>("EarthTexture", "Assets/Textures/earth.png");
	earthTexture->Load();

	Texture* image = resourceManager.Create<Texture>("Image", "Assets/Textures/Image.jpg");
	image->Load();

	Model* man = resourceManager.Create<Model>("Man", "Assets/Meshes/man.obj");
	man->Load();

	Model* moon = resourceManager.Create<Model>("Moon", "Assets/Meshes/moon.obj");
	moon->Load();

	Texture* moonTexture = resourceManager.Create<Texture>("MoonTexture", "Assets/Textures/moon.png");
	moonTexture->Load();

	Model* rocket = resourceManager.Create<Model>("Rocket", "Assets/Meshes/Rocket.obj");
	rocket->Load();

	Texture* rocketTexture = resourceManager.Create<Texture>("RocketTexture", "Assets/Textures/Rocket.jpg");
	rocketTexture->Load();

	Model* satellite = resourceManager.Create<Model>("Satellite", "Assets/Meshes/Satellite.obj");
	satellite->Load();

	Texture* satelliteTexture = resourceManager.Create<Texture>("SatelliteTexture", "Assets/Textures/Satellite.jpg");
	satelliteTexture->Load();

	Model* tiger = resourceManager.Create<Model>("Tiger", "Assets/Meshes/Tiger.obj");
	tiger->Load();

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Resources loaded in " << duration << " ms" << std::endl;

	shader->Link();
	penguin->BindBuffers();
	cube->BindBuffers();
	city->BindBuffers();
	spaceship->BindBuffers();
	train->BindBuffers();
	spiderBot->BindBuffers();
	stadium->BindBuffers();
	ornithopter->BindBuffers();
	earth->BindBuffers();
	man->BindBuffers();
	moon->BindBuffers();
	rocket->BindBuffers();
	satellite->BindBuffers();
	tiger->BindBuffers();
	penguinTexture->Generate();
	defaultTexture->Generate();
	woodTexture->Generate();
	cityTexture->Generate();
	highResImage->Generate();
	spiderBotTexture->Generate();
	stadiumTexture->Generate();
	ornithopterTexture->Generate();
	earthTexture->Generate();
	image->Generate();
	moonTexture->Generate();
	rocketTexture->Generate();
	satelliteTexture->Generate();
}

void Application::LoadResourcesAsync(uint32_t nbThreads)
{
	FrameMark;
	ThreadPool resourcePool(nbThreads);

	auto startTime = std::chrono::high_resolution_clock::now();
	Shader* shader = resourceManager.Create<Shader>("Shader");
	shader->SetVertexShader("Assets/Shaders/VertexShader.glsl");
	shader->SetFragmentShader("Assets/Shaders/FragmentShader.glsl");
	shader->LoadAsync(&resourcePool);

	Model* penguin = resourceManager.Create<Model>("Penguin", "Assets/Meshes/Penguin.obj");
	penguin->LoadAsync(&resourcePool);

	Texture* penguinTexture = resourceManager.Create<Texture>("PenguinTexture", "Assets/Textures/Penguin.png");
	penguinTexture->LoadAsync(&resourcePool);

	Model* cube = resourceManager.Create<Model>("Cube", "Assets/Meshes/cube.obj");
	cube->LoadAsync(&resourcePool);

	Texture* defaultTexture = resourceManager.Create<Texture>("DefaultTexture", "Assets/Textures/Default.png");
	defaultTexture->LoadAsync(&resourcePool);

	Texture* woodTexture = resourceManager.Create<Texture>("WoodTexture", "Assets/Textures/wood.png");
	woodTexture->LoadAsync(&resourcePool);

	Model* city = resourceManager.Create<Model>("city", "Assets/Meshes/City.obj");
	city->LoadAsync(&resourcePool);
	
	Texture* cityTexture = resourceManager.Create<Texture>("CityTexture", "Assets/Textures/City.png");
	cityTexture->LoadAsync(&resourcePool);

	Texture* highResImage = resourceManager.Create<Texture>("HighResImage", "Assets/Textures/highResImage.jpg");
	highResImage->LoadAsync(&resourcePool);

	Model* spaceship = resourceManager.Create<Model>("Spaceship", "Assets/Meshes/spaceship.obj");
	spaceship->LoadAsync(&resourcePool);

	Model* train = resourceManager.Create<Model>("Train", "Assets/Meshes/train.obj");
	train->LoadAsync(&resourcePool);

	Model* spiderBot = resourceManager.Create<Model>("SpiderBot", "Assets/Meshes/SpiderBot.obj");
	spiderBot->LoadAsync(&resourcePool);

	Texture* spiderBotTexture = resourceManager.Create<Texture>("SpiderBotTexture", "Assets/Textures/SpiderBot.png");
	spiderBotTexture->LoadAsync(&resourcePool);

	Model* stadium = resourceManager.Create<Model>("Stadium", "Assets/Meshes/stadium.obj");
	stadium->LoadAsync(&resourcePool);

	Texture* stadiumTexture = resourceManager.Create<Texture>("StadiumTexture", "Assets/Textures/stadium.jpg");
	stadiumTexture->LoadAsync(&resourcePool);

	Model* ornithopter = resourceManager.Create<Model>("Ornithopter", "Assets/Meshes/ornithopter.obj");
	ornithopter->LoadAsync(&resourcePool);

	Texture* ornithopterTexture = resourceManager.Create<Texture>("OrnithopterTexture", "Assets/Textures/ornithopter.png");
	ornithopterTexture->LoadAsync(&resourcePool);

	Model* earth = resourceManager.Create<Model>("Earth", "Assets/Meshes/earth.obj");
	earth->LoadAsync(&resourcePool);

	Texture* earthTexture = resourceManager.Create<Texture>("EarthTexture", "Assets/Textures/earth.png");
	earthTexture->LoadAsync(&resourcePool);

	Texture* image = resourceManager.Create<Texture>("Image", "Assets/Textures/Image.jpg");
	image->LoadAsync(&resourcePool);

	Model* man = resourceManager.Create<Model>("Man", "Assets/Meshes/man.obj");
	man->LoadAsync(&resourcePool);

	Model* moon = resourceManager.Create<Model>("Moon", "Assets/Meshes/moon.obj");
	moon->LoadAsync(&resourcePool);

	Texture* moonTexture = resourceManager.Create<Texture>("MoonTexture", "Assets/Textures/moon.png");
	moonTexture->LoadAsync(&resourcePool);

	Model* rocket = resourceManager.Create<Model>("Rocket", "Assets/Meshes/Rocket.obj");
	rocket->LoadAsync(&resourcePool);

	Texture* rocketTexture = resourceManager.Create<Texture>("RocketTexture", "Assets/Textures/Rocket.jpg");
	rocketTexture->LoadAsync(&resourcePool);

	Model* satellite = resourceManager.Create<Model>("Satellite", "Assets/Meshes/Satellite.obj");
	satellite->LoadAsync(&resourcePool);

	Texture* satelliteTexture = resourceManager.Create<Texture>("SatelliteTexture", "Assets/Textures/Satellite.jpg");
	satelliteTexture->LoadAsync(&resourcePool);

	Model* tiger = resourceManager.Create<Model>("Tiger", "Assets/Meshes/Tiger.obj");
	tiger->LoadAsync(&resourcePool);

	resourcePool.WaitUntilFinished();
	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cout << "Resources loaded in " << duration << " ms" << std::endl;

	shader->Link();
	penguin->BindBuffers();
	cube->BindBuffers();
	city->BindBuffers();
	spaceship->BindBuffers();
	train->BindBuffers();
	spiderBot->BindBuffers();
	stadium->BindBuffers();
	ornithopter->BindBuffers();
	earth->BindBuffers();
	man->BindBuffers();
	moon->BindBuffers();
	rocket->BindBuffers();
	satellite->BindBuffers();
	tiger->BindBuffers();
	penguinTexture->Generate();
	defaultTexture->Generate();
	woodTexture->Generate();
	cityTexture->Generate();
	highResImage->Generate();
	spiderBotTexture->Generate();
	stadiumTexture->Generate();
	ornithopterTexture->Generate();
	earthTexture->Generate();
	image->Generate();
	moonTexture->Generate();
	rocketTexture->Generate();
	satelliteTexture->Generate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}
