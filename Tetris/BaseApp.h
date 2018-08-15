// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>
#include <string>
#include "TetrisObject.h"

using namespace std;

#define WindowInfoFail 2
#define ConsoleBuffSizeFail 3
class BaseApp{
	//Console Engine
	HANDLE mConsole;
	HANDLE mConsoleIn;
	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;
	void Render();
public:
	int textspace;
	const int X_SIZE = 20;
	const int Y_SIZE = 20;
	std::vector<std::vector<int>>Field;
	TetrisObject* TObject;

	//Main functions
	BaseApp(TetrisObject* T, int xSize = 20, int ySize = 20);
	virtual ~BaseApp();
	virtual void UpdateF(float deltaTime){}
	virtual void KeyPressed(int btnCode){}
	void Run();

	//Game Logic
	bool StoreTObject(int X, int Y, int TObject, int Rotation);
	void DeleteLine(int Y);
	bool NeedToDelete();
	bool MoveTObject(int X, int Y, int TObject, int Rotation);
	bool FreeBlock(int X, int Y);
	bool GameOver(bool End);

	//Drawing
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);
};