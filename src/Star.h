/**
Header file for the Star class. These are the little guys in the background.
These are essentially a particle effect. I think there must be a better way to do this than to have an object for each
*/

#ifndef STAR_HEADER
#define STAR_HEADER

#include <SDL.h>
#include <stdlib.h> // why is this getting imported? RNG?



class Star{
private:

	//The texture (its literally 1 pixel of color)
	SDL_Texture* texture;

	// width and heigh values for rendering. Parallax scrolling so stars closer are bigger
	int width;
	int height;

	//control how fast the stars move
	float starSpeedMultiplier;

	//update timer
	float timer;

	//x and y positions
	int x;
	int y;

	//these where for the twinkling animation, not acutally used anymore
	int frame;
	int maxFrame;

public:

	//Consturctor and destrector. Takes a texturer, a size (how close/far away it is) and x and y vals
	Star(int size, SDL_Texture* texture, int x, int y);
	~Star();

	//update, takes dTime
	void update(float dTime);

	//getters and setters
	SDL_Texture* getTexture();

	int getWidth();
	int getHeight();
	float getStarSpeedMultiplier();

	int getX();
	int getY();

	int getFrame();
};

#endif	// STAR_HEADER