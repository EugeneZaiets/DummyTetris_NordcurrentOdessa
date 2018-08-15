#pragma once
#include<windows.h>
class TetrisObject{
	int _TObject[7][4][5][5];
	int _pos[7][4][2];
public:
	TetrisObject();
	~TetrisObject();
	int GetInitPosX(int TObject, int Rotation);
	int GetInitPosY(int TObject, int Rotation);
	int GetBlock(int TObject, int Rotation, int X, int Y);
};