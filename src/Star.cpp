// This is the implemntation for the star class. I dont think this is actually used in the fincal version

#include "Star.h"

//constructor, takes x & y vals and a texture.
Star::Star(int size, SDL_Texture* _texture, int _y, int _x){
	
	texture = _texture;
	
	width = size;
	height = size;

	x = _x;
	y = _y;

	starSpeedMultiplier = 1;

	frame = 0;
	maxFrame = 4;
}

//update function, takes a D-Time value. This just moves the star along by a set ammount.
//If the star goes off the screen it is put back at the start of the screen
void Star::update(float dTime){
	timer += dTime;
	
	x -= width * starSpeedMultiplier;

	if (x < -width){
		x = 1280 + (rand() % (1280 / 2));
	}

	
}

// getters and setters

int Star::getWidth(){
	return width;
}

int Star::getHeight(){
	return height;
}

int Star::getX(){
	return x;
}

int Star::getY(){
	return y;
}

int Star::getFrame(){
	return frame;
}

float Star::getStarSpeedMultiplier(){
	return starSpeedMultiplier;
}

SDL_Texture* Star::getTexture(){
	return texture;
}

