/**
This is the header file for the player class. This should probably be subclassed from a GameObject class.
*/

#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <iostream>
#include <SDL.h>

class Player{
private:

	//Texture for the ship and one for the trail
	SDL_Texture* texture;
	SDL_Texture* trailTexture;

	//x y for pos, width height for rendering
	int x;
	int y;
	int width;
	int height;

	//velocity values
	float velX;
	float velY;

	//power, this is everything
	int power;

	//timer for update and one for animation
	float timer;
	float animTimer;

	//current animation frame and one for the trail
	int frame;
	int trailFrame;
	
	//unused
	SDL_Rect hitBox;

public:

	//costructor and destructor. Takes texutres for the trail and the ship
	Player(SDL_Texture* texture, SDL_Texture* trailTexture);
	~Player();

	//update function, takes dTime and vel values (these get passed in from Game as they are produced by inputs)
	void update(float dTime, float velX, float velY);

	//getters and setters
	int getX();
	int getY();

	int getVelX();
	int getVelY();

	int getWidth();
	int getHeight();

	int getPower();
	void setPower(int power);

	int getFrame();
	int getTrailFrame();



	SDL_Texture* getTexture();
	SDL_Texture* getTrailTexture();

	SDL_Rect getHitBox();
};

#endif	// PLAYER_HEADER