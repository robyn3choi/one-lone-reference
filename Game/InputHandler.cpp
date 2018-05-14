#include "Precompiled.h"
#include "InputHandler.h"


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::HandleInput()
{
	MovePlayer();

	UpdateCursor();

	HandleMouseEvents();
}

void InputHandler::MovePlayer()
{
	mMoveDirection = Vector2(0, 0);
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		mMoveDirection += Vector2(0, -1);
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		mMoveDirection += Vector2(0, 1);
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		mMoveDirection += Vector2(-1, 0);
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		mMoveDirection += Vector2(1, 0);
	}

	GameManager::Instance().GetPlayer()->SetMoveDirection(mMoveDirection);
}

void InputHandler::UpdateCursor()
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mCursorPosition = Vector2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	mCursorDirection = Vector2(mCursorPosition - Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	mCursorDirection.Normalize();
}

void InputHandler::HandleMouseEvents()
{
	while (SDL_PollEvent(&mEvent) != 0)
	{
		if (mEvent.type == SDL_QUIT)
		{
			GameManager::Instance().StopRunning();
		}

		if (GameManager::Instance().IsGameOver())
		{
			if (mEvent.type == SDL_MOUSEBUTTONUP)
			{
				if (mEvent.button.button == SDL_BUTTON_LEFT)
				{
					if (IsCursorOverTryAgainButton())
					{
						GameManager::Instance().Restart();
					}
				}
			}
		}
		else if (mEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			Player* player = GameManager::Instance().GetPlayer();

			if (mEvent.button.button == SDL_BUTTON_LEFT)
			{
				player->Shoot(mCursorDirection);
			}
			if (mEvent.button.button == SDL_BUTTON_RIGHT)
			{
				if (player->IsDashing())
				{
					return;
				}
				if (mMoveDirection.GetLength() == 0)
				{
					player->Dash(mCursorDirection);
				}
				else
				{
					player->Dash(mMoveDirection);
				}
			}
		}
	}
}

bool InputHandler::IsCursorOverTryAgainButton()
{
	Texture* tryAgainBtn = GameManager::Instance().GetTextureManager()->GetTexture(TextureType::TryAgainButton);
	int tryAgainBtnX = SCREEN_WIDTH / 2 - tryAgainBtn->GetWidth() / 2;

	return ((mCursorPosition.x > tryAgainBtnX) &&
			(mCursorPosition.x < tryAgainBtnX + tryAgainBtn->GetWidth()) &&
			(mCursorPosition.y > TRY_AGAIN_BUTTON_Y) &&
			(mCursorPosition.y < TRY_AGAIN_BUTTON_Y + tryAgainBtn->GetHeight()));
}
