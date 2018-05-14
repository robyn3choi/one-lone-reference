#pragma once
class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void HandleInput();
	const Vector2& GetCursorPosition() const { return mCursorPosition; }

private:
	void MovePlayer();
	void UpdateCursor();
	void HandleMouseEvents();
	bool IsCursorOverTryAgainButton();

	SDL_Event mEvent;
	Vector2 mCursorPosition;
	Vector2 mCursorDirection;
	Vector2 mMoveDirection;
};

