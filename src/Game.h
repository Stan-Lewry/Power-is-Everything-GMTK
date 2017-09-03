/**
Monolithic game class. This contains all game construction code, game logic, etc.
*/

//define guards
#ifndef GAME_HEADER
#define GAME_HEADER

//includes everything
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "Projectile.h"
#include "Star.h"
#include "Level.h"
#include "Effect.h"
#include "AmmoPack.h"
#include <vector>
#include <string>

//This is a little struct to store user input
struct UserInput{
	bool quit;
	bool upKey;
	bool downKey;
	bool leftKey;
	bool rightKey;
	bool spaceKey;

	//I dont know why there are velocity values in here
	int velX;
	int velY;
};


class Game{
private:

	//Music stuff for SDL_Mixer. The main soundtrack and all the sound effects used
	Mix_Music* music;
	Mix_Chunk* playerShoot;
	Mix_Chunk* enemyExplode;
	Mix_Chunk* ammoPackSound;
	Mix_Chunk* playerExplode;
	
	//Level 
	Level* level;

	//level number, used when instantiating a new level
	int levelNum;

	//score value, drawn to the top right of the screen
	int score;

	//SDL_TTF font stuff. Two fonts (normal for the ui, BIGFONT is for the menus.
	TTF_Font* font;
	SDL_Color text_color;
	TTF_Font* bigFont;

	//These vectors contain all the game objects used in the game
	std::vector<Enemy*> enemyList;
	std::vector<Projectile*> projectileList;
	std::vector<Projectile*> enemyProjectileList;
	std::vector<Projectile*> projectileDestroyList;
	std::vector<Star*> starField;
	std::vector<Star*> foregroundStarField;
	std::vector<Effect*> effectsList;
	std::vector<AmmoPack*> ammoPackList;
		
	//values for the star field
	int starFieldLayers;
	int starsPerLayer;

	//All the textures used. Get instantiated in this class then passed to the constructors of everything that needs a texture
	//Would probably be wise to have a texture bank class that stores textures, then just have each class hold a key
	SDL_Texture* playerShipTexture;
	SDL_Texture* playerProjectileTexture;
	SDL_Texture* playerShipTrailTexture;
	SDL_Texture* enemyTexture;
	SDL_Texture* enemyShipTrailTexture;
	SDL_Texture* enemyProjectileTexture;
	SDL_Texture* UITexture;
	SDL_Texture* powerTickTexture;
	SDL_Texture* backgroundTexture;
	SDL_Texture* starTexture;
	SDL_Texture* backgroundPlanetTexture;
	SDL_Texture* explosionTexture;
	SDL_Texture* ammoPackTexture;
	SDL_Texture* bossTexture;
	SDL_Texture* startScreen;
	SDL_Texture* enemyPowerTick;

	//Parallax backgrounds and all the variables needed to get that working
	SDL_Texture* background01;
	int background01_1X;
	int background01_2X;
	float background01Y;
	int background01_Speed;
	SDL_Texture* background02;
	int background02_1X;
	int background02_2X;
	float background02Y;
	int background02Speed;
	SDL_Texture* background03;
	int background03_1X;
	int background03_2X;
	float background03Y;
	int background03Speed;
	SDL_Texture* background04;
	int background04_1X;
	int background04_2X;
	float background04Y;
	int background04Speed;
	
	//float controlls how the background moves up and down
	float backgroundUpDownSpeed;

	//timer for the level class
	float levelTimer;

	//Pretty sure this is just the variables to make mars fly across the screen
	int backgroundPlanetStartX;
	int backgroundPlanetX;
	int backgroundPlanetSpeed;

	//SDL stuff, the renderer, window, and event object
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event evnt;

	// global running value. For the gameloop
	bool globalRunning;

	// input caught in the handle inputs function gets caught here to be used in the update function
	UserInput userInput;

	//Player class
	Player* player;

	//Stuff needed for delta time
	Uint32 oldTime;
	Uint32 currentTime;
	float dTime;
	int frames;
	float timer;

	//controls the speed that the score iterates
	float scoreTimer;

	//timers etc for parallax
	int backgroundSpeed;
	int background1X;
	int background2X;
	float backgroundTimer;

	//timer for the star field
	float starFieldTimer;

public:

	//Constructor and desturctor. Takes a renderer and a window
	Game(SDL_Renderer* renderer, SDL_Window* window);
	~Game();

	//Loads a png at the given path, returns an SDL_Texture
	SDL_Texture* loadPNG(char* path);

	//initialises the star field
	void initStarField();

	//main update function, calls the update functions of everything in the lists
	void update();

	//updates the background (parallax)
	void updateBackground(float dTime);

	//functions to scroll the background up and down =
	void BGup();
	void BGdown();
	
	//update function for the star field
	void updateStarField(float dTime);

	//the following functions update all the lists
	void updateLevel(float dTime);
	void updateEnemies(float dTime);
	void updateEffects(float dTime);
	void updateAmmoPacks(float dTime);

	//main render function - better design would potentially be to have this as its own class. Calls all the functions below it
	void render();
	void renderUI();
	void renderEnemies();
	void renderPlayer();
	void renderProjectiles();
	void renderStarField();
	void renderEffects();
	void renderAmmoPacks();
	void renderForegroundStarField();
	void renderBackground();

	//Handle inputs function. Polls input events and sticks them into the userInput struct
	void handleInputs();

	//Returns is running
	bool isRunning();

	// new game function. This dosent work very well
	void newGame();

	//collision detection. takes x,y,w,h, valyes of two objects and returns true if they are colliding
	bool checkCollisions(int x1, int x2,int y1,int  y2,int w1,int w2,int h1,int h2);
};


#endif	// GAME_HEADER