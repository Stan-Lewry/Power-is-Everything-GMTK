/**
Header file for the level class. This class deals with setting the times in which enemies should spawn, etc
*/

#ifndef LEVEL_HEADER
#define LEVEL_HEADER

//includes Enemy class as it spawns enemies. Vector for lists, time for seeding random gen
#include "Enemy.h"
#include <vector>
#include <time.h>


// struct for spawning enemies. Has a time to spawn, enemy type, spawn y, and hit points.
// Level class' main purpose is to generate a list of these structs that get passed to Game to create enemy objects
struct Spawn{
	float spawnTime;
	enemyType enemy;
	int spawnY;
	int hitPoints;
};


class Level{
private:
	int levelNum;
	int enemiesPerWave;
	int enemyType;
	int numEnemies;
	int spawnPosition;
	int startHitPoints;

public:
	Level(int level);
	~Level();
	int getSpawnTime(int i);
	
	float timer;

	std::vector<Spawn> spawnList;
};

#endif	// LEVEL_HEADER