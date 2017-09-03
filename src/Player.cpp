/*
 * This is the implementagtion to the player class. Better practice would be for this to be subclassed from a GameObject class or something
 */ 

#include "Player.h"

// Constructor, takes textures
Player::Player(SDL_Texture* _texture, SDL_Texture* _trailTexture){

	//why does it spawn at these coords?
	x = 100;
	y = 200;

	width = 128;
	height = 128;

	velX = 0;
	velY = 0;

	power = 10;

	texture = _texture;
	trailTexture = _trailTexture;

	timer = 0;
	animTimer = 0;

	frame = 0;
	trailFrame = 0;
}

// Update function, takes a D-Time and some velocity values to be applied
// this function pretty much just moves the player and updates the animations
void Player::update(float dTime, float _velX, float _velY){

	timer += dTime;
	animTimer += dTime;

	if (animTimer > 1000 / 10){
		animTimer = 0;

		trailFrame += 1;
		if (trailFrame > 3) trailFrame = 0;
	}

	if (timer > 1000 / 60){
		velX = _velX;
		velY = _velY;

		if (velX > 0 && x < (1260 - width)){
			x += velX;
		}
		else if (velX < 0 && x > 20){
			x += velX;
		}

		if (velY > 0 && y < (700 - height)){
			y += velY;
			frame = 2;
		}
		else if (velY < 0 && y > 128){
			y += velY;
			frame = 1;
		}
		else if (velY == 0){
			frame = 0;
		}
		timer = 0;
	}



}


// very standard getters and setters beyond this point

int Player::getX(){
	return x;
}

int Player::getY(){
	return y;
}

int Player::getVelX(){
	return velX;
}

int Player::getVelY(){
	return velY;
}

int Player::getWidth(){
	return width;
}

int Player::getHeight(){
	return height;
}

int Player::getPower(){
	return power;
}

void Player::setPower(int _power){
	
	power = _power;

	if (power > 34) power = 34;
}

int Player::getFrame(){
	return frame;
}

int Player::getTrailFrame(){
	return trailFrame;
}

SDL_Texture* Player::getTexture(){
	return texture;
}

SDL_Texture* Player::getTrailTexture(){
	return trailTexture;
}

SDL_Rect Player::getHitBox(){
	return hitBox;
}
