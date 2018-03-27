#include "Precompiled.h"
#include "GameManager.h"

int main(int argc, char* args[])
{
	GameManager gameManager;
	//Start up SDL and create window
	if (!gameManager.Initialize())
	{
		printf("Failed to initialize!\n");
	}
	if (!gameManager.LoadMedia())
	{
		printf("Failed to LoadMedia!\n");
	}

	gameManager.Run();


	return 0;
}
