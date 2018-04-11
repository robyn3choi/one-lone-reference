#include "Precompiled.h"
#include "GameManager.h"

int main(int argc, char* args[])
{
	//GameManager gameManager;

	//Start up SDL and create window
	if (!GameManager::Instance().Initialize())
	{
		printf("Failed to initialize!\n");
	}

	GameManager::Instance().CreateGameObjects();

	GameManager::Instance().Run();


	return 0;
}
