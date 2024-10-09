#include "GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager = new GameManager();
	gameManager->Run();
	delete gameManager;
	return 0;
}