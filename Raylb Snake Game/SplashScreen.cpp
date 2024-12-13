#include "SplashScreen.h"
#include"raylib.h"
#include<deque>
#include<iostream>
#include<raymath.h>


SplashScreen::SplashScreen() {
	cout << "----------------------------------------------------" << endl;
	Image image = LoadImage("img/logo.png");
	//actually use image in texture form
	texture = LoadTextureFromImage(image);
	UnloadImage(image);
}
void SplashScreen::Draw() {
	DrawTexture(texture, (cellSize * cellCount) - 400, (cellSize * cellCount) -100, WHITE);
}
void SplashScreen::Text1() {
	DrawText("OOP Project", (cellSize * cellCount) / 2 - 20, (cellSize *cellCount) / 2, 40, WHITE);
	DrawText("Snake Game Using Raylib", ((cellSize * cellCount) / 2) - 70,(cellSize * cellCount) / 2 + 50, 40, WHITE);
	DrawText("By: Hamid AHmad & M.Aaqil", ((cellSize * cellCount) / 2) - 70,(cellSize * cellCount) / 2 + 100, 40, WHITE);
}


void SplashScreen::CreateSplachScreen() {
	ClearBackground(darkerGreen);
}