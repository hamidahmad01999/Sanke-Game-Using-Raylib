#ifndef __STATE__
#define __STATE__
#include"raylib.h"
#include<deque>
#include<iostream>
#include"raylib.h"
#include"raymath.h"
using namespace std;

//this class is created for that variables and function which i would need in many classes so I can get that variables or functions from this class instead of creating again and again
class State
{
public:
	
	Color dark1 = { 62,62,66,255 };
	Color green = { 173,204,96,255 };
	Color darkGreen = { 43,51,24,255 };
	Color dark2 = { 30,30,30,255 };
	Color darkerGreen = { 43,100,24,255 };
	Color dark3 = { 0,122,204,255 };


	int cellSize = 30;
	int cellCount = 25;
	int offset = 75;
	
	bool ElementInDeque(Vector2 cell, deque<Vector2> deque) {
		for (unsigned int i = 0; i < deque.size(); i++) {
			if (Vector2Equals(deque[i], cell)) {
				return true;
			}
		}
		return false;
	}
	Vector2 GenerateRandomCell(int num=0) {
		int x = GetRandomValue(0, cellCount - 1-num);
		int y = GetRandomValue(0, cellCount - 1);


		Vector2 pos = Vector2{ float(x), float(y) };
		return pos;
	}
};

#endif