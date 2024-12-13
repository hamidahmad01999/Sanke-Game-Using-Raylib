#include "Game.h"
#include"fstream"
#include"raylib.h"
#include<deque>
#include<iostream>
#include<raymath.h>
using namespace std;

Game::Game( bool r , bool isP , int sc , int hScore):State() {
	
	running = r;
	isPaused = isP;
	score = sc;
	highScore = hScore;
	InitAudioDevice();
	eatSound = LoadSound("sounds/eat.mp3");
	collisionSound = LoadSound("sounds/wall.mp3");
	backgroundMusic = LoadSound("sounds/back_music.mp3");
	cheers = LoadSound("sounds/cheers.mp3");
	musicPlayedLasttime = 0;
	gameSpeed = 0.2;
	foodEated = 0;
	noOfFoodRequireForNextLevel = 10;
	LevelDrawHandler();
	MusicPlay();
	colorChoosen = 2;//light mode selected by default
	difficultyLevel=1; //1:Easy, 2:Medium, 3:Hard
	showingDifficultyLevel=false;
	difficultyLevelChange=false;
}

Game::~Game() {
	UnloadSound(eatSound);
	UnloadSound(collisionSound);
	UnloadSound(backgroundMusic);
	CloseAudioDevice();
}

void Game::DrawHelper() {
	//all of this till Menu handler is only for showing state where game paused
	snake.Draw();
	food.Draw();
	if (nextLevel == false) {
		level.Draw(false, requireFood - foodEated);
	}

	Rectangle rec = Rectangle{ (float)(offset - 5), (float)(offset - 5), (float)((cellSize * cellCount) + 10),(float)(cellSize * cellCount + 10) };
	//boundary and score
	if (colorChoosen == 1) {

		ClearBackground(dark1);
		DrawRectangleLinesEx(rec, float(5), dark2);
		DrawText(TextFormat("%i", score), offset - 5, 10 + offset + cellSize * cellCount, 40, dark3);
	}
	else if (colorChoosen == 2) {
		ClearBackground(green);
		DrawRectangleLinesEx(rec, float(5), darkGreen);
		DrawText(TextFormat("%i", score), offset - 5, 10 + offset + cellSize * cellCount, 40, darkerGreen);

	}
}

void Game::DrawGameName() {
	DrawText("Snake Adventure", 75, 30, 30, WHITE);
}

//drawing each element like snake, food and hurldes 
void Game::Draw() {
	if (running == false && nextLevel==false) {
		DrawHelper();
		MenuHandler();
		DrawGameName();
	}
	else if (!isPaused) {

		DrawHelper();

		currentTime = GetTime();
		//in easy mode bonus food will only display for 5sec in medium for 4sec and in hard it will only display for 1.75 sec
		//if there is a bonus food on screen but user didn't eat bonus food and eat normal food to go to next level then the bonus food of previos level will not show on next level
		if (difficultyLevel == 1) {
			if (currentTime - lastExecutionOfBonusFood >= 5.0 || nextLevel==true) {
			
				food.SetBonusFood(false);
			}
		}
		else if (difficultyLevel == 2) {
			if (currentTime - lastExecutionOfBonusFood >= 4.0 || nextLevel == true) {
				food.SetBonusFood(false);
			}
		}
		else if (difficultyLevel == 3) {
			if (currentTime - lastExecutionOfBonusFood >= 1.75 || nextLevel == true) {
				food.SetBonusFood(false);
			}
		}

		if (food.GetBonusFood() == true) {
			food.DrawBonusFood();
		}
		highScore = GetHighScore();
		DrawText("HighScore:", (cellSize * cellCount) / 2-50, (cellSize * cellCount) + offset + 20, 30, WHITE);
		DrawText(TextFormat("%i", highScore), ((cellSize * cellCount) / 2)+130, (cellSize * cellCount) + offset + 20, 30, WHITE);

		if (GetTime() - musicPlayedLasttime >= 240) {
			MusicPlay();
		}
		if (nextLevel == true) {
			 
			NextLevelHandler();
			running = false;
			if (flag == false) {
				gameSpeed = gameSpeed - 0.01; //increasing game speed for each level increase by incrementing number of frames in each second

				level.Update(level.GetLevel() + 1);

				deque<Vector2> b = { Vector2{ 6,9 }, Vector2{ 5,9 }, Vector2{ 4,9 } };
				Vector2 d = { 1, 0 };
				for (int i = 0; i < level.GetLevel(); i++) {
					b.push_front(Vector2Add(b[0], d));
				}
				snake.SetBody(b);
				snake.SetDirection(d);
				LevelDrawHandler();
			}
			flag = true;


			if (GetTime() - levelCompletionTime > 5) {

				levelCompletionTime = 0;
				nextLevel = false;
				running = true;
				flag = false;
				StopSound(cheers);
				PlaySound(backgroundMusic);
			}
		}
		DrawGameName();
	}
	else if (isPaused) {
		GamePause();
		DrawGameName();
	}
	
}

//it's for playing background music
void Game::MusicPlay() {
	PlaySound(backgroundMusic);
	SetSoundVolume(backgroundMusic, 0.5);
}
		
//it's for color mode selection
void Game::ColorModeHandler() {
	if (colorChoosen == 1) {
		food.SetColorChoosen(1);
		snake.SetColorChoosen(1);
		level.SetColorChoosen(1);
	}
	else {
		level.SetColorChoosen(2);
		food.SetColorChoosen(2);
		snake.SetColorChoosen(2);
	}
}


void Game::Update() {
	if (running == true) {
		snake.Update();
		CheckCollisionWithFood();
		CheckCollisionWithEdges();
		CheckCollisionWithTale();
		CheckCollisionWithHurdles();
		

		
	}
	else if (running == false && gameCompleted == true) {
		GameCompleted();
	}
	
}


void Game::GameOver() {
	UpdateHighScore();
	DrawText("Game Over", 300, 70, 30, RED);
	DrawText("Thanks for playing this game", 150, 120, 30, WHITE);
	DrawText("Click \"R\" to restart the Game.", 150, 150, 30, BLACK);
	running = false;
	isPaused = false;
	food.SetBonusFood(false);
	foodEated = 0;
}

void Game::GamePause() {
	DrawText("Paused", 330, 70, 30, RED);
	DrawText("Again press space to continue.", 150, 100, 30, WHITE);
	ClearBackground(dark1);
	MenuHandler();
}

//if game restart stop background music, setScore to 0, create snake again 
void Game::GameRestart() {
	PlaySound(backgroundMusic);
	level.GameOver(difficultyLevel);
	LevelDrawHandler();
	deque<Vector2> b= { Vector2{ 6,9 }, Vector2{ 5,9 }, Vector2{ 4,9 } };
	snake.SetBody(b);
	Vector2 d = { 1, 0 };
	snake.SetDirection(d);
	running = true;
	score = 0;
	foodEated = 0;
	//gameSpeed = 0.2;
	if (difficultyLevel == 1) {
		gameSpeed = 0.2;
	}
	else if (difficultyLevel == 2) {
		gameSpeed = 0.15;
	}
	else if (difficultyLevel == 3) {
		gameSpeed = 0.08;
	}
}
void Game::NextLevelHandler() {
	//running = false;
	DrawText("Next Level", 150, 440, 30, BLACK);
	DrawText("Congratulations! Level Completed", 150, 400, 30, WHITE);

	
}

//handle all kind of food related work
void Game::CheckCollisionWithFood() {
	//check whether snake eated food or not  if eated then go inside
	if (Vector2Equals(snake.GetBody()[0], food.GetPosition())) {
		collisionWithHurdle = true;

		// if snake eat food then generate a new position for food
		Vector2 p = food.GenerateRandomPosition(snake.GetBody(), level.hurdleBody, level.GetNumberOfHurdles(),level.GetNoOfHurdlesSquare());
		//set that position for food
		food.SetPosition(p);

		//and increase size of food
		snake.SetAddSegment(true);
		score++;
		foodEated++;

		//create bonus food after eating 3 foods
		if (foodEated % 3 == 0 && food.GetBonusFood()==false) {

			food.SetBonusPosition(food.GenerateRandomPosition(snake.GetBody(), level.hurdleBody, level.GetNumberOfHurdles(), level.GetNoOfHurdlesSquare()));
			food.SetBonusFood(true);
			lastExecutionOfBonusFood = GetTime();
		}
		PlaySound(eatSound);
		
		
	}
	//check if bonus food is created and snake eated  bonus food then increase score by 5
	if (food.GetBonusFood() == true && Vector2Equals(snake.GetBody()[0], food.GetBonusPosition())) {
		score += 5;
		food.SetBonusFood(false);
		PlaySound(eatSound);
		PlaySound(eatSound);
	}
	//noOfFoodRequireForNextLevel = 2;
	//set number of food requires to eat in this level to move to next level
	requireFood = noOfFoodRequireForNextLevel+(level.GetLevel() *noOfFoodRequireForNextLevel);
	
	//&& flag==false && foodEated!=prevLevelFoodValue
	//if snake eated require food of this level then send him to next level
	if (foodEated % requireFood == 0 && foodEated!=0 && nextLevel==false) {
		levelCompletionTime = GetTime();
		nextLevel = true;
		prevLevelFoodValue = foodEated;
		foodEated = 0;

		PlaySound(cheers);
		StopSound(backgroundMusic);
	}
	if (foodEated >= 50 && level.GetLevel()==10) {
		gameCompleted = true;
		running = false;
	}
	
}

void Game::GameCompleted() {

	DrawText("Game Completed", 300, 370, 30, RED);
	DrawText("You are a Champ Hurah", 300, 370, 30, RED);
	DrawText("Thanks for playing this game", 150, 400, 30, WHITE);
	DrawText("Click \"R\" to restart the Game.", 150, 440, 30, BLACK);
	if (IsKeyPressed(KEY_R)) {
		GameRestart();
	}

}

//cehck whether snake collide with boundary/wall or not
void Game::CheckCollisionWithEdges() {
	if (snake.GetBody()[0].x == cellCount || snake.GetBody()[0].x == -1) {
		
		running = false;
		UpdateHighScore();
		StopSound(backgroundMusic);
		PlaySound(collisionSound);
	}
	else if (snake.GetBody()[0].y == cellCount || snake.GetBody()[0].y == -1) {
		
		running = false;
		UpdateHighScore();
		StopSound(backgroundMusic);
		PlaySound(collisionSound);
	}
}

//check whether snake is collided with it's tale or not
void Game::CheckCollisionWithTale() {
	deque<Vector2> headlessSnake = snake.GetBody();
	headlessSnake.pop_front();
	if (ElementInDeque(snake.GetBody()[0], headlessSnake)) {
		running = false;
		UpdateHighScore();
		StopSound(backgroundMusic);
		PlaySound(collisionSound);
	}
}

//it handle all kind of hurdles related work in game
void Game::LevelDrawHandler() {
	//generate hurdles
	level.GenerateHurdlePosition();
	bool rightHurlde = false;
	
	//here making sure thesse things
	// firstly it's making sure that hurdle shoud not create on snakes tale or on food
	// then
	


	while (rightHurlde==false)
	{
		for (int i = 0; i < level.GetNumberOfHurdles(); i++) {
			for (int j = 0; j < level.GetNoOfHurdlesSquare(); j++) {
				//it will not allow to create hurdle on snake's body
				if (Vector2Equals(level.hurdleBody[i][j], snake.GetBody()[0]) ||
					Vector2Equals(level.hurdleBody[i][j], snake.GetBody()[1]) ||
					Vector2Equals(level.hurdleBody[i][j], snake.GetBody()[2]) ||
					Vector2Equals(level.hurdleBody[i][j], food.GetPosition()) ||
					((snake.GetBody()[0].x < level.hurdleBody[i][j].x + 7) &&
						snake.GetBody()[0].y==level.hurdleBody[i][j].y)

					) {
					level.GenerateHurdlePosition();
					j = 0;//make i=0 to check once again the position of hurdle
				}

				//it will not allow to create hurdle on anoter hurdle
				for (int k = 0; k < i; k++) {
					for (int l = 0; l < level.GetNoOfHurdlesSquare(); l++) {
						if (Vector2Equals(level.hurdleBody[k][l], level.hurdleBody[i][j]) )
						{
							level.GenerateHurdlePosition();
							j = 0;
						}
					}
				}

			}
			
		}
		rightHurlde = true;
	}
	
}

//check if snake collide with hurdle then game will be over
void Game::CheckCollisionWithHurdles() {

	for (int i = 0; i < level.GetNumberOfHurdles(); i++) {
		for (int j = 0; j < level.GetNoOfHurdlesSquare(); j++) {

			if (Vector2Equals(snake.GetBody()[0], level.hurdleBody[i][j])) {
				StopSound(backgroundMusic);
				PlaySound(collisionSound);
				GameOver();
			}
		}
	}

}

//MenuHandler
void Game::MenuHandler() {
	if (!colorModedShowing && !showingDifficultyLevel) {
		menu.CreateMunuScreen(dark1);
		Rectangle pauseBtn, newGameBtn, startBtn, colorMode, backgroundMusicBtn, exitBtn, difficultyBtn;
		int newButtonYPoint = ((cellSize * cellCount) / 3);
		if (isPaused) {

			if (running) {
				if (showContinueButton){
					newButtonYPoint = ((cellSize * cellCount) / 3) - 100;
					pauseBtn = menu.DrawButton(((cellSize * cellCount) / 2), ((cellSize * cellCount) / 3), 200, 50, 20, "Continue Game", RED, WHITE);
					if (menu.IsButtonClicked(pauseBtn)) {
						SetIsPaused(!GetIsPaused());
						//isPaused = false;//don't uncomment it
						return;
					}
				}
				
			}

			newGameBtn = menu.DrawButton((cellSize * cellCount) / 2, newButtonYPoint, 200, 50, 20, "New Game", RED, WHITE);
			
			if (menu.IsButtonClicked(newGameBtn)) {
				isPaused = false;
				//menuShowed = true;//don't uncomment it
				difficultyLevelChange = false;
				showContinueButton = true;
				GameRestart();
				return;
			}

		}

		if (!isPaused ) {
			startBtn = menu.DrawButton((cellSize * cellCount) / 2, (cellSize * cellCount) / 3, 200, 50, 20, "Start Game", RED, WHITE);
			if (menu.IsButtonClicked(startBtn)) {
				menuShowed = true;
				GameRestart();
				return;
			}
		}

		colorMode = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 100, 200, 50, 20, "Select Mode", RED, WHITE);

		if (IsSoundPlaying(backgroundMusic)) {
			backgroundMusicBtn=menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 200, 200, 50, 20, "Off Music", RED, WHITE);
			if (menu.IsButtonClicked(backgroundMusicBtn)) {
				StopSound(backgroundMusic);

			}
		}
		else {
			backgroundMusicBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 200, 200, 50, 20, "On Music", RED, WHITE);
			if (menu.IsButtonClicked(backgroundMusicBtn)) {
				PlaySound(backgroundMusic);

			}
			
		}
		

		difficultyBtn= menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 300, 200, 50, 20, "Difficulty", RED, WHITE);
		exitBtn= menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 400, 200, 50, 20, "Exit Game", RED, WHITE);

		if (menu.IsButtonClicked(colorMode)) {
			ColorModeSelector();
			colorModedShowing = true;
		}
		else if (menu.IsButtonClicked(exitBtn)) {
			closeGame = true;
		}
		else if (menu.IsButtonClicked(difficultyBtn)) {
			showingDifficultyLevel = true;
			DifficultySelector();
		}
		 
	}
	else if(colorModedShowing==true){
		ColorModeSelector();
	}
	else if (showingDifficultyLevel == true) {
		DifficultySelector();
	}
	

}

//Difficulty LevelHandler
void Game::DifficultyLevelHandler(int difficulty) {
	difficultyLevel = difficulty;
	if (difficultyLevel == 1) {
		level.UpdateNumberOfHurdles(2,5);
		gameSpeed = 0.2;
	}
	else if (difficultyLevel == 2) {
		level.UpdateNumberOfHurdles(2,7);
		gameSpeed = 0.15;
	}
	else if (difficultyLevel == 3) {
		level.UpdateNumberOfHurdles(2,8);
		gameSpeed = 0.08;
	}
	cout << "Difficulty level set to " << difficultyLevel << endl;
	difficultyLevelChange = true;
	showContinueButton = false;
}

void Game::DifficultySelector() {
	menu.CreateMunuScreen(dark2);
	DrawText("Select Difficulty Level", 150, 150, 30, WHITE);
	Rectangle easyBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 50, 200, 50, 20, "Easy", RED, WHITE);

	Rectangle mediumBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 150, 200, 50, 20, "Medium", RED, WHITE);

	Rectangle hardBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 250, 200, 50, 20, "Hard", RED, WHITE);

	Rectangle backBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 350, 200, 50, 20, "Back", RED, WHITE);


	if (menu.IsButtonClicked(easyBtn)) {

		DifficultyLevelHandler(1);
	}
	else if (menu.IsButtonClicked(mediumBtn)) {
		
		DifficultyLevelHandler(2);

	}
	else if (menu.IsButtonClicked(hardBtn)) {
		DifficultyLevelHandler(3);

	}
	else if (menu.IsButtonClicked(backBtn)) {
		showingDifficultyLevel = false;
	}


	if (difficultyLevel == 1) {
		DrawText("Difficulty level set to easy", 150, cellSize * cellCount, 30, WHITE);

	}
	else if (difficultyLevel == 2) {
		DrawText("Difficulty level set to medium", 150, cellSize * cellCount, 30, WHITE);
	}
	else if (difficultyLevel == 3) {
		DrawText("Difficulty level set to hard", 150, cellSize * cellCount, 30, WHITE);
	}
}

void Game::ColorModeSelector() {
	menu.CreateMunuScreen(dark2);
	DrawText("Select Color Mode for your game", 150, 150, 30, WHITE);
	Rectangle lightMode = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3)+50, 200, 50, 20, "Light Mode", RED, WHITE);

	Rectangle darkMode = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 150, 200, 50, 20, "Dark Mode", RED, WHITE);

	Rectangle backBtn = menu.DrawButton((cellSize * cellCount) / 2, ((cellSize * cellCount) / 3) + 250, 200, 50, 20, "Back", RED, WHITE);
	if (menu.IsButtonClicked(lightMode)) {
		
		colorChoosen = 2;
		
	}
	else if(menu.IsButtonClicked(darkMode)){
		colorChoosen = 1;
		
	}
	else if (menu.IsButtonClicked(backBtn)) {
		colorModedShowing = false;

	}

	if (colorChoosen == 2) {
		DrawText("Light Mode Selected", 150, cellSize*cellCount, 30, WHITE);

	}
	else if (colorChoosen == 1) {
		DrawText("Dark Mode Selected", 150, cellSize*cellCount, 30, GRAY);
	}
}

//manage high score
void Game::UpdateHighScore() {
	ifstream fin;
	ofstream fout;
	fin.open("highscore.txt");
	fout.open("highscore.txt");
	int hScore;
	if (!fin || !fout) {
		cout << "File didn't open" << endl;
	}
	else {
		if (highScore < score) {
			fout << score;
		}
		else {
			fout << highScore;
		}
	}
	fin.close();
	fout.close();
}
int Game::GetHighScore() {
	try {
		ifstream fin;
		int hScore;
		fin.open("highscore.txt");
		if (!fin) {
			cout << "File didn't open" << endl;
		}
		else {

			fin >> hScore;
		}
		return hScore;
	}
	catch (...) {
		int hScore = -1;
		return hScore;
	}
	
}


//setters
void Game::SetRunning(bool r) {
	running = r;
}
void Game::SetIsPaused(bool p) {
	isPaused = p;
}
void Game::SetScore(int s) {
	score = s;
}
void Game::SetColorChoosen(int c) {
	colorChoosen = c;
}
void Game::SetGameSpeed(double s) {
	gameSpeed = s;
}
void Game::SetNextLevel(bool l) {
	nextLevel = l;
}
void Game::SetDifficultyLevel(int d) {
	difficultyLevel = d;
}

//getters
bool Game::GetRunning()const {
	return running;
}
bool Game::GetIsPaused()const {
	return isPaused;
}
int Game::GetScore()const {
	return score;
}
int Game::GetColorChoosen()const {
	return colorChoosen;
}
double Game::GetGameSpeed()const {
	return gameSpeed;
}
bool Game::GetNextLevel()const {
	return nextLevel;
}
int Game::GetDifficultyLevel()const {
	return difficultyLevel;
}