#include "space_shooter.h"

char field[FIELD_HIGHT + 1][FIELD_WIDTH + 1];

Ship ship;
Ship* enemys;

int main()
{
	srand(time(NULL));
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hidecursor(consoleHandle);

	int score = 0;
	int maxScore = 0;
	int countEnemys = DEFAULT_COUNT_ENEMY_SHIP;
	bool inGame = true;
	bool pause = false;
	
	clock_t start = clock(), diff;

	
	InitShip();
	InitEnemys(countEnemys);	

	while (inGame)
	{
		InitField();
		AddShipToField();
		AddBulletToField();

		AddEnemysToField(countEnemys);

		diff = clock() - start;

		if (diff > 1000)
		{
			EnemysMoves(countEnemys, &inGame);
			if((int)rand() % 3 == 0) NewEnemys(&countEnemys);
			start = clock();
		}
		BulletMoves();

		PrintField(score, maxScore);

		if (pause)
		{
			setcur((FIELD_WIDTH - 14) / 2, FIELD_HIGHT / 2);
			printf("=== PAUSE ===");
		}

		char c = (_kbhit()) ? _getch() : -1;

		if      (c == 'a' && !pause)   MoveLeft();
		else if (c == 'd' && !pause)   MoveRight();
		else if (c == 'q')             inGame = false;
		else if (c == 'w')             Shoot();
		else if (c == VK_SPACE)	       pause = !pause;

		

		CheckHit(&inGame, &score, &countEnemys);

		Sleep(200);
		setcur(0, 0);
	}
	return 0;
}

void InitField()
{
	for (int i = 0; i < FIELD_WIDTH; ++i)
	{
		field[0][i] = '#';
		field[FIELD_HIGHT][i] = '#';
	}
	field[0][FIELD_WIDTH] = '\0';

	strncpy(field[1], field[0], FIELD_WIDTH + 1);

	for (int i = 1; i < FIELD_WIDTH - 1; ++i)
	{
		field[1][i] = ' ';
	}
	
	for (int i = 2; i < FIELD_HIGHT; ++i)
	{
		strncpy(field[i], field[1], FIELD_WIDTH + 1);
	}

}

void PrintField(int score, int maxScore)
{
	printf("                      SPACE SHOOTER\n");
	printf("                      MAX SCORE....%d\n", maxScore);
	printf("                      SCORE........%d\n", score);

	for (int i = 0; i <= FIELD_HIGHT; ++i)
	{
		printf("%s\n", field[i]);
	}
}

void InitShip()
{
	ship.x = FIELD_WIDTH / 2;
	ship.y = FIELD_HIGHT - 2;
	ship.size = 3;
	ship.direction = UP;
	ship.countBullets = 0;
}

void AddShipToField()
{
	for (int i = ship.x; i < ship.x + ship.size; ++i)
	{
		if (i % 2 == 0) field[ship.y][i] = 'm'; // TODO fix?
		else field[ship.y][i] = 'H';
	}
}

void MoveLeft()
{
	if (ship.x > 1)
	{
		ship.x--;
	}
}

void MoveRight()
{
	if (ship.x + ship.size < FIELD_WIDTH - 1)
	{
		ship.x++;
	}
}

void Shoot()
{
	if (ship.countBullets < MAX_BULLETS_FOR_SHIP)
	{
		ship.countBullets += 1;
		ship.bullets[ship.countBullets - 1].x = ship.x + 1;
		ship.bullets[ship.countBullets - 1].y = ship.y - 1;
		ship.direction = UP;
	}
}

void AddBulletToField()
{
	for (int i = 0; i < ship.countBullets; ++i)
	{
		if (ship.direction == UP) field[ship.bullets[i].y][ship.bullets[i].x] = '^';
	}
}

void BulletMoves()
{
	for (int i = ship.countBullets - 1; i >= 0; --i)
	{
		if (ship.bullets[i].y > 1) ship.bullets[i].y--;
		else if (ship.bullets[i].y == 1) ship.countBullets--;
	}
}

void InitEnemys(int countEnemys)
{
	if ((enemys = (Ship*)malloc(sizeof(Ship) * countEnemys)) != NULL)
	{
		for (int i = 0; i < countEnemys; ++i)
		{
			enemys[i].size = (int)rand() % 3 + 2;
			enemys[i].direction = DOWN;
			enemys[i].countBullets = 0;
			enemys[i].x = (int)rand() % (FIELD_WIDTH - 3) + 1;
			enemys[i].y = 1;
		}
	}
}

void NewEnemys(int* countEnemys)
{
	int moreEnemis = *countEnemys + (int)rand() % 3;
	enemys = realloc(enemys, sizeof(Ship) * moreEnemis);
	if (enemys != NULL)
	{
		for (int i = *countEnemys; i < moreEnemis; ++i)
		{
			enemys[i].size = (int)rand() % 3 + 2;
			enemys[i].direction = DOWN;
			enemys[i].countBullets = 0;
			enemys[i].x = (int)rand() % (FIELD_WIDTH - 3) + 1;
			enemys[i].y = 1;
		}
	}

	*countEnemys = moreEnemis;
}

void AddEnemysToField(int countEnemys)
{
	for (int i = 0; i < countEnemys; ++i)
	{
		for (int j = enemys[i].x; j < enemys[i].x + enemys[i].size; ++j)
		{
			if (j % 2 == 0) field[enemys[i].y][j] = 'v';
			else field[enemys[i].y][j] = 'V';
		}
	}
}

void EnemysMoves(int countEnemys, bool* inGame)
{
	for (int i = 0; i < countEnemys; ++i)
	{
		enemys[i].y++;
		if (enemys[i].y > ship.y) *inGame = false;
	}
}

void CheckHit(bool* inGame, int* score, int* countEnemys)
{
	for (int i = 0; i < *countEnemys; ++i)
	{
		for (int j = enemys[i].x; j < enemys[i].x + enemys[i].size; j++)
		{
			if (field[enemys[i].y][j] == '^')
			{
				enemys[i].size = 0;
				//for .... ship.bullets[i]..... найти в цикле пулю которая попала и удалить её
			}
		}
	}
	watchI(*countEnemys);
	for (int i = 0; i < *countEnemys; ++i)
	{
		
		if (enemys[i].size == 0 && i + 1 < *countEnemys)
		{
			*score += 1;
			enemys[i] = enemys[i + 1];
			--i;
			*countEnemys -= 1;
		}
		else if (enemys[i].size == 0 && i + 1 == *countEnemys)
		{
			*score += 1;
			*countEnemys -= 1;
		}
	}
	watchI(*countEnemys);
	enemys = realloc(enemys, sizeof(Ship) * (*countEnemys));

}

void hidecursor(HANDLE consoleHandle)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}