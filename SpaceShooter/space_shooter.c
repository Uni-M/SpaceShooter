#include "space_shooter.h"

char field[FIELD_HIGHT + 1][FIELD_WIDTH + 1];

Ship ship;
Ship enemys[];

int main()
{
	srand(time(NULL));
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hidecursor(consoleHandle);

	int score = 0;
	int maxScore = 0;
	bool inGame = true;
	bool pause = false;
	
	//clock_t now = clock(), diff;

	
	InitShip();
	

	while (inGame)
	{
		InitField();
		AddShipToField();
		AddBulletToField();
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

		BulletMoves();

		Sleep(10);
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
	ship.bullets[0].x = ship.x + 1;
	ship.bullets[0].y = ship.y - 1;
	ship.bullets->count += 1;
	ship.bullets->direction = UP;
}

void AddBulletToField()
{
	if (ship.bullets->direction == UP) field[ship.bullets[0].y][ship.bullets[0].x] = '^';
}

void BulletMoves()
{
	ship.bullets[0].y--;
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