#include "BaseApp.h"
#include"Game.h"
void main(){	
	TetrisObject tobject;
	Game game(&tobject, 20, 20);
	game.Run();
}