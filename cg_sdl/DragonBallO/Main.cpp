#include "GameManager.h"

extern bool gQuitGame;

int main( int argc, char* args[] )
{
	GameManager gameManager;

	if(gameManager.Init()){
		while (!gQuitGame) {
			gameManager.Update();
			gameManager.Render();
		}
	}

	gameManager.Cleanup();

	return 0;
}