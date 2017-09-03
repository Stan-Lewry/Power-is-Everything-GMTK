/**
This is the header file for the Effects class. Details of imp in Effect.cpp
This class is basically a little animation with a set number of frames that can be spawned at a given position then destroyed 
when it reaches max frame.
*/

//define guards
#ifndef EFFECT_HEADER
#define EFFECT_HEADER

//SDL so that it can store its texture
#include <SDL.h>


class Effect{
private:
	//The texture
	SDL_Texture* texture;
	
	// update timer (in this case it dosent need a seperate timer for anim because all it does is animate)
	float timer;

	// current frame and max frame
	int frame;
	int maxFrame;
	
	//mark for deletion in Game
	bool toDestroy;
	
	//position values
	//Currently effects can only be stationary so no need for velocity values
	int x;
	int y;

public:

	//constructor and destructor, takes the texture, max frame, and the coords you want it to spawn at
	Effect(SDL_Texture* texture, int maxFrame, int x, int y);
	~Effect();

	//update function, takes the dtime
	void update(float dTime);

	// getters and setters, standard stuff
	int getFrame();
	SDL_Texture* getTexture();
	bool ToDestroy();

	int getX();
	int getY();

};

#endif