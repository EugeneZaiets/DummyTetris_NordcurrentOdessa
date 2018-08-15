#define _CRT_NONSTDC_NO_WARNINGS
#pragma warning(disable:4996)
#include "Game.h"


Game::Game(TetrisObject* object, int xSize, int ySize) : BaseApp(object, xSize, ySize){
	srand((unsigned int) time(NULL));

	nScore = 0; nDeletedLines = 0;
	nSpeed = 10; nSpeedCount = 0;
	PushDown = isDeleted = isOver = false;

	texts.resize(5);
	texts[0] = "Next Object:";
	texts[1] = "GAME OVER";
	texts[2] = "Press ESC to Exit";
	texts[3] = "Lines: ";
	texts[4] = "Score: ";

	nObject = ((rand() % 7) + 5) % 6;
	nRotation = rand() % 3;

	nPosX = ((X_SIZE ) / 2) + TObject->GetInitPosX(nObject, nRotation);
	nPosY = TObject->GetInitPosY(nObject, nRotation);

	nNextObject = rand() % 6;
	nNextRotation = rand() % 3;
	nNextX = (X_SIZE) / 2;
	nNextY = 0;
}

void Game::KeyPressed(int btnCode)
{
	nPosX -= (btnCode == 75 && MoveTObject(nPosX - 1, nPosY, nObject, nRotation)) ? 1 : 0;
	nPosX += (btnCode == 77 && MoveTObject(nPosX + 1, nPosY, nObject, nRotation)) ? 1 : 0;
/*	nPosY -= (btnCode == 72 && MoveTObject(nPosX, nPosY - 1, nObject, nRotation)) ? 1 : 0;*/ // UP, don't use
	nPosY += (btnCode == 80 && MoveTObject(nPosX, nPosY + 1, nObject, nRotation)) ? 1 : 0;

	if (btnCode == 32) {
		if (MoveTObject(nPosX, nPosY, nObject, (nRotation + 1) % 4)){
			nRotation = (nRotation + 1) % 4;
		}
	} // Space to rotate

	if (btnCode == 13){
		while (MoveTObject(nPosX, nPosY + 1, nObject, nRotation)) { ++nPosY; }
		isOver = GameOver(StoreTObject(nPosX, nPosY, nObject, nRotation));
			isDeleted = NeedToDelete();
			CreateNewObject();
	}//Push Down by ENTER

	if (btnCode == 27) exit(GAMEEXIT);//Esc to Exit
}

void Game::UpdateF(float deltaTime){
	ClearScreen();
	ShowNextObject();
	if (isOver) {
		Over();
		return;
	}
	for (int i = 0; i < X_SIZE ; i++)
	{
		for (int j = 0; j < Y_SIZE ; j++)
		{
			if (Field[i][j])
				SetChar(i, j, L'O'/*0x2588*/);
		}
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			if (TObject->GetBlock(nObject, nRotation, j, i) != 0) 
				SetChar(nPosX + i, nPosY + j, L'O'/*0x2588*/);
		}
	}

	if (PushDown){
		if (MoveTObject(nPosX, nPosY + 1, nObject, nRotation)) ++nPosY;
		else {
			isOver = GameOver(StoreTObject(nPosX, nPosY, nObject, nRotation));
			isDeleted = NeedToDelete();
			CreateNewObject();
		}
		PushDown = false;
	}
	/*
	if (MoveTObject(nPosX, nPosY + 1, nObject, nRotation))++nPosY;
	else {
	StoreTObject(nPosX, nPosY, nObject, nRotation);
	if (NeedToDelete()){
	isDeleted = true;
	CreateNewObject();
	}
	else isOver = GameOver(StoreTObject(nPosX, nPosY, nObject, nRotation));
	}*/
	if (isDeleted){
		isDeleted = false;
		++nDeletedLines;
		nScore += 25;
		if (nSpeed >= 0) --nSpeed;
	}
	 
	++nSpeedCount;
	if (nSpeedCount == nSpeed){
		nSpeedCount = 0;
		PushDown = true;
	}
}

void Game::ClearScreen(){
	for (int x = 0, i = 0, j = 0, k = 0, lines = 0, score = 0; x < X_SIZE + textspace; x++)
	{
		for (int y = 0; y < Y_SIZE; y++)
		{
			SetChar(x, y, L' ');
			if (x == X_SIZE) SetChar(x, y, L'|');
			if (y == 6 && x >= X_SIZE + 1) SetChar(x, y, L'_');
			if (y == 8 && x >= X_SIZE + 1) SetChar(x, y, L'_');
			i += (DrawTexts(X_SIZE + 1, 0, x, y, i, texts[0])) ? 1 : 0;
			j += (DrawTexts(X_SIZE + 1, 7, x, y, j, texts[3])) ? 1 : 0;
			k += (DrawTexts(X_SIZE + 1, 9, x, y, k, texts[4])) ? 1 : 0;
			lines += (DrawTexts(X_SIZE + 1 + texts[3].size(), 7, x, y, lines, itoa(nDeletedLines, linesbuff, 10))) ? 1 : 0;
			score += (DrawTexts(X_SIZE + 1 + texts[4].size(), 9, x, y, score, itoa(nScore, scorebuff, 10))) ? 1 : 0;
		}
	}
}
void Game::ShowNextObject() {
	for (int i = 0; i < BLOCK_SIZE; i++) 
		for (int j = 0; j < BLOCK_SIZE; j++)
			if (TObject->GetBlock(nNextObject, nNextRotation, j, i) != 0)
				SetChar(25 + i, 1 + j, L'O'/*0x2588*/);
}
void Game::CreateNewObject(){
	nObject = nNextObject;
	nRotation = nNextRotation;
	nPosX = (X_SIZE / 2) + TObject->GetInitPosX(nObject, nRotation);
	nPosY = TObject->GetInitPosY(nObject, nRotation);
	nNextObject = rand() % 6;
	nNextRotation = rand() % 3;
}
void Game::Over(){
	for (int x = 0, i = 0, j = 0; x < X_SIZE + textspace; ++x)
	{
		for (int y = 0; y < Y_SIZE; ++y)
		{
			SetChar(x, y, L' ');
			if (x >= (X_SIZE + 15) / 2 - 5 && y == Y_SIZE / 2 - 1){
				while (i <  texts[1].size()) {
					SetChar(x, y, texts[1][i++]);
					break;
				}
			}
			if (x >= (X_SIZE + 15) / 2 - 8 && y == Y_SIZE / 2 + 1){
				while (j <  texts[2].size()) {
					SetChar(x, y, texts[2][j++]);
					break;
				}
			}
		}
	}
}
bool Game::DrawTexts(int first, int second, int x, int y, int increment, std::string str){
	if (x >= first && y == second) {
		while (increment < str.size()) {
			SetChar(x, y, str[increment]);
			return true;
		}
	}
	else return false;
}
