#include "Level.h"
#include"raylib.h"
#include<iostream>
using namespace std;


Level::Level() {
	level = 0;
	noOfHurdlesSquare = 5;
	numberOfHurdles = 2;
	colorChoosen = 2;
	hurdleBody = new Vector2* [numberOfHurdles];
	for (int i = 0; i < numberOfHurdles; i++) {
		hurdleBody[i] = new Vector2[noOfHurdlesSquare];
	}
}

void Level::Draw(bool update, int food) {

	for (int i = 0; i <numberOfHurdles; i++) {
		for (int j = 0; j < noOfHurdlesSquare; j++) {

		int x = hurdleBody[i][j].x;
		int y = hurdleBody[i][j].y;
		Vector2 pos{ x,y };

		DrawRectangle((offset + x * cellSize), offset + y * cellSize, cellSize, cellSize, BLUE);
		} 
	}
	if (colorChoosen == 1) {
		DrawText("Level: ", cellSize * cellCount - 70, 10, 40, dark2);
		DrawText(TextFormat("%i", level), cellCount * cellSize + 50, 10, 40, dark2);
		DrawText("Food Left:  ", (cellSize * cellCount)-130, (cellSize * cellCount) + offset+20, 40, dark2);
		DrawText(TextFormat("%i", food), (cellSize * cellCount)+offset+20, (cellSize * cellCount) + offset+20, 40, dark2);
	}
	else {
		DrawText("Level: ", cellSize * cellCount - 70, 10, 40, darkerGreen);
		DrawText(TextFormat("%i", level), cellCount*cellSize+50, 10, 40, darkerGreen);
		DrawText("Food Left:  ", (cellSize * cellCount)-130, (cellSize * cellCount) + offset+20, 40, darkerGreen);
		DrawText(TextFormat("%i", food), (cellSize * cellCount) + offset+20, (cellSize * cellCount) + offset+20, 40, darkerGreen);
	}

}


//generating hurdles 
//no more than one hurdles can create single line
//and if col1 has a hurdle than column next to this one can't have hurdle but 2nd col to this one can have hurdle
void Level::GenerateHurdlePosition() {
	Vector2 pos;
	for (int i = 0; i < numberOfHurdles; i++) {
		bool skip = false; 

		pos = GenerateRandomCell(noOfHurdlesSquare);
		//making sure that first hurdle will not create on row of snake initial row where snake have to create 
		//next loop code is handling not to create any hurdle in row of snake except first hurdle
		if (i == 0) {
			if (pos.y == 9) {
				i--;
				continue;
			}
		}

		for (int l = 0; l < i; l++) {
			//current hurdle is at lower pos
			//current hurdle at upper pos

			//checking position of square of hurdle and should not allow another hurdle to create next to it col 
			//if first col has hurdle then second one will not have but third can have hurdle
			//cehcking if hurdle is creating on bottom and comparing with upper one
			if (pos.y == 9) {
				i--;
				break;
			}
			if (pos.y > hurdleBody[l][0].y) {
				if ((pos.y - hurdleBody[l][0].y < 2)) {
					i--;
					skip = true;
					break;
				}
			}
			else if (pos.y < hurdleBody[l][0].y) {
			//cehcking if hurdle is creating on upper and comparing with lower one

				if ((hurdleBody[l][0].y- pos.y < 2)) {
					i--;
					skip = true;
					break;
				}
			}
		}
		//it should be true if we want to skip itteration because this itteration creating hurdle in same column in which hurdle already created or next column to this hurdle already created
		if (skip == true) {
			continue;
		}
		//if there is no hurdle created to next line to this hurdle then it can be create otherwise loop will not reach here
		for (int j = 0; j < noOfHurdlesSquare; j++) {
			pos.x = pos.x + 1;
			hurdleBody[i][j]=pos;
		}
	}

}

void Level::Update(int l) {
	int updateHurdles = (l) + 2;

	int addSquareInHurdles = 1;
	UpdateNumberOfHurdles(updateHurdles, noOfHurdlesSquare+addSquareInHurdles);
	level++;
}

void Level::GameOver(int difficulty=1) {
	level = 0;
	//UpdateNumberOfHurdles(2, 5);//I have to change it-->changed from 8 to 5
	if (difficulty == 1) {
		UpdateNumberOfHurdles(2, 5);//I have to change it-->changed from 8 to 5

	}
	else if (difficulty == 2) {
		UpdateNumberOfHurdles(2, 7);//I have to change it-->changed from 8 to 5

	}
	else if (difficulty == 3) {
		UpdateNumberOfHurdles(2, 8);//I have to change it-->changed from 8 to 5

	}
}


//as level increase we have to incease hurdle and number of squares in hurdle 
void Level::UpdateNumberOfHurdles(int h, int s) {
	// Deallocate previous memory if any
	if (hurdleBody != nullptr) {
		for (int i = 0; i < numberOfHurdles; i++) {
			delete[] hurdleBody[i];
		}
		delete[] hurdleBody;
		hurdleBody = nullptr;
	}

	// Update number of hurdles and squares
	numberOfHurdles = h;
	noOfHurdlesSquare = s;

	// Allocate new memory
	hurdleBody = new Vector2 * [numberOfHurdles];
	for (int i = 0; i < numberOfHurdles; i++) {
		hurdleBody[i] = new Vector2[noOfHurdlesSquare];
	}
}

//setters
void Level::SetLevel(int l) {
	level = l;
}
void Level::SetNumberOfHurdles(int h) {
	numberOfHurdles = h;
}
void Level::SetNoOfHurdlesSquare(int h){
	noOfHurdlesSquare = h;
}
void Level::SetColorChoosen(int n) {
	colorChoosen = n;
}

//getters
int Level::GetLevel()const {
	return level;
}
int Level::GetNumberOfHurdles()const{
	return numberOfHurdles;
}

int Level::GetNoOfHurdlesSquare()const{
	return noOfHurdlesSquare;
}
int Level::GetColorChoosen()const {
	return colorChoosen;
}

Level::~Level() {
	if (hurdleBody != nullptr) {
		for (int i = 0; i < numberOfHurdles; i++) {
			delete[]hurdleBody[i];
			hurdleBody[i] = nullptr;
		}
		delete[] hurdleBody;
		hurdleBody = nullptr;
	}
}