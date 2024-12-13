#ifndef __SNAKE__
#define __SNAKE__

#include"raylib.h"
#include<deque>
#include<iostream>
#include"State.h"
using namespace std;

class Snake :public State{
	deque<Vector2> body = { Vector2{6,9},  Vector2{5,9},  Vector2{4,9} };	//body of snake, deque works like array but it allow us to pop and push from start and end

	Vector2 direction = { 1,0 };	//direction in which snake have to move we will change it in our code by replacing it with {-1,0} for left moving, {0,1} for downward moving and {0,-1} for upward moving

	bool addSegment = false;		//tell if increase the size of snake or not, it will be true when snake eat food to increment size by one segment then again it will be false

	int colorChoosen;				//color pallet selected for snake body 1 for dark, 2 for light mode

public:
	// constructor taking default starting body of snake and tell whether add new segment to it or not
	// second argument is direction in whcih we want to move it initally by default we are moving it in right direction
	Snake(deque<Vector2> b= { Vector2{6,9},  Vector2{5,9},  Vector2{4,9} }, Vector2 d= { 1,0 }, bool addS=false);

	//setters
	void SetBody(deque<Vector2>);	
	void SetDirection(Vector2);
	void SetAddSegment(bool);
	void SetColorChoosen(int);

	//getters
	deque<Vector2> GetBody()const;		
	Vector2 GetDirection()const;
	bool GetAddSegment()const;
	int GetColorChoosen()const;


	//it will draw snake on screen
	void Draw();

	//it will update the movement of snake
	//it will handle 
	//logic for snake movement and growing snake
	void Update();
};



#endif 

