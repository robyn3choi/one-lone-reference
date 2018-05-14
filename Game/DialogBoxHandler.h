#pragma once
class DialogBoxHandler
{
public:
	DialogBoxHandler();
	~DialogBoxHandler();

	void RunDialogBoxSequence(float deltaTime);

private:
	std::vector<Texture*> mDialogBoxes;
	float mTimer = DELAY_BEFORE_DIALOG_BOX;
	int mIndex = -1;
	bool mAreDialogBoxesFinished = false;
	bool mHaveDialogBoxesStarted = false;
};

