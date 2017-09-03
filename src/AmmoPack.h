/**
This is the header file for the AmmoPack class - the little ammo guys that float across the screen.
Details of implementation can be found in AmmoPack.cpp
This class contains alot of the same variables as other classes, all the stuff required for movement, etc.
Nothing particularly strange happening in here as far as I remember.
*/



//define guards
#ifndef AMMOPACK_HEADER
#define AMMOPACK_HEADER

//class needs SDL as it stores its own texture (no texture bank class)
#include <SDL.h>
#include <iostream> // dont know why it needs iostream, probably just for couts

class AmmoPack{
private:

	// x and y position values
	int x;
	int y;
	//width and height vals for rendering
	int width;
	int height;
	//velocity vals (should probably be done as a Vect2 but whatever)
	int velX;
	int velY;
	//movement speed val, velX & y get multiplied by this value before they are applied to the positions
	int movementSpeed;

	//update timer and animation timer
	float timer;
	float animTimer;

	//current anim frame and the max anim frame
	int frame;
	int maxFrame;

	//the Texture
	SDL_Texture* texture;

	//The ammo val that goes to the player when it gets collected
	int ammo;

	//mark it so Game knows when to delete it
	bool toDestroy;
public:
	// Constructor and destructor. takes a texture, starting coords, and the ammo val
	AmmoPack(SDL_Texture* texture, int x, int y, int ammo);
	~AmmoPack();

	//update function, takes the dTime and the players position (so that it can move towards the player)
	void update(float dTime, int playerX, int playerY);

	//standard getters and setters
	int getWidth();
	int getHeight();
	int getX();
	int getY();
	int getVelX();
	int getVelY();
	int getAmmo();
	int getFrame();

	bool ToDestroy();
	void setToDestroy(bool toDestroy);

	float getTimer();

	SDL_Texture* getTexture();
};

#endif	//AMMOPACK_HEADER