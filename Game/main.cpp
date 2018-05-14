
int main(int argc, char* argv[])
{
	try 
	{
		GameManager::Instance().Initialize();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	GameManager::Instance().CreateGameObjects();

	GameManager::Instance().Run();


	return 0;
}
