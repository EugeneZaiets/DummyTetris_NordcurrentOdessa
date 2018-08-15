// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include <algorithm>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include <strsafe.h>

#define BLOCK_SIZE 5
#define MY_PERFORMENCE_COUNTER
#define _UNICODЕ
#define UNICODЕ

#include "PerformanceCounter.h"
#include "BaseApp.h"

BaseApp::BaseApp(TetrisObject* tobject, int xSize, int ySize) : TObject(tobject), X_SIZE(xSize), Y_SIZE(ySize)
{
	textspace = 15;
	srand((unsigned int)time(NULL));

	COORD windowBufSize = { X_SIZE + 1 + textspace, Y_SIZE + 1 };
	SMALL_RECT windowSize = { 0, 0, X_SIZE - 1 + textspace, Y_SIZE - 1 };
	
	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	if (!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
	{
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
		system("pause");
		exit(WindowInfoFail);
	}
	if (!SetConsoleScreenBufferSize(mConsole, windowBufSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
		system("pause");
		exit(ConsoleBuffSizeFail);
	}
	
	
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(mConsole, &cursorInfo);


	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE + 1 + textspace)*(Y_SIZE + 1)*sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 16;
	mDwBufferSize.Y = Y_SIZE + 1;		// размер буфера данных

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// координаты ячейки

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1 + textspace;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// прямоугольник для чтения

	Field.resize(X_SIZE, std::vector<int>(Y_SIZE));

	for (int i = 0; i < X_SIZE; ++i)
	{
		for (int j = 0; j < Y_SIZE; ++j)
		{
			SetChar(j, i, L' ');
			Field[i][j] = 0;
		}
	} 
}
BaseApp::~BaseApp()
{
	free(mChiBuffer);
	exit(8);
}
void BaseApp::SetChar(int x, int y, wchar_t c = 0x2B88)
{
	mChiBuffer[x + (X_SIZE + 1 + textspace)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE + 1 + textspace)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
}
wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE + 1 + textspace)*y].Char.AsciiChar;
}
bool BaseApp::StoreTObject(int X, int Y, int Object, int Rotation){
	if (Y < 0){
		return true;
	}
	for (int i = X, k = 0; i < X + BLOCK_SIZE; ++i, ++k)
	{
		for (int j = Y, l = 0; j < Y + BLOCK_SIZE; ++j, ++l)
		{
			if (TObject->GetBlock(Object, Rotation, l, k) != 0) Field[i][j] = 1;
		}
	}
	return false;
}
void BaseApp::DeleteLine(int Y){
	for (int j = Y ; j > 0; --j)
	{
		for (int i = 0; i < X_SIZE; ++i)
		{
			Field[i][j] = Field[i][j - 1];
		}
	}
}
bool BaseApp::NeedToDelete(){
	bool isDeleted = false;
	for (int j = 0; j < Y_SIZE; ++j)
	{
		int i = 0;
		while (i < X_SIZE){
			if (!Field[i][j]) break;
			++i;
		}
		if (i == X_SIZE){
			isDeleted = true;
			DeleteLine(j);
		}
	}
	return isDeleted;
}
bool BaseApp::GameOver(bool End){
	if(End) return true;
	for (int i = 0; i < X_SIZE; ++i) if (Field[i][0]) return true;
	return false;
}
bool BaseApp::FreeBlock(int X, int Y){
	return (Field[X][Y] == 0) ? true : false;
}
bool BaseApp::MoveTObject(int X, int Y, int Object, int Rotation){
	for (int i1 = X, i2 = 0; i1 < X + BLOCK_SIZE; ++i1, ++i2)
	{
		for (int j1 = Y, j2 = 0; j1 < Y + BLOCK_SIZE; ++j1, ++j2)
		{
			if (i1 < 0 || i1 > X_SIZE - 1|| j1 > Y_SIZE - 1){
				if (TObject->GetBlock(Object, Rotation, j2, i2) != 0) 
					return false;
			}
			if (j1 >= 0){
				if (TObject->GetBlock(Object, Rotation, j2, i2) != 0 && !FreeBlock(i1, j1)) 
					return false;
			}
		}
	}
	return true;
}
void BaseApp::Render()
{
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion))
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
	}
}
void BaseApp::Run()
{
	CStopwatch timer;
	int sum = 0;
	int counter = 0;
	int deltaTime = 0;

	while (1)
	{
		timer.Start();
		if (_kbhit())
		{

			KeyPressed(_getch());
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}

		UpdateF((float)deltaTime / 1000.0f);
		Render();

		Sleep(50);

		while (1)
		{
			if (_kbhit())
			{

				KeyPressed(_getch());
				if (!FlushConsoleInputBuffer(mConsoleIn))
					cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
			}
			deltaTime = timer.Now();
			if (deltaTime > 20)
				break;
		}
		sum += deltaTime;
		counter++;
		if (sum >= 1000)
		{
			TCHAR  szbuff[255];
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), counter);
			SetConsoleTitle(szbuff);
			counter = 0;
			sum = 0;
		}
	}
}