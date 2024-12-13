#include "Menu.h"
#include<iostream>
#include"raylib.h"
using namespace std;



void Menu::CreateMunuScreen(Color color) {
	ClearBackground(color);
}

Rectangle Menu::DrawButton(float pointX,float  pointY,float width, float height,int fontSize, const char* text, Color btnColor, Color txtColor) {
	Rectangle rec = Rectangle{ pointX, pointY, width, height };

	DrawRectangleRec(rec, btnColor);

	DrawText(text, pointX + 50, pointY + 10, fontSize, txtColor);
	return rec;
}

bool Menu::IsButtonClicked(Rectangle button) {
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
		return true;
	}
	else {
		return false;
	}
}