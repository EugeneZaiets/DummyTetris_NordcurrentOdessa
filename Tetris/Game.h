#pragma once
#define GAMEEXIT 4
#define GAMEOVER 5
#define BLOCK_SIZE 5

#include <time.h>
#include <string>
#include <conio.h>
#include "BaseApp.h"
#include "TetrisObject.h"
class Game : public BaseApp {
	int nNextObject, nNextRotation;
	int nNextX, nNextY;
	int nSpeed, nSpeedCount;
	int nPosX, nPosY;
	int nObject, nRotation;
	int nScore;
	int nDeletedLines;
	bool PushDown;
	bool isDeleted;
	bool isOver;
	char linesbuff[1024];
	char scorebuff[1024];
	std::vector<string> texts;
public:
	Game(TetrisObject* T, int xSize = 20, int ySize = 20);
	virtual void KeyPressed(int btnCode);
	virtual void UpdateF(float deltaTime);
	void Over();
	void ClearScreen();
	void ShowNextObject();
	void CreateNewObject();
	bool Game::DrawTexts(int first, int second, int x, int y, int increment, std::string str);
};