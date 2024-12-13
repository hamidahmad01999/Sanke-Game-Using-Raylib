#pragma once
#include"raylib.h"
#include"State.h"
class Menu:public State
{
public:
	void CreateMunuScreen(Color);
	Rectangle DrawButton(float pointX, float  pointY, float width, float height, int fontSize, const char* text, Color btnColor, Color txtColor);

	bool IsButtonClicked(Rectangle button);
};

