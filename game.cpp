#include "game.h"
#include <iostream>

SDL_Surface* Game::LoadGraphic(char path[])
{
	SDL_Surface* graphic;
	graphic = SDL_LoadBMP(path);
	if (graphic == NULL) {
		printf("Image load error: %s\n", SDL_GetError());
		ExitGame();
	}
	return graphic;
}

// narysowanie napisu txt na powierzchni screen, zaczynaj�c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj�ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void Game::DrawString(int x, int y, const char* text) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt �rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void Game::DrawSurface(SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

// rysowanie linii o d�ugo�ci l w pionie (gdy dx = 0, dy = 1) 
// b�d� poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void Game::DrawLine(int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

// rysowanie prostok�ta o d�ugo�ci bok�w l i k
// draw a rectangle of size l by k
void Game::DrawRectangle(int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) 
{
	int i;
	DrawLine(x, y, k, 0, 1, outlineColor);
	DrawLine(x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(x, y, l, 1, 0, outlineColor);
	DrawLine(x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(x + 1, i, l - 2, 1, 0, fillColor);
};


Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		ExitGame();
	}
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		ExitGame();
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "TRON");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	charset = LoadGraphic("./sprites/cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);
	

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	srand(time(NULL));
}

bool Game::Update()
{
	while (!quit) {
		t2 = SDL_GetTicks();

		// w tym momencie t2-t1 to czas w milisekundach,
		// jaki uplyna� od ostatniego narysowania ekranu
		// delta to ten sam czas w sekundach
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		if (GetInput() == 'n')
		{
			ExitGame();
			return true;
		}
		

		SDL_FillRect(screen, NULL, CZARNY);
		


		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		DrawInterface();		
		frames++;
	};

		ExitGame();
		return false;
}

void Game::DrawInterface()
{
	char text[128];
	// tekst informacyjny / info text
	DrawRectangle(4, 4, SCREEN_WIDTH/4, 20, ZIELONY, CZARNY);
	DrawRectangle(SCREEN_WIDTH/2, 4, SCREEN_WIDTH/2 - 10, 20, ZIELONY, CZARNY);
	DrawRectangle(5, screen->h - 24, SCREEN_WIDTH / 2 - 40, 20, ZIELONY, CZARNY);
	//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
	sprintf(text, "Bullet Hell Game");
	DrawString(10, 10, text);
	sprintf(text, "czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
	DrawString(screen->w / 2 + 5, 10, text);
	//	      "Esc - exit, \030 - faster, \031 - slower"
	sprintf(text, "Esc - wyjscie, \030 \031 \032 \033 - movement, N - new game, W A S D - shooting");
	DrawString(10, screen->h - 20, text);
	DrawString(50, 50, text);
	DrawString(50, 90, text);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

char Game::GetInput()
{
	// obs�uga zdarze� (o ile jakie� zasz�y) / handling of events (if there were any)
	while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				if (event.key.keysym.sym == SDLK_n) return 'n';
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
	};
}

void Game::ExitGame()
{
	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Game::PrintTitleScreen()
{
	SDL_FillRect(screen, NULL, CZARNY);

	DrawRectangle(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ZIELONY, CZARNY);
	char text[120];
	sprintf(text, "Bullet Hell - The Game");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 10, text);
	sprintf(text, "By Jan Barczewski - 188679");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 30, text);
	sprintf(text, "Select level: ");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 90, text);
	sprintf(text, "Level 1");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 110, text);
	sprintf(text, "Level 2");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 130, text);
	sprintf(text, "Level 3");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 150, text);
	sprintf(text, "Esc - Wyjscie z gry");
	DrawString(SCREEN_WIDTH / 2 - strlen(text) * 4, SCREEN_HEIGHT / 4 + 190, text);

	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}
