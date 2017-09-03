/*Implementation for the main class. This is program entry point. initilaizes some SDL stuff and instantiates the Game class
 *
 *
 */

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Game.h"

#define screenWidth 1280
#define screenHeight 720

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Game* game = NULL;

bool globalRunning;

// initialize all SDL stuff. Standard function with some basic error catching
bool initSDL(){
	SDL_Init(SDL_INIT_EVERYTHING);

	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

	window = SDL_CreateWindow("When you X; Y happens", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window != NULL){
		printf("window initialized\n");
		renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
		if (renderer != NULL){
			printf("renderer initialized\n");
			return true;
		}
		return true;
	}
	else{
		return false;
	}
}

// main function, instantiates game, starts game loop
int main(int argv, char* argc[]){
	
	globalRunning = initSDL();

	game = new Game(renderer, window);

	while (globalRunning){
		game->update();
		globalRunning = game->isRunning();
	}
	
	return 0;
}

