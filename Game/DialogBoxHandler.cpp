#include "Precompiled.h"
#include "DialogBoxHandler.h"


DialogBoxHandler::DialogBoxHandler()
{
	TextureManager* textureManager = GameManager::Instance().GetTextureManager();
	mDialogBoxes.reserve(4);
	mDialogBoxes.push_back(textureManager->GetTexture(TextureType::PlayerText1));
	mDialogBoxes.push_back(textureManager->GetTexture(TextureType::BossText1));
	mDialogBoxes.push_back(textureManager->GetTexture(TextureType::BossText2));
	mDialogBoxes.push_back(textureManager->GetTexture(TextureType::PlayerText2));
}


DialogBoxHandler::~DialogBoxHandler()
{
}

// I would refactor this if I had time
void DialogBoxHandler::RunDialogBoxSequence(float deltaTime)
{
	if (mAreDialogBoxesFinished)
	{
		return;
	}
	mTimer -= deltaTime;

	if (mHaveDialogBoxesStarted)
	{
		mDialogBoxes[mIndex]->Render(DIALOG_BOX_POSITION.x, DIALOG_BOX_POSITION.y);
	}

	if (mTimer <= 0)
	{
		mHaveDialogBoxesStarted = true;
		if (mIndex < static_cast<int>(mDialogBoxes.size()) - 1)
		{
			mIndex++;
			mTimer = DIALOG_BOX_DURATION;
		}
		else
		{
			mAreDialogBoxesFinished = true;
		}
	}
}
