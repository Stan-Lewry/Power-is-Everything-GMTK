#include "Level.h"

/*
spawn heights:
120
240
360
480
600
*/


Level::Level(int _levelNum){
	levelNum = _levelNum;
	srand(time(NULL));
	enemiesPerWave = levelNum * 3;
	numEnemies = 3;
	startHitPoints = 3;
	timer = 0;

	for (int i = 0; i < enemiesPerWave; i++){
		enemyType = rand() % 3;
		spawnPosition = 120 * (rand() % 4 + 1);

		Spawn s;
		switch (enemyType){
		case 0:  s = { getSpawnTime(i), TYPE1, spawnPosition, (levelNum - 1) + startHitPoints };
				 break;
		case 1:  s = { getSpawnTime(i), TYPE2, spawnPosition, (levelNum - 1) + startHitPoints };
				 break;
		case 2:  s = { getSpawnTime(i), TYPE3, spawnPosition, (levelNum - 1) + startHitPoints };
				 break;
		}


		spawnList.push_back(s);
	}

	Spawn s = { getSpawnTime(enemiesPerWave + 1), TYPE4, 240, enemiesPerWave };
	spawnList.push_back(s);



}

int Level::getSpawnTime(int i){
	return (i + 1) * 3000 - (levelNum * 100);
}