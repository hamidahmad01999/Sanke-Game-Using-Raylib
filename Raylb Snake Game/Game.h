#ifndef __GAME__
#define __GAME__
#include"Snake.h"
#include"Food.h"
#include"Level.h"
#include"Menu.h"

class Game :public State{
	int foodEated;	//it will keep track that how many food snake eated in single level
	bool running;	//it will be true is player is not out and false in case of game over
	bool isPaused;	//tell whether game is pause or not
	int score;		//store score
	int highScore;	//it will store high score still not worked on this
	Sound eatSound;	//sound when snake eat fooe
	Sound collisionSound;	//sound when game over
	Sound backgroundMusic;	//background music
	Sound cheers;			//cheers on level completion sound
	bool gameCompleted;		//if player played all levels then it will be true
	int noOfFoodRequireForNextLevel;
	int requireFood;
	
	int colorChoosen;
	double gameSpeed;
	double musicPlayedLasttime; //I have to fix it --->changed from bool to double data type
	int prevLevelFoodValue=0;
	bool nextLevel = false;
	bool collisionWithHurdle=true;
	Level level;
	double lastExecutionOfBonusFood = 0;
	double currentTime = 0;
	bool flag = false;
	double levelCompletionTime = 0;
	Menu menu;
	int difficultyLevel; //1:Easy, 2:Medium, 3:Hard
	bool showingDifficultyLevel;
	bool difficultyLevelChange;
	bool showContinueButton = true;
public:
	bool closeGame = false;
	bool menuShowed=false;
	bool colorModedShowing = false;
	Snake snake=Snake();
	Food food =Food(snake.GetBody(), level.hurdleBody, level.GetNumberOfHurdles(), level.GetNoOfHurdlesSquare());


	Game(bool r = true, bool isP = false, int sc = 0, int hScore = 0);

	~Game();

	//setter 
	void SetRunning(bool);
	void SetIsPaused(bool);
	void SetScore(int);
	void SetColorChoosen(int);
	void SetGameSpeed(double);
	void SetNextLevel(bool);
	void SetDifficultyLevel(int);

	//getter
	bool GetRunning()const;
	bool GetIsPaused()const;
	int GetScore()const;
	int GetColorChoosen()const;
	double GetGameSpeed()const;
	bool GetNextLevel()const;
	int GetDifficultyLevel()const;

	
	void Draw();

	void Update();

	void GameOver();

	void GamePause();

	void GameRestart();

	void CheckCollisionWithFood();

	void CheckCollisionWithEdges();

	void CheckCollisionWithTale();

	void CheckCollisionWithHurdles();

	void UpdateHighScore();

	int GetHighScore();

	void DrawHelper();

	//this function will make sure hurdle will not create on position where food is located or snake body or on the walls

	void MusicPlay();
	void GameCompleted();

	//Handlers
	void LevelDrawHandler();
	void NextLevelHandler();
	void ColorModeHandler();
	void MenuHandler();
	void DifficultyLevelHandler(int);

	void ColorModeSelector();
	void DifficultySelector();
	void DrawGameName();
};

#endif
