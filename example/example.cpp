#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL.h" // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_mixer.h" // Using SDL2_mixer
// Screen dimension constants


const int WIDTH = 1600;
const int HEIGHT = 1000;
// Numbers of Fischinger components
const int WNUM = 16;
const int HNUM = 11;
int mouseState;

struct Fischinger
{
	bool activate; // true: selected by user, false: deselected by user
	bool runActivate; // true: playing tone, false: not playing tone
	int w; // width
	int h; // height
	char tonePath[100] = "../audio/"; // path of tone file
	Mix_Chunk *tone = NULL; // tone
	bool effectActivate; // true: playing effect
	int r; // radius of effect
	int alpha; // alpha channel
	int cr, cg, cb; // colors of r, g ,b
	int paused;
};

int initSDL(); // Starts up SDL and creates window
void closeSDL(); // Frees media and shuts down SDL
void drawFischinger(SDL_Renderer *renderer, Fischinger fcg[HNUM][WNUM], int c);

Uint32 runPlay(Uint32 interval, void *param);
Uint32 runEffect(Uint32 interval, void *param);

void mouseHandleEvent(SDL_Event *e, Fischinger fcg[HNUM][WNUM], int *x, int *y);

SDL_Window *window = NULL; // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The window renderer
Fischinger fcg[HNUM][WNUM];


int initSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
	{
		// Error Handling
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}
	// Create window
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("OOP SDL Tutorial", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}
	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return 5;
	}
	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 6;
	}
	return 0;
}

void closeSDL()
{
	// Free the sound effects
	for (int i = 0; i < HNUM; i++)
	{
		for (int j = 0; j < WNUM; j++)
		{
			//Mix_FreeMusic(fcg[i][j].tone);
			Mix_FreeChunk(fcg[i][j].tone);
		}
	}
	// Destroy renderer
	// Destroy window
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	// Shutdown subsystem
	Mix_Quit();
	SDL_Quit();
}

// Draw visual music on renderer according to parameters of Fischinger components
// 1. Draw basic components or activate components or runActivate components according to activate / runActivate status of Fischinger components
// 2. Draw visual effects according to effectActivate status of Fischinger components

void drawFischinger(SDL_Renderer *renderer, Fischinger fcg[HNUM][WNUM], int run)
{
	for (int i = 0; i < HNUM; i++)
	{
		for (int j = 0; j < WNUM; j++)
		{
			if (j == run) {
				filledCircleColor(renderer, fcg[i][run].w + 50, fcg[i][run].h + 50, 6,0xFF0000FF);
			}
			else {
				aacircleColor(renderer, fcg[i][j].w + 50, fcg[i][j].h + 50, 6, 0xFFFFFFFF);

				if (fcg[i][j].effectActivate == 1)
						aacircleRGBA(renderer, fcg[i][j].w + 50, fcg[i][j].h + 50, fcg[i][j].r, fcg[i][j].cr, fcg[i][j].cg, fcg[i][j].cb, fcg[i][j].alpha);

				if (fcg[i][j].activate == 1)
					filledCircleColor(renderer, fcg[i][j].w + 50, fcg[i][j].h + 50, 5, 0xC5D4B5FF);
			}

		}
	}

}

// Timer callback function
// 1. Update runActivate status of Fischinger components
// 2. Play tone and update effectActivate status according to active status of Fischinger components

Uint32 runPlay(Uint32 interval, void *param)
{
	int *run = (int *)param;

	for (int i = 0; i < HNUM; i++)
	{
		for (int j = 0; j < WNUM; j++)
		{
			if (j == run[0])
				fcg[i][j].runActivate = 1;
			else
				fcg[i][j].runActivate = 0;

			if (fcg[i][j].activate == 1 && fcg[i][j].runActivate == 1)
			{
				fcg[i][j].effectActivate = 1;
				fcg[i][j].paused = 0;
			}
				
		}
	}
	run[0] = (run[0] + 1) % 16;
	return interval;
}

// Timer callback function
// 1. Update visual effects parameters according to effectActivate status of Fischinger components
// 2. Reset visual effects parameters of Fischinger components if the stop requirement of effect meets

Uint32 runEffect(Uint32 interval, void *param)
{
	//int *run = (int *)param;

	for (int i = 0; i < HNUM; i++) 
	{
		for (int j = 0; j < WNUM; j++)
		{
			if (fcg[i][j].effectActivate)
			{
				if (fcg[i][j].alpha < 50) 
				{
					Mix_HaltChannel(-1);
					fcg[i][j].effectActivate = 0;
					fcg[i][j].r = 0;
					fcg[i][j].alpha = 255;
				}
				else
				{
					fcg[i][j].r += 10;
					fcg[i][j].alpha -= 10;
				}
				
				if (fcg[i][j].paused == 0)
				{
					Mix_PlayChannel(-1, fcg[i][j].tone, 0);
					fcg[i][j].paused = 1;
				}
			}
		}
	}

	return interval;
}

// Mouse event handler
// Update activate status of Fischinger components when click on corresponding area of screen

void mouseHandleEvent(SDL_Event *e, Fischinger fcg[HNUM][WNUM], int *x, int *y)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		static int lasttime = SDL_GetTicks();
		static int curtime = SDL_GetTicks();
		int timediv;

		lasttime = curtime;
		curtime = SDL_GetTicks();
		timediv = curtime - lasttime;

		switch (e->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			break;

		case SDL_MOUSEBUTTONUP:
			if (e->button.button == SDL_BUTTON_LEFT) 
			{
				SDL_GetMouseState(x, y);
				mouseState = 1;
				(*x) /= 100;
				(*y) /= 100;
			}
			break;
		}
	}
}

// When using SDL, you have to use "int main(int argc, char* args[])"
// int main() and void main() are not allowed

int main(int argc, char* args[])
{
	int mousepos[2];

	// Start up SDL and create window
	if (initSDL())
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}

	for (int i = 0; i < HNUM; i++) 
	{
		for (int j = 0; j < WNUM; j++) 
		{
			char str[1000];
			sprintf_s(str, 1000, "%sp%02d.wav", fcg[i][j].tonePath, i + 1);
			fcg[i][j].tone = Mix_LoadWAV(str);
			if (fcg[i][j].tone == NULL)
			{
				printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
				return false;
			}
		}
	}
	// Initialize all Fischinger components

	for (int i = 0; i < HNUM; i++) {
		for (int j = 0; j < WNUM; j++) {

			fcg[i][j].activate = 0;
			fcg[i][j].runActivate = 0;
			fcg[i][j].w = 100 * j;
			fcg[i][j].h = 100 * i;
			fcg[i][j].effectActivate = 0;
			fcg[i][j].r = 5;
			fcg[i][j].alpha = 255;
			fcg[i][j].cr = rand() % 256;
			fcg[i][j].cg = rand() % 256;
			fcg[i][j].cb = rand() % 256;
			fcg[i][j].paused = 1;
		}
	}

	// Settings of timer callback function 
	int run[1] = { 0 };

	SDL_TimerID timerID_runPlay = SDL_AddTimer(500, runPlay, run);
	SDL_TimerID timerID_runEffect = SDL_AddTimer(100, runEffect, run);


	bool quit = false; // Main loop flag
	SDL_Event e; // Event handler

	// While application is running
	while (!quit)
	{
		mouseState = 0;
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}


			mouseHandleEvent(&e, fcg, &mousepos[0], &mousepos[1]);

			// call mouse event handler
			switch (mouseState)
			{
			case 0:
				break;
			case 1:
				fcg[mousepos[1]][mousepos[0]].activate = !fcg[mousepos[1]][mousepos[0]].activate;
				break;
			}
		}
		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		// Draw visual music on renderer according to parameters of Fischinger components
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		drawFischinger(renderer, fcg, run[0]);

		// Update screen
		SDL_RenderPresent(renderer);

	}
	//Free resources and close SDL
	SDL_RemoveTimer(timerID_runPlay);
	SDL_RemoveTimer(timerID_runEffect);

	closeSDL();
	return 0;
}