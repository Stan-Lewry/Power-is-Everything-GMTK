// implementation for the projectile class
#include "Projectile.h"

// constructor - takes spawn coords, texturesm velocities, width and height
Projectile::Projectile(int _spawnX, int _spawnY, SDL_Texture* _texture, int initialXVelocity, int initialYVelocity, int _width, int _height){
	width = _width;
	height = _height;

	texture = _texture;
	

	x = _spawnX;
	y = _spawnY;

	timer = 0;
	
	// projectiles currently travel at a constant velocity
	velX = initialXVelocity;
	velY = initialYVelocity;

	toDestroy = false;

	frame = 0;
	animTimer = 0;

}

// destructor
Projectile::~Projectile(){
	std::cout << "Deleting a projectile" << std::endl;
}


// Update function for the projectile, takes a D-Time value
// This updates its position by the velocity and updates the animation frame
void Projectile::update(float dTime){
	timer += dTime;
	animTimer += dTime;

	if (animTimer > 1000 / 10){
		animTimer = 0;

		frame += 1;

		if (frame > 3) frame = 0;
	}

	if (timer > 1000 / 60){
		timer = 0;

		x += velX;
		y += velY;

		if (x > 1280 || x < -width){
			toDestroy = true;
		}
	}

}

// getters and setters beyond here
int Projectile::getX(){
	return x;
}

int Projectile::getY(){
	return y;
}

int Projectile::getWidth(){
	return width;
}

int Projectile::getHeight(){
	return height;
}

int Projectile::getFrame(){
	return frame;
}

SDL_Texture* Projectile::getTexture(){
	return texture;
}

bool Projectile::ToDestroy(){
	return toDestroy;
}

