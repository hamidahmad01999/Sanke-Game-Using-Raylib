#include "Food.h"
#include"raylib.h"
#include<deque>
#include<iostream>
#include<raymath.h>
#include"State.h"




//constructor take snake body, hurdleBody,and number of hurdles and their size to make sure that food will not create on snake body and on hurdles
Food::Food(deque<Vector2> sBody,Vector2** hurdleB, int numberOfHurdles, int numberOfSquaresInHurdle) {
	position = GenerateRandomPosition(sBody, hurdleB, numberOfHurdles, numberOfSquaresInHurdle);
	bonusPosition = GenerateRandomPosition(sBody, hurdleB, numberOfHurdles, numberOfSquaresInHurdle);
	
	//loading image of food
	Image image = LoadImage("img/food.png");
	texture = LoadTextureFromImage(image);
	UnloadImage(image);

	bonusFood = false;
}

void Food::Draw() {
	//displaying food
	DrawTexture(texture, offset + position.x * cellSize, offset +position.y * cellSize, WHITE);	
		
}
void Food::DrawBonusFood() {
	//display bonus food
	DrawCircle(offset + bonusPosition.x * cellSize+ 15,15+ offset + bonusPosition.y * cellSize , cellCount/1.5 , RED);

}
Food::~Food() {
	UnloadTexture(texture);
}



//generating postion for bonus food and take snake body, hurdleBody,and number of hurdles and their size to make sure that food will not create on snake body and on hurdles
Vector2 Food::GenerateRandomPosition(deque<Vector2> snakeBody,  Vector2** hurdleB, int numberOfHurdles, int numberOfSquaresInHurdle) {
	Vector2 pos = GenerateRandomCell();

	for (int i = 0; i < numberOfHurdles; i++) {
		for (int j = 0; j < numberOfSquaresInHurdle; j++) {
			if (Vector2Equals(pos, hurdleB[i][j])) {
				pos = GenerateRandomCell();
			}
		}
	}
	if (ElementInDeque(pos, snakeBody)) {
		pos = GenerateRandomCell();
	}
	return pos;
}


//setter
void Food::SetPosition(Vector2 p) {
	position = p;
}

void Food::SetColorChoosen(int n) {
	colorChoosen = n;
}
void Food::SetBonusPosition(Vector2 p){ 
	bonusPosition = p;
}
void Food::SetBonusFood(bool b){
	bonusFood = b;
}

//getters

Vector2 Food::GetPosition()const {
	return position;
}
int Food::GetColorChoosen()const{
	return colorChoosen;
}
Vector2 Food::GetBonusPosition()const{
	return bonusPosition;
}
bool Food::GetBonusFood()const{
	return bonusFood;
}