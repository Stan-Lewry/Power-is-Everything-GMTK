/**
This the header file for the enemey class. These are the enemies in the game (wow relie ?!)
This is probably the sloppiest class in the game as it was written towards the end of the game jam and does a lot of different stuff.
This should also probably be subclassed from GameObject or something as much of its implementation is identical to Player.h
*/

//define guards
#ifndef ENEMY_HEADER
#define ENEMY_HEADER

//include SDL so it can store its texture
#include <SDL.h>

/**
First strange thing about this class. Using an enum to define different enemy subtypes
As far as I can remember:
TYPE1 - The guy that goes up and down, shoots at the player, then flys across the screen
TYPE2 - The guy that follows a (sort of) sine wave across the screen - These guys have a habit of flying off the top or bottom of the screen
TYPE3 - Homes in on the player (same code as AmmoPack basically)
TYPE4 - The boss - enters from the right, flys up and down shooting three bullets at once.

This NEEDS to be changed into subclasses, this is sloppy.
*/
enum enemyType{ TYPE1, TYPE2, TYPE3 , TYPE4};

class Enemy{
private:

	// enemy type as discussed
	enemyType type;

	//A texture for the ship and one for the trail
	SDL_Texture* texture;
	SDL_Texture* trailTexture;

	//x y coords for position, w h for rendering
	int x;
	int y;
	int width;
	int height;

	//velocity values 
	float velX;
	float velY;

	//enemy ships have a power value (this is their health)
	int power;

	//Three timers here, one for update, one for animation and one to controll the shooting rate
	float timer;
	float animTimer;
	float shootingTimer;

	//This is not used anymore - was so that they could get updated by other classes
	float externalTimer;

	//All of these weird bools are for controlling conditional behaviour across update calls
	bool movingUp;
	bool movingDown;

	bool sineGoingUp;


	float changeVertical;
	float deceleration;

	//tag for deletion
	bool toDestroy;
	bool shooting;

	//movement speeds for each different type
	int type1MovementSpeed;
	int type2MovementSpeed;
	int type3MovementSpeed;
	int type4MovementSpeed;

	//current anim frame for the ship and it's trail
	int frame;
	int trailFrame;

public:

	//Constructor and destructor
	//Takes a texture for the ship and its trail, enmum enemyType, spawnY (no x val because they always spawn off the right of the screen) and a power value
	Enemy(SDL_Texture* texture, SDL_Texture* trailTexture, enemyType type, int spawnY, int power);
	~Enemy();

	//Update function. Takes dTime, velX and velY values (dosent need to, these get computed in the function) and the players pos so the ship can fly towared the player
	void update(float dTime, float velX, float velY, int playerX, int playerY);

	//Standard getters and setters
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

	enemyType getType();

	bool MovingUp();
	void setMovingUp(bool movingUp);
	bool MovingDown();
	void setMovingDown(bool movingDown);

	float getExternalTimer();
	void setExternalTimer(float time);

	bool ToDestroy();
	void setToDestroy(bool destroyu);
	bool Shooting();
	void setShooting(bool shooting);
};

#endif	//ENEMY_HEADER