#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "blockModel.h"

/*Size of Gameboard*/
#define GBOARD_HEIGHT 20
#define GBOARD_WIDTH 10

/*Size of Gameboard*/
#define GBOARD_ORIGIN_Y 2
#define GBOARD_ORIGIN_X 4

/* Å°º¸µå Å°°ª Á¤ÀÇ */
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

// °ÔÀÓº¸µå
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int curPosX, curPosY;	// ÇöÀç Ä¿¼­ À§Ä¡ ÀúÀå º¯¼ö
int block_id;			// ³»·Á¿Ã ºí·ÏÀÇ id
int score = 0;			// Á¡¼ö Ãß°¡
int speed = 1;			// block ¼Óµµ Á¶Á¤
int score = 0;			// Á¡¼ö Ãß°¡

// ÇöÀç Ä¿¼­ À§Ä¡ x, y·Î º¯°æ
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ÇöÀç Ä¿¼­ À§Ä¡ ¹Þ¾Æ¿À±â
COORD GetCurrentCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO	curInfo;
	COORD curPoint;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}


// Ä¿¼­ Áö¿ì±â
void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}


// ºí·ÏÀ» ±×¸²
void showBlock(char blockInfo[4][4])
{
	// ÇöÀç Ä¿¼­ÀÇ À§Ä¡ ¹ÝÈ¯
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// Ä¿¼­¸¦ ¿òÁ÷ÀÌ¸ç 4*4 °ø°£¿¡ ºí·ÏÀ» ±×¸²
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("¡á");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// Ã³À½ Ä¿¼­ À§Ä¡·Î º¹±Í
	}
}

// ºí·ÏÀ» Áö¿ò€
void deleteBlock(char blockInfo[4][4])
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// Ä¿¼­¸¦ ¿òÁ÷ÀÌ¸ç 4*4 °ø°£¿¡ ±×·È´ø ºí·ÏÀ» Áö¿ò
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("  ");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// Ã³À½ Ä¿¼­ À§Ä¡·Î º¹±Í
	}
}

// ½ÃÀÛ ½Ã °ÔÀÓº¸µå ÃÊ±âÈ­
void gameBoardinit()
{
	// °ÔÀÓº¸µå Á¤º¸¸¦ 0À¸·Î ÃÊ±âÈ­
	memset(gameBoardInfo, 0, sizeof(int));

	// ¿Üº® Á¤º¸ 1·Î ÃÊ±âÈ­
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (int x = 0; x < GBOARD_WIDTH + 2; x++)
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
}

// °ÔÀÓ º¸µå È­¸é¿¡ Ãâ·Â
void drawBoard()
{
	int x, y;

	// °ÔÀÓ ½ÃÀÛ ½Ã ÃÊ±âÈ­ÇÏ¸ç Ãâ·Â
	gameBoardinit();

	// 1ÀÎ ¿Üº® ºÎºÐÀ» ±×¸²
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("¦¦");
		else
			printf("¦¢");
	}

	for (x = 1; x <= GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2) - 1, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("¦¡");
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("¦¥");
		else
			printf("¦¢");
	}
}

// ºí·Ï³¢¸®ÀÇ Ãæµ¹À» ÆÇº°
int DetectCollision(int _x, int _y, char blockInfo[4][4])
{
	int x, y;
	int arrX = (_x - GBOARD_ORIGIN_X) / 2;
	int arrY = _y - GBOARD_ORIGIN_Y;

	for (x = 0; x < 4; x++)
	{
		for (y = 0; y < 4; y++)
		{
			if (blockInfo[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1)
				return 0;
		}
	}

	return 1;
}

// ºí·ÏÀ» ¿ÞÂÊÀ¸·Î ¿òÁ÷ÀÓ
// ±âÈ£´Â ¹®ÀÚ 2Ä­ Â÷Áö +-2
void ShiftLeft()
{
	// Ãæµ¹ ÄÚµå Ãß°¡
	if (!DetectCollision(curPosX - 2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// ºí·ÏÀ» ¿À¸¥ÂÊÀ¸·Î ¿òÁ÷ÀÓ
void ShiftRight()
{
	if (!DetectCollision(curPosX + 2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}


// ÀÏÁ¤ ½Ã°£¸¶´Ù ºí·ÏÀ¸·Î ¾Æ·¡ÂÊÀ¸·Î ³»¸²
int BlockDown()
{
	if (!DetectCollision(curPosX, curPosY + 1, blockModel[block_id]))
		return 0;

	deleteBlock(blockModel[block_id]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);

	return 1;
}

// ºí·Ï È¸Àü
int RotateBlock()
{
	int block_base = block_id / 4;
	int block_rotated = (block_base * 4) + ((block_id + 1) % 4);

	// ¹æÇâ ÀüÈ¯ ½Ã ºí·Ï°ú Ãæµ¹ÀÇ °æ¿ì
	if (!DetectCollision(curPosX, curPosY, blockModel[block_rotated]))
		return 0;

	deleteBlock(blockModel[block_id]);
	block_id = (block_base * 4) + ((block_id + 1) % 4);
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);

	return 1;
}

// ½ºÆäÀÌ½º¸¦ ´©¸£¸é ¹Ù·Î ¹ØÀ¸·Î ³»·Á¿È
void SpaceDown()
{
	while (BlockDown());
}

// °ÔÀÓº¸µå¿¡ ºí·Ï Á¤º¸¸¦ ÀúÀå
void AddBlockToBoard()
{
	// 4*4 ¿µ¿ªÀ» µ¹¸é¼­ °ÔÀÓº¸µåÀÇ 0ÀÎ ºÎºÐÀ» 1·Î º¯°æÇÏ¿© ºí·Ï Á¤º¸ ÀúÀå
	int x, y, arrCurX, arrCurY;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1)
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
		}
	}
}

// Ãß°¡µÈ ºí·Ï Á¤º¸¸¦ Æ÷ÇÔÇÏ¿© °ÔÀÓº¸µå¸¦ ´Ù½Ã ±×¸²
void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1)
				printf("¡á");
			else
				printf("  ");
		}
	}
}

// ÇÑ ¶óÀÎÀÌ ¸ðµÎ ºí·ÏÀ¸·Î °¡µæ Ã¡À» °æ¿ì ÇØ´ç ¶óÀÎ Á¦°Å
void RemoveFillUpLine()
{
	int y, x, line;

	// °ÔÀÓº¸µå ÀüÃ¼¸¦ µ¼
	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		// ÀüÃ¼¸¦ µ¹´Ù°¡ ¾î´À ÇÑ ÁÙÀÌ Ã³À½ºÎÅÍ ¸¶Áö¸·±îÁö 1ÀÏ °æ¿ì
		// ÇÑ ÁÙÀÌ ¿Ï¼ºµÆ´Ù°í ÆÇ´Ü
		if (x == GBOARD_WIDTH + 1)
		{
			// ÇØ´ç ÁÙÀ» ÃÊ±âÈ­
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			// ÁÙÀ» ÇÏ³ª¾¿ ¾Æ·¡·Î ³»¸²
			y += 1;
			score += 10;	// ÁÙ Á¦°Å ½Ã Á¡¼ö È¹µæ
		}
	}
	RedrawBlocks();
}

// Å° ÀÔ·ÂÀ» ¹ÞÀ½
void ProcessKeyInput()
{

	for (int i = 0; i < 20; i++)
	{
		int key;
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				RotateBlock();
				break;
			case SPACE:
				SpaceDown();
				break;
			}
		}

		Sleep(speed);
	}
}

// °ÔÀÓº¸µå¿¡ ºí·Ï Á¤º¸¸¦ ÀúÀå
void AddBlockToBoard()
{
	// 4*4 ¿µ¿ªÀ» µ¹¸é¼­ °ÔÀÓº¸µåÀÇ 0ÀÎ ºÎºÐÀ» 1·Î º¯°æÇÏ¿© ºí·Ï Á¤º¸ ÀúÀå
	int x, y, arrCurX, arrCurY;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPosY - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1)
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
		}
	}
}

// Ãß°¡µÈ ºí·Ï Á¤º¸¸¦ Æ÷ÇÔÇÏ¿© °ÔÀÓº¸µå¸¦ ´Ù½Ã ±×¸²
void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1)
				printf("¡á");
			else
				printf("  ");
		}
	}
}

// ÇÑ ¶óÀÎÀÌ ¸ðµÎ ºí·ÏÀ¸·Î °¡µæ Ã¡À» °æ¿ì ÇØ´ç ¶óÀÎ Á¦°Å
void RemoveFillUpLine()
{
	int y, x, line;

	// °ÔÀÓº¸µå ÀüÃ¼¸¦ µ¼
	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		// ÀüÃ¼¸¦ µ¹´Ù°¡ ¾î´À ÇÑ ÁÙÀÌ Ã³À½ºÎÅÍ ¸¶Áö¸·±îÁö 1ÀÏ °æ¿ì
		// ÇÑ ÁÙÀÌ ¿Ï¼ºµÆ´Ù°í ÆÇ´Ü
		if (x == GBOARD_WIDTH + 1)
		{
			// ÇØ´ç ÁÙÀ» ÃÊ±âÈ­
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			// ÁÙÀ» ÇÏ³ª¾¿ ¾Æ·¡·Î ³»¸²
			y += 1;
			score += 10;	// ÁÙ Á¦°Å ½Ã Á¡¼ö È¹µæ
		}
	}
	RedrawBlocks();
}

// ºí·ÏÀÌ ÀÏÁ¤ ¼öÁØ ÀÌ»óÀ¸·Î ¿Ã¶ó¿À¸é °ÔÀÓ Á¾·á
int IsGameOver()
{
	if (!DetectCollision(curPosX, curPosY, blockModel[block_id]))
		return 1;

	return 0;
}

// ÇöÀç Á¡¼ö¸¦ º¸¿©ÁÖ´Â ÀÎÅÍÆäÀÌ½º
void showScore()
{
	SetCurrentCursorPos(30, 10);
	printf("Score : %d", score);
	SetCurrentCursorPos(curPosX, curPosY);
}


// main
int main()
{
	srand(time(NULL));

	RemoveCursor();

	while (1)
	{
		// ºí·Ï ·£´ý »ý¼º
		block_id = rand() % 28;
		curPosX = GBOARD_ORIGIN_X + 6;
		curPosY = GBOARD_ORIGIN_Y;

		drawBoard();

		if (IsGameOver())
		{
			SetCurrentCursorPos(8, 1);
			printf("Game Over!!");
			break;
		}

		while (1)
		{
			showScore();

			if (BlockDown() == 0)	// Å×Æ®¸®½º¿¡¼­ ºí·ÏÀº °è¼Ó ³»·Á¿È
			{
				// ÀÌÈÄ Ãæµ¹ ½Ã 0À» ¹ÝÈ¯
				// ºí·ÏÀ» ÇöÀç À§Ä¡¿¡ ÀúÀå
				// °ÔÀÓº¸µå¸¦ ´Ù½Ã ±×¸²
				AddBlockToBoard();
				RemoveFillUpLine();
				break;
			}
			ProcessKeyInput();	// Å°º¸µå ÀÔ·ÂÀ» ¹ÞÀ½
		}
	}

	getchar();

	return 0;
}
