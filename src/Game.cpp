/**
This file is the implementation of the game class. for more info on the game class please refer to Game.h
NOTE: this file is honestly about 20 times longer then it should be.
This contains all of the initialisation code for the game
all the game logic
all the ui code
all the rendering code
input handling
and taste based computing code
*/

#include "Game.h"

//Constructor. This initialises everyting used for the game pretty much
Game::Game(SDL_Renderer* _renderer, SDL_Window* _window){
	
	// seed the rng
	srand(time(NULL));
	
	//score is initialised to zero
	score = 0;

	//set renderer and window
	renderer = _renderer;
	window = _window;
	
	//set level num to 1, initialise a new level object
	levelNum = 1;
	level = new Level(levelNum);

	// loading the main song of the game
	std::cout << "loading main theme:" << std::endl;
	music = Mix_LoadMUS("Sound/Main_Theme.mp3");
	if (music != NULL){
		std::cout << "Succesfully loaded music!" << std::endl;
	}

	// load all of the sound effects
	playerShoot = Mix_LoadWAV("Sound/player_shoot.wav");
	enemyExplode = Mix_LoadWAV("Sound/enemy_explode.wav");
	ammoPackSound = Mix_LoadWAV("Sound/ammo_pack.wav");
	playerExplode = Mix_LoadWAV("Sound/player_explode.wav");

	// start playing the song
	Mix_PlayMusic(music, -1);

	//initilaise the text color
	text_color = { 255, 255, 255 };

	//initialise the score timer to 0
	scoreTimer = 0;

	//load the fonts
	font = TTF_OpenFont("Fonts/Thirteen-Pixel-Fonts.ttf", 30);
	bigFont = TTF_OpenFont("Fonts/Thirteen-Pixel-Fonts.ttf", 60);

	//load all the textures we need (the need for a TextureBank class becomes pretty apparent here
	playerShipTexture = loadPNG("Assets/ship.png");
	playerProjectileTexture = loadPNG("Assets/player_projectile.png");
	playerShipTrailTexture = loadPNG("Assets/ship_trail.png");
	enemyTexture = loadPNG("Assets/enemy_ships.png");
	enemyShipTrailTexture = loadPNG("Assets/enemy_ship_trail.png");
	enemyProjectileTexture = loadPNG("Assets/enemy_projectile.png");
	UITexture = loadPNG("Assets/hud.png");
	powerTickTexture = loadPNG("Assets/power_tick.png");
	backgroundTexture = loadPNG("Assets/background_test.png");
	starTexture = loadPNG("Assets/star.png");
	background01 = loadPNG("Assets/background_01_small.png");
	background02 = loadPNG("Assets/background_02_small.png");
	background03 = loadPNG("Assets/background_03_small.png");
	background04 = loadPNG("Assets/background_05_small.png");
	backgroundPlanetTexture = loadPNG("Assets/planet.png");
	explosionTexture = loadPNG("Assets/explosion.png");
	ammoPackTexture = loadPNG("Assets/ammo_pack.png");
	bossTexture = loadPNG("Assets/boss.png");
	startScreen = loadPNG("Assets/start_screen.png");
	enemyPowerTick = loadPNG("Assets/enemy_power_tick.png");

	//initialise all the positions of the differnt parallax background layers
	background01_1X = 0;
	background01_2X = 1280;

	background02_1X = 0;
	background02_2X = 1280;

	background03_1X = 0;
	background03_2X = 1280;

	background04_1X = 0;
	background04_2X = 1280;

	background01Y = 90;
	background02Y = 20;
	background03Y = 0;
	background04Y = -20;

	//initialise the speed that the background pan up and down
	backgroundUpDownSpeed = 0.01f;

	//the planet that scrolls across. gets set like 4 screen away and moves very slowly
	backgroundPlanetStartX = 5120;
	backgroundPlanetX = backgroundPlanetStartX;
	backgroundPlanetSpeed = 1;

	// make a new player and give it the right textures
	player = new Player(playerShipTexture, playerShipTrailTexture);

	//running is true
	globalRunning = true;

	//set the speeds and starting positions for the backgrounds
	backgroundSpeed = 10;
	background1X = 0;
	background2X = 1280;
	backgroundTimer = 0.f;

	//set the number of star field layers and the number of stars per layer
	starFieldLayers = 7;
	starsPerLayer = 70;

	//initialise the star field
	initStarField();

	//level timer is zero
	levelTimer = 0;
}

//This function initialises the star field
void Game::initStarField(){
	
	for (int i = 0; i < starFieldLayers; i++){ //for each layer
		for (int j = 0; j < starsPerLayer; j++){	//for the number of stars in a layer 
			int x = rand() % 1280;	// generate x and y values within the screen size
			int y = rand() % 720;
			Star* star = new Star(i + 1, starTexture, x, y);	//generate a new star object. size is proportional to the layer it is on
			starField.push_back(star);	// stick it in the list of stars
		}
	}

	//This is the same as above but for the foreground layer. (a single layer of stars that renders on top of everything else)
	for (int i = 0; i < starsPerLayer; i++){
		int x = rand() % 1280;
		int y = rand() % 720;
		Star* star = new Star(6, starTexture, x, y); //size = 6. this should probably be starFieldLayers + 1 or something
		foregroundStarField.push_back(star);	//stick it in the list of foreground stars so it can be rendered seperatly
	}
}


//This function loads a png as a texture. takes a cstr path
SDL_Texture* Game::loadPNG(char* path){
	std::cout << "Loading: " << path << "..." << std::endl;
	SDL_Surface* tempSurf = IMG_Load(path);	//use the function from SDL_imgage to load a png as a surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurf);	//convert the surface to a texture
	SDL_FreeSurface(tempSurf);	//throw the surface away
	if (texture != NULL){
		std::cout << "Loaded!" << std::endl;
	}
	return texture;	//return the texture
}

//returns global running
bool Game::isRunning(){
	return globalRunning;
}

//this is the main render function clears the render buffer then calls all the other render function, then swaps the buffers
void Game::render(){
	SDL_RenderClear(renderer);

	

	renderStarField();

	renderBackground();

	if (player->getPower() > 0){
		renderPlayer();
	}
	
	renderEnemies();

	renderProjectiles();

	renderEffects();

	renderAmmoPacks();


	//renderForegroundStarField();

	renderUI();

	// for the first 5 seconds that the game is running render the start screen
	if (timer < 5000){
		SDL_RenderCopy(renderer, startScreen, NULL, NULL);
	}

	SDL_RenderPresent(renderer);
}

// renders the UI
void Game::renderUI(){

	//set up a dest rect in the right position and size for one the first tick in the users power bar
	SDL_Rect powerTickDest;
	powerTickDest.w = 9;
	powerTickDest.h = 28;
	powerTickDest.x = 52;
	powerTickDest.y = 65;

	//for each power the player is render a power tick with 12 pixel gap
	for (int i = 0; i < player->getPower(); i++){
		SDL_RenderCopy(renderer, powerTickTexture, NULL, &powerTickDest);
		powerTickDest.x += 12;
	}

	// render the UI texture
	SDL_RenderCopy(renderer, UITexture, NULL, NULL);

	//conver the score into to a string
	std::string str = std::to_string(score);

	//convert the socre string to a cstr
	const char* txt = str.c_str();

	// rendering text code. kinda nightmarish - should have its own function
	SDL_Rect textDest{ 1060, 70, 0, 0 };	//dest for the text
	TTF_SizeText(font, txt, &textDest.w, &textDest.h);	//give this function refs to the dest w and h and it will make them the right size for the given font and string
	SDL_Surface* temp = TTF_RenderText_Solid(font, txt, text_color); // make a surface with the text on
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, temp);	//make a texture from that surface
	SDL_FreeSurface(temp);	//throw away that surface
	SDL_RenderCopy(renderer, t, NULL, &textDest);	//render that texture
	SDL_DestroyTexture(t);	//destroy that texture

	//if the player is dead render the game over screen
	if (player->getPower() <= 0){

		//this is basically the same as above
		std::string str = "PRESS SPACE TO RESTART!";

		const char* txt2 = str.c_str();

		SDL_Rect textDest2{ 1060, 70, 0, 0 };
		TTF_SizeText(bigFont, txt2, &textDest2.w, &textDest2.h);
		textDest2.x = (1280 / 2) - (textDest2.w / 2);	//tweaking the position of the rect so it is in the center of the screen after we have found the width and height
		textDest2.y = 300;
		SDL_Surface* temp = TTF_RenderText_Solid(bigFont, txt2, text_color);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
		SDL_RenderCopy(renderer, t, NULL, &textDest2);		//render it
		SDL_DestroyTexture(t);

	}
}

//render the player
void Game::renderPlayer(){
	
	//get the playes current frame
	int pFrame = player->getFrame();

	//set up a rect for the flame trail behind the player
	SDL_Rect trailRect;
	trailRect.w = player->getWidth();
	trailRect.h = player->getHeight();
	
	//stick it in the middle and just behind the player
	trailRect.x = player->getX() - 90;
	trailRect.y = player->getY();

	//Source rect for the trail
	SDL_Rect trailSource;
	trailSource.w = 64;
	trailSource.h = 64;
	trailSource.x = 0;
	trailSource.y = 0;

	//choose the correct animation depending on if the player is banking up or down
	(pFrame == 0) ? (trailSource.y = 64) : ((pFrame == 1) ? (trailSource.y = 0) : ((pFrame == 2) ? (trailSource.y = 128) : (trailSource.y = 64)));

	// if the players x velocity is greater than zero switch to the blue flame
	if (player->getVelX() > 0){
		trailSource.y += 192;
	}

	//get the animation frame of the trail
	int trailFrame = player->getTrailFrame();

	//set the sources x to the right frame in the sprire sheet
	trailSource.x = trailFrame * trailSource.w;

	//render the flame first (so the player gets renderd on top of it)
	SDL_RenderCopy(renderer, player->getTrailTexture(), &trailSource, &trailRect);
	
	//set up source and dest rects of the player ship
	SDL_Rect pRect;
	pRect.w = player->getWidth();
	pRect.h = player->getHeight();
	pRect.x = player->getX();
	pRect.y = player->getY();

	SDL_Rect sRect;
	sRect.w = player->getWidth();
	sRect.h = player->getHeight();
	sRect.x = 0;

	//change animation if the player is banking up or down
	(pFrame == 0) ? (sRect.y = 128) : ((pFrame == 1) ? (sRect.y = 0) : ((pFrame == 2) ? (sRect.y = 256) : (sRect.y = 128)));

	//render the player
	SDL_RenderCopy(renderer, player->getTexture(), &sRect, &pRect);
}

//render the enemies a lot of this is very simmilar to rendering the player, I will indicate where it differes
void Game::renderEnemies(){
	if (enemyList.size() > 0){
		for (int i = 0; i < enemyList.size(); i++){ //for each enemy in the list

			
			SDL_Rect trailSource;
			SDL_Rect trailDest;

			trailDest.w = enemyList.at(i)->getWidth();
			trailDest.h = enemyList.at(i)->getHeight();
			trailDest.y = enemyList.at(i)->getY();
			trailDest.x = enemyList.at(i)->getX() + 94;

			// enemies dont back up or down so no need to change animaation here
			trailSource.w = 64;
			trailSource.h = 64;
			trailSource.x = enemyList.at(i)->getTrailFrame() * 64;
			trailSource.y = 0;

			SDL_RenderCopy(renderer, enemyList.at(i)->getTrailTexture(), &trailSource, &trailDest);

			SDL_Rect enemyDest;
			enemyDest.x = enemyList.at(i)->getX();
			enemyDest.y = enemyList.at(i)->getY();
			enemyDest.w = enemyList.at(i)->getWidth();
			enemyDest.h = enemyList.at(i)->getHeight();

			
			SDL_Rect enemySource;
			enemySource.x = 0;
			enemySource.w = enemyList.at(i)->getWidth();
			enemySource.h = enemyList.at(i)->getHeight();
			
			//set different source rects for each type of enemy
			if (enemyList.at(i)->getType() == TYPE1){		
				enemySource.y = 0;
			}
			else if (enemyList.at(i)->getType() == TYPE2){
				enemySource.y = 128;
			}
			else if (enemyList.at(i)->getType() == TYPE3){
				enemySource.y = 256;
			}
			if (enemyList.at(i)->getType() == TYPE4){ // type 4 enemies are really big
				enemyDest.w = 512;
				enemyDest.h = 256;
				SDL_RenderCopy(renderer, bossTexture, NULL, &enemyDest);
			}
			else{ // if its not a type 4 just render it normally
				SDL_RenderCopy(renderer, enemyList.at(i)->getTexture(), &enemySource, &enemyDest);
			}

			//this is all to render the little ticks underneath the enemy. Functions much the same as the ui power meter
			SDL_Rect powerTickDest;
			powerTickDest.w = 12;
			powerTickDest.h = 34;
			
			powerTickDest.x = enemyDest.x + enemyDest.w / 2;
			powerTickDest.x -= (12 * enemyList.at(i)->getPower()) + (3 * (enemyList.at(i)->getPower() - 1));

			powerTickDest.y = enemyDest.y + enemyDest.h;

			for (int j = 0; j < enemyList.at(i)->getPower(); j++){
				SDL_RenderCopy(renderer, enemyPowerTick, NULL, &powerTickDest);
				powerTickDest.x += 15;
			}

		}
	}
}

//this function renders the projectiles
//A lot of the following functions look very simmilar
void Game::renderProjectiles(){
	for (int i = 0; i < projectileList.size(); i++){	//for each projectile in the projectiles list
		
		//set up a source rect at the appropriate sprite sheet frame
		SDL_Rect source;
		source.x = projectileList.at(i)->getFrame() * 64;
		source.y = 0;
		source.w = 64;
		source.h = 16;

		//set up a dest with the right position and dimentions
		SDL_Rect projectileRect;
		projectileRect.w = projectileList.at(i)->getWidth();
		projectileRect.h = projectileList.at(i)->getHeight();
		projectileRect.x = projectileList.at(i)->getX();
		projectileRect.y = projectileList.at(i)->getY();

		//render it using the objects texture and the source and dest rects
		SDL_RenderCopy(renderer, projectileList.at(i)->getTexture(), &source, &projectileRect);
	}

	for (int i = 0; i < enemyProjectileList.size(); i++){ //Do the same as above but for enemy projectiles
		SDL_Rect source;
		source.x = enemyProjectileList.at(i)->getFrame() * 16;  // enemy shots have a smaller size in the sprite sheet
		source.y = 0;
		source.w = 64;
		source.h = 16;

		SDL_Rect dest;
		dest.w = enemyProjectileList.at(i)->getWidth();
		dest.h = enemyProjectileList.at(i)->getHeight();
		dest.x = enemyProjectileList.at(i)->getX();
		dest.y = enemyProjectileList.at(i)->getY();

		//render it using the objects texture and the source and dest rects
		SDL_RenderCopy(renderer, enemyProjectileList.at(i)->getTexture(), &source, &dest);
	}

}

//renders the effects in the effects list
void Game::renderEffects(){
	if (effectsList.size() > 0){
		for (int i = 0; i < effectsList.size(); i++){	//for each effect
			
			//set up a source rect of the right size at the right frame
			SDL_Rect source;
			source.x = effectsList.at(i)->getFrame() * 64;
			source.y = 0;
			source.w = 64;
			source.h = 64;

			//set up a dest at the right size and the right position 
			SDL_Rect dest;
			dest.x = effectsList.at(i)->getX();
			dest.y = effectsList.at(i)->getY();
			dest.w = 128;
			dest.h = 128;

			//render it using the objects texture and the source and dest rects
			SDL_RenderCopy(renderer, effectsList.at(i)->getTexture(), &source, &dest);
		}
	}
}


//this renders the ammo packs in the ammo pack list
void Game::renderAmmoPacks(){
	if (ammoPackList.size() > 0){ 
		for (int i = 0; i < ammoPackList.size(); i++){	//for each ammo pack
			
			//Set up a source rect of the right size at the right frame
			SDL_Rect source;
			source.x = ammoPackList.at(i)->getFrame() * 34;
			source.y = 0;
			source.w = 34;
			source.h = 40;

			//set up dest rect of the right size and the right position
			SDL_Rect dest;
			dest.x = ammoPackList.at(i)->getX();
			dest.y = ammoPackList.at(i)->getY();
			dest.w = ammoPackList.at(i)->getWidth();
			dest.h = ammoPackList.at(i)->getHeight();

			//render it using the objects texture and the source and dest rects
			SDL_RenderCopy(renderer, ammoPackList.at(i)->getTexture(), &source, &dest);
		}
	}
}

//this function is responsible for rendering the parallax star field in the background. Alos the little planet that floats by
void Game::renderStarField(){

	//set up a dest for the planet at the right x value
	SDL_Rect planetDest;
	planetDest.x = backgroundPlanetX;
	planetDest.y = 200;
	planetDest.w = 128;
	planetDest.h = 128;

	//render the planet
	SDL_RenderCopy(renderer, backgroundPlanetTexture, NULL, &planetDest);

	for (int i = 0; i < starField.size(); i++){ //for each star in the star field

		//set up source and dest rects
		SDL_Rect starDest;
		starDest.x = starField.at(i)->getX();
		starDest.y = starField.at(i)->getY();
		starDest.w = starField.at(i)->getWidth();
		starDest.h = starField.at(i)->getHeight();


		SDL_Rect starSource;
		starSource.x = starField.at(i)->getFrame(); // animation not used
		starSource.y = 0;
		starSource.w = 1;
		starSource.h = 1;

		//render the star
		SDL_RenderCopy(renderer, starField.at(i)->getTexture(), &starSource, &starDest);
	}
}

// This function renders the stars in the foreground. I beleive this was actually disabled for the final game. Identical from the above function
void Game::renderForegroundStarField(){
	for (int i = 0; i < foregroundStarField.size(); i++){
		SDL_Rect starDest;
		starDest.x = foregroundStarField.at(i)->getX();
		starDest.y = foregroundStarField.at(i)->getY();
		starDest.w = foregroundStarField.at(i)->getWidth();
		starDest.h = foregroundStarField.at(i)->getHeight();

		SDL_Rect starSource;
		starSource.x = foregroundStarField.at(i)->getFrame();
		starSource.y = 0;
		starSource.w = 1;
		starSource.h = 1;

		SDL_RenderCopy(renderer, foregroundStarField.at(i)->getTexture(), &starSource, &starDest);
	}
}

//this function renders the parallax background layers 
// It just sets up source and dest rects for each layer and renders it (painter)
void Game::renderBackground(){
	SDL_Rect bg4Rect1;
	SDL_Rect bg4Rect2;

	bg4Rect1.x = background04_1X;
	bg4Rect1.y = background04Y;
	bg4Rect1.w = 1280;
	bg4Rect1.h = 720;

	bg4Rect2.x = background04_2X;
	bg4Rect2.y = background04Y;
	bg4Rect2.w = 1280;
	bg4Rect2.h = 720;

	SDL_RenderCopy(renderer, background04, NULL, &bg4Rect1);
	SDL_RenderCopy(renderer, background04, NULL, &bg4Rect2);

	SDL_Rect bg3Rect1;
	SDL_Rect bg3Rect2;

	bg3Rect1.x = background03_1X;
	bg3Rect1.y = background03Y;
	bg3Rect1.w = 1280;
	bg3Rect1.h = 720;

	bg3Rect2.x = background03_2X;
	bg3Rect2.y = background03Y;
	bg3Rect2.w = 1280;
	bg3Rect2.h = 720;

	SDL_RenderCopy(renderer, background03, NULL, &bg3Rect1);
	SDL_RenderCopy(renderer, background03, NULL, &bg3Rect2);

	SDL_Rect bg2Rect1;
	SDL_Rect bg2Rect2;

	bg2Rect1.x = background02_1X;
	bg2Rect1.y = background02Y;
	bg2Rect1.w = 1280;
	bg2Rect1.h = 720;

	bg2Rect2.x = background02_2X;
	bg2Rect2.y = background02Y;
	bg2Rect2.w = 1280;
	bg2Rect2.h = 720;

	SDL_RenderCopy(renderer, background02, NULL, &bg2Rect1);
	SDL_RenderCopy(renderer, background02, NULL, &bg2Rect2);

	SDL_Rect bg1Rect1;
	SDL_Rect bg1Rect2;

	bg1Rect1.x = background01_1X;
	bg1Rect1.y = background01Y;
	bg1Rect1.w = 1280;
	bg1Rect1.h = 720;

	bg1Rect2.x = background01_2X;
	bg1Rect2.y = background01Y;
	bg1Rect2.w = 1280;
	bg1Rect2.h = 720;

	SDL_RenderCopy(renderer, background01, NULL, &bg1Rect1);
	SDL_RenderCopy(renderer, background01, NULL, &bg1Rect2);
}

//This function updates the games level
void Game::updateLevel(float dTime){

	//increment the levels timer
	level->timer += dTime;

	//check the spawn list
	if (level->spawnList.size() > 0){
		if (level->timer > level->spawnList.at(0).spawnTime){ // if the zeroth element is ready to spawn
			//make a new Enemy with the values from the levels spawn list
			Enemy* e = new Enemy(enemyTexture, enemyShipTrailTexture, level->spawnList.at(0).enemy, level->spawnList.at(0).spawnY, level->spawnList.at(0).hitPoints);
			enemyList.push_back(e);	//stick the new enemy in the enemy list
			level->spawnList.erase(level->spawnList.begin()); // remove that element from the levels spawn list
		}
	}
	else{
		if (enemyList.size() <= 0){
			level = new Level(++levelNum); // if all the enemies have been dfeated construct a new level with the itterated level number
			//NOTE: I think this line may actually be responsible for some of the weirdness of creating new levels
			//We are passing levelNum += 1 but I dont think this is actually itterating the level number
			//i.e we are just constantly passing it 2 I think (might be wrong)
		}
	}
}


//This function updates all the enemies currently in the enemy list
void Game::updateEnemies(float dTime){
	if (enemyList.size() > 0){	//I think this check every time we access a vector is superflous if we are doing i < vec.size() anyway
		for (int i = 0; i < enemyList.size(); i++){
			
			// call the update function of the enemy list
			enemyList.at(i)->update(dTime, 0, 0, player->getX(), player->getY());

			//if its a type four and its shooting
			if (enemyList.at(i)->getType() == TYPE4 && enemyList.at(i)->Shooting()){
				enemyList.at(i)->setShooting(false); // it is no longer shooting
				Projectile* p1 = new Projectile(enemyList.at(i)->getX(), enemyList.at(i)->getY() + (enemyList.at(i)->getHeight() / 2), enemyProjectileTexture, -20, -10, 32, 32);
				Projectile* p2 = new Projectile(enemyList.at(i)->getX(), enemyList.at(i)->getY() + (enemyList.at(i)->getHeight() / 2), enemyProjectileTexture, -20, 10, 32, 32);
				Projectile* p3 = new Projectile(enemyList.at(i)->getX(), enemyList.at(i)->getY() + (enemyList.at(i)->getHeight() / 2), enemyProjectileTexture, -20, 0, 32, 32);
				enemyProjectileList.push_back(p1);
				enemyProjectileList.push_back(p2);
				enemyProjectileList.push_back(p3);
				//just spawns three projectiles (one forwards, two diagonal) when a type 4 shoots
			}

			//if its not a type 4 and its shooting
			else if (enemyList.at(i)->Shooting()){
				//spawn only one projectile
				Projectile* p = new Projectile(enemyList.at(i)->getX(), enemyList.at(i)->getY() + (enemyList.at(i)->getHeight() / 2), enemyProjectileTexture, -20, 0, 32, 32);
				enemyProjectileList.push_back(p);
				enemyList.at(i)->setShooting(false);
			}

			
			if (enemyList.at(i)->ToDestroy()){	//if its time to destroy and enemy
				Mix_PlayChannel(-1, enemyExplode, 0);	//play the explosion sound
				AmmoPack* a = new AmmoPack(ammoPackTexture, enemyList.at(i)->getX(), enemyList.at(i)->getY(), 5);	//spawn a new ammo pack at the enemies position
				//NOTE: Ive just realised were always spawning ammo packs with five ammo. This should be like enemy->getPower() + 1 or something
				// in other words I forgot to ever actually implement one of the games core gimmicks
				ammoPackList.push_back(a); // stick the ammo pack on the list


				if (enemyList.at(i)->getType() == TYPE4){ //if its a type 4 the player get 1000 score
					score += 1000;
				}
				else{ // otherwise they just get 250
					score += 250;
				}

				//remove from list - needs to call destructor as well
				enemyList.erase(enemyList.begin() + i);
			}
		}
	}
}

//This function updates the ammo packs
void Game::updateAmmoPacks(float dTime){
	if (ammoPackList.size() > 0){	//superflous check
		for (int i = 0; i < ammoPackList.size(); i++){ // for each ammo pack in the list
			ammoPackList.at(i)->update(dTime, player->getX(), player->getY()); // update the ammo pack
			if (ammoPackList.at(i)->ToDestroy()){	//if its marked for destruction
				delete ammoPackList.at(i);	//delete it
				ammoPackList.erase(ammoPackList.begin() + i); // erase it
			}
		}
	}
}


//This function is the main update function for the game. does lots of things
void Game::update(){

	//get delta time /time since last frame
	oldTime = currentTime;
	currentTime = SDL_GetTicks();
	dTime = (currentTime - oldTime);
	timer += dTime;
	frames ++;
		

	if (player->getPower() == 0){ // if players power is 0
		player->setPower(-1);	// set players power to -1 (?)
		Mix_PlayChannel(-1, playerExplode, 0);	// play the player explosion sound
		Effect* e = new Effect(explosionTexture, 7, player->getX(), player->getY()); // spawn a new explosion effect
		effectsList.push_back(e);	//stick it in the effects list
	}

	//call the handle inputs function
	handleInputs();
	
	//do nothing for the first five seconds 
	if (timer < 5000){
		// (?)
	}
	else{
		
		//increment the score timer
		scoreTimer += dTime;
		if (scoreTimer >= 1000 / 30){
			scoreTimer = 0;
			score += 10; // players score always goes up by 10
		}

		//call update functions
		updateBackground(dTime);
		updateStarField(dTime);
		updateLevel(dTime);
		updateEnemies(dTime);
		updateAmmoPacks(dTime);
		updateEffects(dTime);

		//If the user pressed the quit button
		if (userInput.quit == true){	
			globalRunning = false; //terminate the game loop
		}

		//these are the velocity values that get sent to the player's update function
		//these get initialised to zero
		float velX = 0;
		float velY = 0;

		// the following string of if/elses test the values stored in the player inputs struct and alter the players velocity valyes
		if (userInput.rightKey){
			velX = 5.f;	// player moves to the right slower then left
		}
		else{
			//if (velX > 0){
			velX -= 1.f;	//player is always moving slowly to the left
			//}
		}

		if (userInput.leftKey){
			velX = -10.f;	//player moves quickly to the left

		}
		else{
			if (velX < 0){
				velX += 0.2f;
			}
		}

		//player moves up and down a tth same speed 
		if (userInput.downKey){
			velY = 10.f;

		}
		else{
			if (velY > 0){
				velY -= 0.2f;
			}
		}

		if (userInput.upKey){
			velY = -10.f;

		}
		else{
			if (velY < 0){
				velY += 0.2f;
			}
		}

		//move the backgroudn up if the player is moving down and they are not off the screen
		if (userInput.downKey && player->getY() < (700 - player->getHeight())){

			BGup();
		}

		//move the background down if the player is moving up and they are not off the screen
		if (userInput.upKey && player->getY() > 128){
			BGdown();
		}

		// if the player has power update the player
		if (player->getPower() > 0){
			player->update(dTime, velX, velY); // pass it the dTime and the values we just computed
		}

		//if there is a space keypress stored in the user input struct
		if (userInput.spaceKey){
			if (player->getPower() > 0){
				Mix_PlayChannel(-1, playerShoot, 0);
				player->setPower(player->getPower() - 1);
				Projectile* p = new Projectile(player->getX() + player->getWidth(), player->getY() + (player->getHeight() / 2), playerProjectileTexture, 40, 0, 128, 32);
				projectileList.push_back(p);
				userInput.spaceKey = false;
			}
		}
		// run through players projectiles
		for (int i = 0; i < projectileList.size(); i++){
			
			//update them
			projectileList.at(i)->update(dTime);
			
			//destroy them if its time
			if (projectileList.at(i)->ToDestroy()){
				delete projectileList.at(i);
				projectileList.erase(projectileList.begin() + i);
			}
		}

		// run through the enemy projectiles
		for (int i = 0; i < enemyProjectileList.size(); i++){
	
			//update them
			enemyProjectileList.at(i)->update(dTime);

			//destroy them if its time to do so
			if (enemyProjectileList.at(i)->ToDestroy()){
				delete enemyProjectileList.at(i);
				enemyProjectileList.erase(enemyProjectileList.begin() + i);
			}
		}

		//This checks collisions with everything
		bool collision;
		//check collisions with the player and all of the enemies
		for (int i = 0; i < enemyList.size(); i++){
		

			collision = checkCollisions(
				player->getX(), enemyList.at(i)->getX(),
				player->getY(), enemyList.at(i)->getY(),
				player->getWidth(), enemyList.at(i)->getWidth(),
				player->getHeight(), enemyList.at(i)->getWidth());
			
			//if a collision is found, do the appropriate stuff
			if (collision){
				Mix_PlayChannel(-1, playerExplode, 0);
				player->setPower(0);
				Effect* e = new Effect(explosionTexture, 7, player->getX(), player->getY());
				effectsList.push_back(e);
			}
			
			//collision detecteion with enemies and player projectiles
			for (int j = 0; j < projectileList.size(); j++){

				collision = checkCollisions(
					enemyList.at(i)->getX(), projectileList.at(j)->getX(),
					enemyList.at(i)->getY(), projectileList.at(j)->getY(),
					enemyList.at(i)->getWidth(), projectileList.at(j)->getWidth(),
					enemyList.at(i)->getHeight(), projectileList.at(j)->getHeight());
				if (collision){
					//std::cout << "COLLISION FOUND!" << std::endl;
					delete projectileList.at(j);
					projectileList.erase(projectileList.begin() + j);
					//spawn an explosion here

					//CHANGE THIS TO SMALL EXPLOSION
					Effect* e = new Effect(explosionTexture, 7, enemyList.at(i)->getX(), enemyList.at(i)->getY());
					effectsList.push_back(e);

					enemyList.at(i)->setPower(enemyList.at(i)->getPower() - 1);
					//enemyList.erase(enemyList.begin() + i);
					//enemyList.at(i)->setToDestroy(true);
					break;
				}
			}
		}
		//collision detection for player and other objects; enemies, pickups, etc.
		if (player->getPower() > 0){
			for (int i = 0; i < enemyProjectileList.size(); i++){
				collision = checkCollisions(
					player->getX(), enemyProjectileList.at(i)->getX(),
					player->getY(), enemyProjectileList.at(i)->getY(),
					player->getWidth(), enemyProjectileList.at(i)->getWidth(),
					player->getHeight(), enemyProjectileList.at(i)->getHeight());
				if (collision){
					Mix_PlayChannel(-1, playerExplode, 0);
					delete enemyProjectileList.at(i);
					enemyProjectileList.erase(enemyProjectileList.begin() + i);
					player->setPower(player->getPower() - 1);
					Effect* e = new Effect(explosionTexture, 7, player->getX(), player->getY());
					effectsList.push_back(e);
				}
			}
			for (int i = 0; i < ammoPackList.size(); i++){
				collision = checkCollisions(
					player->getX(), ammoPackList.at(i)->getX(),
					player->getY(), ammoPackList.at(i)->getY(),
					player->getWidth(), ammoPackList.at(i)->getWidth(),
					player->getHeight(), ammoPackList.at(i)->getHeight());
				if (collision){
					Mix_PlayChannel(-1, ammoPackSound, 0);
					player->setPower(player->getPower() + ammoPackList.at(i)->getAmmo());
					delete ammoPackList.at(i);
					ammoPackList.erase(ammoPackList.begin() + i);
				}
			}
		}
	}

		render();
	
}

// move each of the backgrounds up by a set ammount
void Game::BGup(){
	background01Y -= 4 * backgroundUpDownSpeed;
	background02Y -= 3 * backgroundUpDownSpeed;
	background03Y -= 2 * backgroundUpDownSpeed;
	background04Y -= 1 * backgroundUpDownSpeed;
}

// move each of the background layers down by a set ammount
void Game::BGdown(){
	background01Y += 4 * backgroundUpDownSpeed;
	background02Y += 3 * backgroundUpDownSpeed;
	background03Y += 2 * backgroundUpDownSpeed;
	background04Y += 1 * backgroundUpDownSpeed;
}

//update function for parallax backgrounds
//there is two backgrounds that scroll together
//when one goes off the screen it is moved to the tail of the other one
//emulates continuous scrolling
void Game::updateBackground(float dTime){
	backgroundTimer += dTime;
	if (backgroundTimer > 1000 / 60){
		backgroundTimer = 0;

		background04_1X -= backgroundSpeed * 1;
		background04_2X -= backgroundSpeed * 1;

		if (background04_1X <= -1280){
			background04_1X = background04_2X + 1280;
		}
		if (background04_2X <= -1280){
			background04_2X = background04_1X + 1280;
		}

		background03_1X -= backgroundSpeed * 2;
		background03_2X -= backgroundSpeed * 2;

		if (background03_1X <= -1280){
			background03_1X = background03_2X + 1280;
		}
		if (background03_2X <= -1280){
			background03_2X = background03_1X + 1280;
		}

		background02_1X -= backgroundSpeed * 3;
		background02_2X -= backgroundSpeed * 3;

		if (background02_1X <= -1280){
			background02_1X = background02_2X + 1280;
		}
		if (background02_2X <= -1280){
			background02_2X = background02_1X + 1280;
		}

		background01_1X -= backgroundSpeed * 4;
		background01_2X -= backgroundSpeed * 4;

		if (background01_1X <= -1280){
			background01_1X = background01_2X + 1280;
		}
		if (background01_2X <= -1280){
			background01_2X = background01_1X + 1280;
		}

	}
}

// update function for Effects
// Runs update behaviour for all effects currently in the game
void Game::updateEffects(float dTime){
	if (effectsList.size() > 0){
		for (int i = 0; i < effectsList.size(); i++){
			effectsList.at(i)->update(dTime);
			if (effectsList.at(i)->ToDestroy()){
				effectsList.erase(effectsList.begin() + i);
			}
		}
	}
}

// updates the star field
void Game::updateStarField(float dTime){
	starFieldTimer += dTime;



	if (starFieldTimer > 1000 / 60){

		backgroundPlanetX -= 2 * backgroundPlanetSpeed;
		if (backgroundPlanetX == -128){
			backgroundPlanetX = backgroundPlanetStartX;
		}

		starFieldTimer = 0;

		for (int i = 0; i < starField.size(); i++){
			starField.at(i)->update(dTime);
		}

		for (int i = 0; i < foregroundStarField.size(); i++){
			foregroundStarField.at(i)->update(dTime);
		}
	}
}

// handles inputs, standard stuff
void Game::handleInputs(){
	if (timer < 5000){
		while (SDL_PollEvent(&evnt)){
			switch (evnt.type){
			case SDL_QUIT:
				userInput.quit = true;
				break;
			default:
				break;
			}
		}
	}

	else{

		while (SDL_PollEvent(&evnt)){
			switch (evnt.type){
			case SDL_QUIT:
				userInput.quit = true;
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym){
				case SDLK_RIGHT:
					userInput.rightKey = true;
					break;
				case SDLK_LEFT:
					userInput.leftKey = true;
					break;
				case SDLK_UP:
					userInput.upKey = true;
					break;
				case SDLK_DOWN:
					userInput.downKey = true;
					break;
				case SDLK_SPACE:
					if (player->getPower() <= 0){
						newGame();
					}
					userInput.spaceKey = true;
					break;
				case SDLK_ESCAPE:
					userInput.quit = true;
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (evnt.key.keysym.sym){
				case SDLK_LEFT:
					userInput.leftKey = false;
					break;
				case SDLK_RIGHT:
					userInput.rightKey = false;
					break;
				case SDLK_UP:
					userInput.upKey = false;
					break;
				case SDLK_DOWN:
					userInput.downKey = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}
}

// collision detection. Takes coords and W&H values for two objects. retunrs true if they are intersecting
bool Game::checkCollisions(int x1, int x2, int y1, int y2, int w1, int w2, int h1, int h2){
	return ((x1 + w1 > x2) && (x1 < x2 + w2) && (y1 + h1 > y2) && (y1 < y2 + h2));
}

// new game function. This is poorly implemented.
// deletes stuff and creates new stuff, I think this is causing bugs and needs to be re-implemented in a more elegant manner
void Game::newGame(){
	enemyList.erase(enemyList.begin() + enemyList.size() - 1);
	player = new Player(playerShipTexture, playerShipTrailTexture);
	timer = 0;
	score = 0;
	level->timer = 0;
	levelNum = 1;
}
