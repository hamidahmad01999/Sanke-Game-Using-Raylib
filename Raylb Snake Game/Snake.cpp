#include "Snake.h"
#include"raylib.h"
#include<deque>
#include<iostream>
#include<raymath.h>
#include"State.h"
using namespace std;


Snake::Snake(deque<Vector2> b , Vector2 d , bool addS) {
	body = b;
	direction = d;
	addSegment = addS;
	colorChoosen = 2; //by default light mood snake
}

void Snake::Draw() {
	//for displaying the snake getting size of snake body from deque and displaying on screen one by one
	for (unsigned int i = 0; i < body.size(); i++) {
		float x = body[i].x;
		float y = body[i].y;
		Rectangle rect = Rectangle{ offset + x * cellSize,offset + y * cellSize,(float)cellSize,(float) cellSize };

		if (i == 0) {
			//selectiong color pallete for face of snake
			if (colorChoosen == 1) {
				DrawRectangleRounded(rect, 0.5, 6, dark3);
			}
			else {
				DrawRectangleRounded(rect, 0.5, 6, darkerGreen);

			}
		}
		else {
			if (colorChoosen == 1) {
				DrawRectangleRounded(rect, 0.5, 6, dark2);
			}
			else {
				DrawRectangleRounded(rect, 0.5, 6, darkGreen);
			}
		}

	}
}



//setters
void Snake::SetBody(deque<Vector2> b) {
	body = b;
}
void Snake::SetDirection(Vector2 d) {
	direction = d;
}
void Snake::SetAddSegment(bool s) {
	addSegment = s;
}
void Snake::SetColorChoosen(int c){
	colorChoosen = c;
}


//getters
deque<Vector2> Snake::GetBody() const{
	return body;
}

Vector2 Snake::GetDirection()const{
	return direction;
}

bool Snake::GetAddSegment()const {
	return addSegment;
}
int Snake::GetColorChoosen()const {
	return colorChoosen;
}

void Snake::Update() {
	//as body is a deque data type of Vector2 so we can append and pop it 
	//it's logic for snake movement and growing snake
	//for movement we are adding each segement at start/front/mouth of snake
	body.push_front(Vector2Add(body[0], direction));

	//if don't grow snake(it didn't eat food then remove last segment of snake body)
	//if we have to grow snake then after adding a segment at start don't remove last segment of snake body
	if (addSegment) {
		addSegment = false; //jsut false condtion to add segment
	}
	else {
		body.pop_back();	//removing last segment as for movement we added a segment at front
	}
	
}
