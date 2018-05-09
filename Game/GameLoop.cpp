//#include "Precompiled.h"
//#include "GameLoop.h"
//
//
//GameLoop::GameLoop()
//{
//}
//
//
//GameLoop::~GameLoop()
//{
//}
//
//void GameLoop::RunGameLoop(SDL_Event e)
//{
//	HandleInput(e);
//
//	CenterCameraOverPlayer();
//
//	CheckCollisions();
//
//	SDL_RenderClear(GameManager::Instance().GetRenderer());
//
//	RenderTiledGround();
//
//	float deltaTime = GetDeltaTime();
//	UpdateAndRenderGameObjects(deltaTime);
//
//	if (GameManager::Instance().IsGameOver())
//	{
//		RenderGameOverUI();
//	}
//
//	GameManager::Instance().GetTextureManager()->GetTexture(TextureType::Crosshair)->Render(mCursorPos.x, mCursorPos.y);
//
//	RenderHearts();
//
//	SDL_RenderPresent(GameManager::Instance().GetRenderer());
//}
//
//
//void GameLoop::CenterCameraOverPlayer()
//{
//	int playerWidth = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::Player)->GetWidth();
//	int playerHeight = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::Player)->GetHeight();
//	mCamera.x = (static_cast<int>(mPlayer->GetPosition().x) + playerWidth / 2) - SCREEN_WIDTH / 2;
//	mCamera.y = (static_cast<int>(mPlayer->GetPosition().y) + playerHeight / 2) - SCREEN_HEIGHT / 2;
//}
//
//void GameLoop::RenderTiledGround()
//{
//	for (int i = 0; i <= LEVEL_WIDTH - TILE_WIDTH; i += TILE_WIDTH)
//	{
//		for (int j = 0; j <= LEVEL_HEIGHT - TILE_WIDTH; j += TILE_WIDTH)
//		{
//			mGroundTexture->Render(i - mCamera.x, j - mCamera.y);
//		}
//	}
//}
//
//float GameLoop::GetDeltaTime()
//{
//	float timeAtCurrentFrame = (float)SDL_GetTicks();
//	float deltaTime = (timeAtCurrentFrame - mTimeAtPreviousFrame) / 1000; // deltatime in seconds
//	mTimeAtPreviousFrame = timeAtCurrentFrame;
//	return deltaTime;
//}
//
//void GameLoop::UpdateAndRenderGameObjects(float deltaTime)
//{
//	for (auto& g : mGameObjects)
//	{
//		if (g->IsActive())
//		{
//			g->Update(deltaTime);
//			Vector2 pos = g->GetPosition();
//			Texture* tex = mTextureManager->GetTexture(g->GetTextureType());
//			tex->Render(static_cast<int>(pos.x) - mCamera.x, static_cast<int>(pos.y) - mCamera.y);
//		}
//	}
//}
//
//void GameLoop::RenderGameOverUI()
//{
//	Texture* gameOverText = mTextureManager->GetTexture(TextureType::GameOverText);
//	gameOverText->Render(SCREEN_WIDTH / 2 - gameOverText->GetWidth() / 2, 200);
//
//	// TODO: REFACTOR
//	Texture* tryAgainBtn = mTextureManager->GetTexture(TextureType::TryAgainButton);
//	tryAgainBtn->Render(SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2, 700);
//}
//
//void GameLoop::RenderHearts()
//{
//	Texture* heart = mTextureManager->GetTexture(TextureType::Heart);
//	int currentHealth = mPlayer->GetHealth();
//
//	if (currentHealth > 0)
//	{
//		heart->Render(50, 50);
//	}
//	if (currentHealth > 1)
//	{
//		heart->Render(120, 50);
//	}
//	if (currentHealth > 2)
//	{
//		heart->Render(190, 50);
//	}
//}
//
//void GameLoop::HandleInput(SDL_Event& e)
//{
//	int mouseX;
//	int mouseY;
//	SDL_GetMouseState(&mouseX, &mouseY);
//	Vector2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));
//	Vector2 mouseDir(mousePos - Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
//	mouseDir.Normalize();
//	Vector2 moveDir(0, 0);
//	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
//	if (currentKeyStates[SDL_SCANCODE_W])
//	{
//		moveDir += Vector2(0, -1);
//	}
//	else if (currentKeyStates[SDL_SCANCODE_S])
//	{
//		moveDir += Vector2(0, 1);
//	}
//	if (currentKeyStates[SDL_SCANCODE_A])
//	{
//		moveDir += Vector2(-1, 0);
//	}
//	else if (currentKeyStates[SDL_SCANCODE_D])
//	{
//		moveDir += Vector2(1, 0);
//	}
//
//	mPlayer->Move(moveDir);
//
//	Vector2 centrePos = Vector2(static_cast<float>(mCamera.x + SCREEN_WIDTH / 2), static_cast<float>(mCamera.y + SCREEN_HEIGHT / 2));
//	m_CursorPos = mousePos; // + mouseDir * CURSOR_RADIUS;
//
//							//Handle events on queue
//	while (SDL_PollEvent(&e) != 0)
//	{
//		if (e.type == SDL_QUIT)
//		{
//			mIsGameRunning = false;
//		}
//
//		if (e.type == SDL_MOUSEBUTTONDOWN)
//		{
//			if (e.button.button == SDL_BUTTON_LEFT)
//			{
//				// shoot
//				//Bullet* bullet = mPlayerBulletPool->GetBullet();
//				mPlayerBulletPool->GetBullet()->Shoot(mPlayer->GetPosition(), mouseDir);
//			}
//			if (e.button.button == SDL_BUTTON_RIGHT)
//			{
//				//dash
//				if (mPlayer->IsDashing())
//				{
//					return;
//				}
//				if (moveDir.GetLength() == 0)
//				{
//					mPlayer->Dash(mouseDir);
//				}
//				else
//				{
//					mPlayer->Dash(moveDir);
//				}
//			}
//		}
//
//		if (mIsGameOver)
//		{
//			if (e.type == SDL_MOUSEBUTTONUP)
//			{
//				if (e.button.button == SDL_BUTTON_LEFT)
//				{
//					Texture* tryAgainBtn = mTextureManager->GetTexture(TextureType::TryAgainButton);
//					int tryAgainBtnX = SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2;
//					int tryAgainBtnY = 700;
//					//If the mouse is over the button 
//					if ((mouseX > tryAgainBtnX) &&
//						(mouseX <tryAgainBtnX + tryAgainBtn->GetWidth()) &&
//						(mouseY > tryAgainBtnY) &&
//						(mouseY < tryAgainBtnY + tryAgainBtn->GetHeight()))
//					{
//						//Set the button sprite 
//						Restart();
//					}
//				}
//			}
//		}
//
//	}
//}
//
//void GameLoop::CheckCollisions()
//{
//	// check if player has been hit by enemy bullets 
//	// check if bullets have hit walls
//	for (auto &bullet : mEnemyBulletPool->GetPool())
//	{
//		if (!bullet->IsActive())
//		{
//			continue;
//		}
//		SDL_Rect playerCollider = mPlayer->GetCollider();
//		SDL_Rect bulletCollider = bullet->GetCollider();
//		if (SDL_HasIntersection(&playerCollider, &bulletCollider))
//		{
//			if (!mPlayer->IsDashing())
//			{
//				mPlayer->TakeDamage();
//				mEnemyBulletPool->ReturnBullet(bullet.get());
//			}
//		}
//		else
//		{
//			int enemyBulletTexWidth = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetWidth();
//			int enemyBulletTexHeight = mTextureManager->GetTexture(TextureType::EnemyBullet)->GetHeight();
//			if (IsOutOfBounds(bullet->GetPosition(), enemyBulletTexWidth, enemyBulletTexHeight))
//			{
//				mEnemyBulletPool->ReturnBullet(bullet.get());
//			}
//		}
//	}
//
//	// check if enemies have been hit by player bullets
//	// check if bullets have hit walls
//	for (auto &bullet : mPlayerBulletPool->GetPool())
//	{
//		if (!bullet->IsActive())
//		{
//			continue;
//		}
//		for (auto& enemy : mEnemies)
//		{
//			if (!enemy->IsActive())
//			{
//				continue;
//			}
//			SDL_Rect enemyCollider = enemy->GetCollider();
//			SDL_Rect bulletCollider = bullet->GetCollider();
//			if (SDL_HasIntersection(&enemyCollider, &bulletCollider))
//			{
//				enemy->TakeDamage();
//				mPlayerBulletPool->ReturnBullet(bullet.get());
//				std::cout << "enemy damaged" << std::endl;
//			}
//			else
//			{
//				int playerBulletTexWidth = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetWidth();
//				int playerBulletTexHeight = mTextureManager->GetTexture(TextureType::PlayerBullet)->GetHeight();
//
//				if (IsOutOfBounds(bullet->GetPosition(), playerBulletTexWidth, playerBulletTexHeight))
//				{
//					mEnemyBulletPool->ReturnBullet(bullet.get());
//				}
//			}
//		}
//	}
//}
//
//bool GameLoop::IsOutOfBounds(Vector2 pos, int width, int height)
//{
//	if (static_cast<int>(pos.x) > LEVEL_WIDTH - width ||
//		static_cast<int>(pos.x) < 0 ||
//		static_cast<int>(pos.y) > LEVEL_HEIGHT - height ||
//		static_cast<int>(pos.y) < 0)
//	{
//		return true;
//	}
//	return false;
//}
