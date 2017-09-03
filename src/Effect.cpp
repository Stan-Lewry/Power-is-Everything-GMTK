/**
This is the implementation of the Effect class. for more info on the class please visit Effect.h
*/

#include "Effect.h"

//Constructor. Sets everything.
Effect::Effect(SDL_Texture* _texture, int _maxFrame, int _x, int _y){
	x = _x;
	y = _y;
	
	texture = _texture;
	maxFrame = _maxFrame;

	//frame and timers initialised as zero
	frame = 0;
	timer = 0;
	toDestroy = false;

}


//update function
void Effect::update(float dTime){

	//set the animation speed. I have no idea why this is 11
	timer += dTime;
	if (timer > 1000 / 11){
		timer = 0;
		if (frame > maxFrame){
			toDestroy = true;
		}
		else{
			frame++;
		}
	}
}


//getters and setters
int Effect::getFrame(){
	return frame;
}

SDL_Texture* Effect::getTexture(){
	return texture;
}

bool Effect::ToDestroy(){
	return toDestroy;
}

int Effect::getX(){
	return x;
}

int Effect::getY(){
	return y;
}

