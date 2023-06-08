#pragma once

// extern "C" {
// // #include"./SDL2-2.0.10/include/SDL.h"
// // #include"./SDL2-2.0.10/include/SDL_main.h"
// }
#include <SDL2/SDL.h>
// #include <SDL/SDL_main.h>

#include <time.h>
#include "common.h"
#include <string.h>

class Game {
public:
	Game();
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface** backgrounds, *deathScreen, *victoryScreen, *healthPack;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps;


	void DrawString(int x, int y, const char* text);
	void DrawSurface(SDL_Surface* sprite, int x, int y);
	void DrawLine(int x, int y, int l, int dx, int dy, Uint32 color);
	void DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
	SDL_Surface* LoadGraphic(char path[]);
	bool Update();
	char GetInput();
	void DrawInterface();
	void PrintTitleScreen();
	void ExitGame();
};