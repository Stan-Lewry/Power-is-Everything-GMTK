/**
This is the implementation for the AmmoPack class. details of class in AmmoPack.h.
*/

#include "AmmoPack.h"

//Constructor, sets everything that gets passed.
AmmoPack::AmmoPack(SDL_Texture* _texture, int _x, int _y, int _ammo){
	texture = _texture;
	x = _x;
	y = _y;
	ammo = _ammo;
	movementSpeed = 3;		//movement speed set manually 
	timer = 0;				//Timers initialised at 0
	animTimer = 0;

	
	width = 68;				//width and heigh vals set manually, just double whatever size the sprite is. probably not best practice
	height = 80;

	frame = 0;				//current frame initialised to zero. max frame manually set. again not best practice
	maxFrame = 3;
}

//Destructor. dosent do anything - test implementation
AmmoPack::~AmmoPack(){
	std::cout << "Destroy ammo pack" << std::endl;
}

//update function. Hovers towards the player
void AmmoPack::update(float dTime, int playerX, int playerY){
	timer += dTime;
	animTimer += dTime;

	//Set the animation speed. 
	if (animTimer > 1000 / 10){
		animTimer = 0;
		
		//itterate the frame, if its over max frame reset it
		frame++;
		if (frame > maxFrame){
			frame = 0;
		}

	}

	//set the update speed
	if (timer > 1000 / 60){
		timer = 0;
		
		//zero the x and y velocities
		velX = 0;
		velY = 0;

		//ammo pack is always movie to the left at its movement speed
		velX -= movementSpeed;


		if (x > playerX){		// only drift toward the player if it is in front of the player

			//we add 10 to the checks to stop it from jittering about when in line with the player
			if (y > playerY + 10){			//if below the player move up by movement speed
				velY -= movementSpeed;
			}
			else if(y < playerY - 10){		//if above the player move down by movement speed
				velY += movementSpeed;
			}
		}

		// apply new velocieties 
		x += velX;
		y += velY;
	}
}


//Standard getter setter stuff
int AmmoPack::getX(){
	return x;
}

int AmmoPack::getY(){
	return y;
}

int AmmoPack::getWidth(){
	return width;
}

int AmmoPack::getHeight(){
	return height;
}

int AmmoPack::getVelX(){
	return velX;
}

int AmmoPack::getVelY(){
	return velY;
}

int AmmoPack::getAmmo(){
	return ammo;
}

int AmmoPack::getFrame(){
	return frame;
}

bool AmmoPack::ToDestroy(){
	return toDestroy;
}

void AmmoPack::setToDestroy(bool _toDestroy){
	toDestroy = _toDestroy;
}

float AmmoPack::getTimer(){
	return timer;
}

SDL_Texture* AmmoPack::getTexture(){
	return texture;
}