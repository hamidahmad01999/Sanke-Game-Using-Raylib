#include <iostream>
#include"Game.h"
#include "raylib.h"
#include<raymath.h>
#include<fstream>
#include"SplashScreen.h"
#include"State.h"
#include"Menu.h"
using namespace std;



double lastUpdateTime = 0;

bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

int main() {
	State state;
	bool splashScreen = true;

	InitWindow(2 * state.offset + (state.cellCount * state.cellSize), 2 * state.offset + (state.cellCount * state.cellSize), "Snake Game Project");
	SetTargetFPS(60);
	Game game = Game(true,false,0,0);
	
	int frames = 0;
	SplashScreen sp = SplashScreen();
	
	game.SetRunning(true);
	
	while (WindowShouldClose() == false)
	{
		
		BeginDrawing();
		
		if (splashScreen) {
			sp.CreateSplachScreen();
			sp.Text1();
			sp.Draw();
			
			game.DrawGameName();
			if (frames<100) {
				frames++;
			}
			else {
				splashScreen = false;
			}
		}
		else if (splashScreen == false && game.menuShowed == false) {
			game.DrawGameName();
			game.MenuHandler();
		}
		else {
			game.Draw();
			if (IsKeyPressed(KEY_SPACE) && game.GetRunning() == true) {

				game.SetIsPaused(!game.GetIsPaused());
			}
			else if (game.GetRunning() == false && game.GetNextLevel() == false) {
				
				game.GameOver();
				if (IsKeyPressed(KEY_R)) {
					game.GameRestart();
				}
			}


			if (!game.GetIsPaused()) {
				if (eventTriggered(game.GetGameSpeed())) {
					
					game.Update();
				}
				Vector2 v = game.snake.GetDirection();
				
				if (IsKeyPressed(KEY_UP) && game.snake.GetDirection().y != 1) {
					
					Vector2 v = { 0,-1 };
					game.snake.SetDirection(v);
				}
				if (IsKeyPressed(KEY_DOWN) && game.snake.GetDirection().y != -1) {
					
					Vector2 v = { 0,1 };
					game.snake.SetDirection(v);
				}
				if (IsKeyPressed(KEY_RIGHT) && game.snake.GetDirection().x != -1) {
					
					Vector2 v = { 1,0 };
					game.snake.SetDirection(v);
				}
				if (IsKeyPressed(KEY_LEFT) && game.snake.GetDirection().x != 1) {
					
					Vector2 v = { -1,0 };
					game.snake.SetDirection(v);
				}
			}
		}
		
		if (game.closeGame == true) {
			break;
		}

		//game.getHighScore();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}