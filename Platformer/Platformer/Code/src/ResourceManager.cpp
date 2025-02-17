#include "ResourceManager.h"

#include <tracy/Tracy.hpp>

#include "ThreadPool.h"

IResource::~IResource()
{
	std::cout << "Resource unloaded" << std::endl;
}

void IResource::LoadAsync(ThreadPool* pool)
{
	ZoneScoped;
	auto LoadLambda = [this]()
		{
			this->Load();
		};

	pool->Enqueue(LoadLambda);
}

ResourceManager::~ResourceManager()
{
	ThreadPool resourcePool(std::thread::hardware_concurrency());
	for (auto& r : resources)
	{
		resourcePool.Enqueue([resource = r.second]() {
			resource->Unload();
			delete resource;
			});
	}
	resourcePool.WaitUntilFinished();
	resources.clear();
}
