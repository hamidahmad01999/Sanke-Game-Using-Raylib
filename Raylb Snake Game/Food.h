#ifndef __FOOD__
#define __FOOD__

#include<deque>
#include"raylib.h"
#include<iostream>
#include"State.h"
using namespace std;

//this class handle food related things of Game
class Food :public State{
	
	Vector2 position;	//it will have location of food like {5,6}. x=5 & y=6 here
	Texture texture;	//use to store image of food
						//actually image load into RAM and texture load into GCU

	int colorChoosen;	//to set color for food if needed otherwise we are creating food from image 
	Vector2 bonusPosition;	//store the location for bonus food
	bool bonusFood;			// whether bonusfood is present of secreen right now or not
public:

	//constructor take snake body, hurdleBody,and number of hurdles and their size to make sure that food will not create on snake body and on hurdles
	Food(deque<Vector2>, Vector2**, int, int );



	//setters 
	void SetPosition(Vector2);
	void SetColorChoosen(int);
	void SetBonusPosition(Vector2);
	void SetBonusFood(bool);

	//getters
	Vector2 GetPosition()const;
	int GetColorChoosen()const;
	Vector2 GetBonusPosition()const;
	bool GetBonusFood()const;

	//display
	void Draw();
	void DrawBonusFood();

	Vector2 GenerateRandomPosition(deque<Vector2>, Vector2** , int , int );

	~Food();
};

#endif