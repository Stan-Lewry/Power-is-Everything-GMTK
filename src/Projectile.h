/**
This is the header file for the projectile class. very simmilar to Effect.h but they move
*/

#ifndef PROJECTILE_HEADER
#define PROJECTILE_HEADER

//SDL for the texture
#include <SDL.h>
#include <iostream>

class Projectile{
private:
	
	//Texture
	SDL_Texture* texture;

	//Width and heigh vals for rendering
	int width;
	int height;

	//Position and velocity values, these ones actually move. Need both X and Y for the bosses diagonal projectiles
	int x;
	int y;
	int velX;
	int velY;

	//update timer and animation timer
	float timer;
	float animTimer;

	//current frame (unlike Effects projectiles dont automatical die when they finish the animation)
	int frame;

	//Mark for deletion
	bool toDestroy;

public:

	//Consturctor and destructor.
	//Takes spawn positions, a texture, initial velocity values, and width and height values
	Projectile(int spawnX, int spawnY, SDL_Texture* texture, int initialXVelocity, int initialYVelocity, int width, int height);
	~Projectile();

	//update. Projectiles are assigned a velocity when they spawn then travel at a constant velocity
	void update(float dTime);

	//getters and setters, nothing very interesting at all
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getFrame();

	bool ToDestroy();

	SDL_Texture* getTexture();
};

#endif	// PROJECTILE_HEADER