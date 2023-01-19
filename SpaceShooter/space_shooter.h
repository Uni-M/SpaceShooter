#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "game_objects.h"


#define watchI(x) printf("#x is %d\n", x)
#define watchF(f) printf("#f is %g\n", f)
#define watchCh(ch) printf("#ch is %c\n", ch)

#define FIELD_WIDTH 55
#define FIELD_HIGHT 25

void InitField();
void AddShipToField();
void PrintField(int score, int maxScore);
void InitShip();
void MoveLeft();
void MoveRight();

void Shoot();
void BulletMoves();
void AddBulletToField();

void hidecursor(HANDLE consoleHandle);
void setcur(int x, int y);