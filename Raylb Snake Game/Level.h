#ifndef __LEVEL__
#define __LEVEL__
#include"State.h"
#include<deque>
#include"raylib.h"
#include<iostream>
using namespace std;

class Level:public State {
	int level;
	int numberOfHurdles;
	int noOfHurdlesSquare;
	int colorChoosen;
public:
	
	Vector2** hurdleBody;

	Level();
	void Draw(bool, int);
	void Update(int);
	void GenerateHurdlePosition();

	//setters
	void SetLevel(int);
	void SetNumberOfHurdles(int);
	void SetNoOfHurdlesSquare(int);
	void SetColorChoosen(int);

	//getters
	int GetLevel()const;
	int GetNumberOfHurdles()const;
	int GetNoOfHurdlesSquare()const;
	int GetColorChoosen()const;

	void UpdateNumberOfHurdles(int , int );
	void GameOver(int);

	~Level();
};


#endif // !__LEVEL__


