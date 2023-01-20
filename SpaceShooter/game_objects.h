#pragma once

#define MAX_BULLETS_FOR_SHIP 25
#define DEFAULT_COUNT_ENEMY_SHIP 3
#define MAX_ENEMY_SHIP_AT_LINE 7


typedef enum {
	UP,
	DOWN
} Direction;

typedef struct
{
	int x, y;

} Bullet;


typedef struct
{
	int x, y;
	int size;
	int countBullets;
	Direction direction;
	Bullet bullets[MAX_BULLETS_FOR_SHIP];
} Ship;


