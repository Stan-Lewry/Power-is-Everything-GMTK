/**
This is the implementation of the class Enemy. For more info on the class please visit Enemy.h
*/

#include "Enemy.h"

//Constructor. A lot is going on in here
Enemy::Enemy(SDL_Texture* _texture, SDL_Texture* _trailTexture, enemyType _type, int _spawnY, int _power){
	x = 1280;					//x is set to (hard coded) screen width - all enemies appear off the right of the screen
	y = _spawnY;

	type = _type;				//set the type

	width = 128;				//hardcoded width and heighy vals (double sprite sheet size
	height = 128;

	velX = 0;					// velocieties initialised to zero
	velY = 0;


	if (type == TYPE4){			// special values for type 4 (the boss)
		width = 512;
		height = 256;
	}


	power = _power;				// power = everything 

	texture = _texture;
	trailTexture = _trailTexture;

	timer = 0;
	animTimer = 0;

	frame = 0;
	trailFrame = 0;


	//set all the behaviour bools. Always start going down by default
	movingUp = false;
	movingDown = true;

	sineGoingUp = true;

	toDestroy = false;
	shooting = false;

	changeVertical = 1000;
	deceleration = 0;

	//unique movement speeds for each enemy type except the boss because it dosent move apparently
	type1MovementSpeed = 4;
	type2MovementSpeed = 3;
	type3MovementSpeed = 6;

}


//Update function for the enemies. Quite a lot going on in here
void Enemy::update(float dTime, float _velX, float _velY, int playerX, int playerY){
	//increment all the timers by dTime
	timer += dTime;
	animTimer += dTime;
	externalTimer += dTime;
	shootingTimer += dTime;

	//Animation for the trail
	if (animTimer > 1000 / 10){
		animTimer = 0;
		//do an animation
		trailFrame += 1;
		if (trailFrame > 3) trailFrame = 0;
	}

	//set the update speed
	if (timer > 1000 / 60){
		timer = 0;

		//zero the velocity values
		velX = 0;
		velY = 0;

		//x += velX;
		//y += velY;

		//if the enemy is off the left side of the screen the get destroyed
		if (x < -width){
			toDestroy = true;
		}

		/**
		Update behaviour for type 1 enemies.
		The following block does basically:
		Move slowely in from the right untill a certain point
		then move up and down while shooting every 800 ms
		do that for 5 secs then fly across the screen and off the left side
		*/
		if (type == TYPE1){

			// move slowely in from the right untill they are at 1100
			// NOTE: THESE HARDCODED VALUES ARE TURBO BAD PRACTICE SHOULD REALLY ALL BE DONE AT % OF SCREEN WIDTH
			if (x > 1100){
				velX = -4;
				velY = 0;
			}

			// if its been over a second and less then 5 secs
			else if (externalTimer > 1000 && externalTimer < 5000){
				
				// shoot if its been 800 ms since last shooting
				if (shootingTimer > 800){
					shootingTimer = 0;
					shooting = true; // shoot
				}


				//if we are moving up
				if (movingUp){
					velY = -4;	//move up
					if (y <= 128){	// if we are at the top of the screen
						movingUp = false;	//move down
						movingDown = true;
					}
				}
				else if (movingDown){ //if we are moving down
					velY = 4;	//move down
					if (y >= 600){	//if we are at the bottom of the screen
						movingDown = false;	// move up
						movingUp = true;
					}
				}
			}
			else if (externalTimer > 5000){ // if its been 5 secs
				velX = -type1MovementSpeed;	//just fly to the left
			}
		}

		/**
		This is the behaviour for type 2 enemies. They:
		move across the screen from right to left
		float up and down in a kind of sine ish wave pattern
		*/
		else if (type == TYPE2){
			
			velX = -type2MovementSpeed; // move left


			 
			if (externalTimer > changeVertical){ // if we have been moving up for the specified time
				changeVertical += changeVertical;
				sineGoingUp = !sineGoingUp;			// tbh i dont know what is going on here or why
				deceleration = 0;
			}

			if (sineGoingUp){
				velY = -(4 - deceleration);
			}
			else{
				velY = 4 - deceleration;
			}
			deceleration += 0.2;

			if (sineGoingUp && y <= 128 || !sineGoingUp && y >= 600){
				velY = 0;
			}

		}

		/**
		The following block specified the behaviour for type 3 enemies.
		These guys do the same thing as the AmmoPack. can look in AmmoPack.cpp for details
		All the time we are in front of the player we follow the players y position.
		10pixel margin to stop jittering when in line with player
		*/
		else if (type == TYPE3){
			velX = -type3MovementSpeed;

			if (x > playerX){
				if (y > playerY + 10){
					velY = -3;
				}
				else if (y < playerY - 10){
					velY = 3;
				}
			}
		}

		/**
		The following block specifies the behaviour for type 4 enemies - the boss ship
		This enemy floats in from the right and moves up and down the screen shooting
		*/
		else if (type == TYPE4){
			if (x > 758){
				velX = -5;
			}
			else{

				if (shootingTimer > 800){
					shootingTimer = 0;
					shooting = true;
				}

				velX = 0;
				if (movingUp){
					velY = -6;
					if (y <= 128){
						movingUp = false;
						movingDown = true;
					}
				}
				else if (movingDown){
					velY = 6;
					if (y >= 450){
						movingDown = false;
						movingUp = true;
					}
				}
			}



		}

		//apply velocites
		x += velX;
		y += velY;

	}

}


//standard getters and setters
int Enemy::getX(){
	return x;
}

int Enemy::getY(){
	return y;
}

int Enemy::getVelX(){
	return velX;
}

int Enemy::getVelY(){
	return velY;
}

int Enemy::getWidth(){
	return width;
}

int Enemy::getHeight(){
	return height;
}

int Enemy::getPower(){
	return power;
}

int Enemy::getFrame(){
	return frame;
}

int Enemy::getTrailFrame(){
	return trailFrame;
}

SDL_Texture* Enemy::getTexture(){
	return texture;
}

SDL_Texture* Enemy::getTrailTexture(){
	return trailTexture;
}

enemyType Enemy::getType(){
	return type;
}

float Enemy::getExternalTimer(){
	return externalTimer;
}

void Enemy::setExternalTimer(float time){
	externalTimer = time;
}

bool Enemy::MovingDown(){
	return movingDown;
}

void Enemy::setMovingDown(bool _movingDown){
	//movingUp = !_movingDown;
	movingDown = _movingDown;
}

bool Enemy::MovingUp(){
	return movingUp;
}

void Enemy::setMovingUp(bool _movingUp){
	//movingDown = !_movingUp;
	movingUp = _movingUp;
}

bool Enemy::ToDestroy(){
	return toDestroy;
}

void Enemy::setToDestroy(bool destroy){
	toDestroy = destroy;
}

bool Enemy::Shooting(){
	return shooting;
}

void Enemy::setShooting(bool _shooting){
	shooting = _shooting;
}

void Enemy::setPower(int _power){
	power = _power;
	if (power <= 0) toDestroy = true;
}