#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "game.h"
#include "common.h"

// main
#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char **argv) {
	
	
	int nextLevel = 0;
	// tryb pe�noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	while (true)
	{
		Game game;
		if (game.Update() == false)
		{
			break;
		}
	}
	
	return 0;
};
