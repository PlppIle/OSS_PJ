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

// 게임보드
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

int curPosX, curPosY;	// 현재 커서 위치 저장 변수
int block_id;			// 내려올 블록의 id

// 현재 커서 위치 x, y로 변경
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 현재 커서 위치 받아오기
COORD GetCurrentCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO	curInfo;
	COORD curPoint;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

// 블록을 그림
void showBlock(char blockInfo[4][4])
{
	// 현재 커서의 위치 반환
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// 커서를 움직이며 4*4 공간에 블록을 그림
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("■");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// 처음 커서 위치로 복귀
	}
}

// 블록을 지움
void deleteBlock(char blockInfo[4][4])
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// 커서를 움직이며 4*4 공간에 그렸던 블록을 지움
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("  ");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// 처음 커서 위치로 복귀
	}
}

// main
int main()
{
	srand(time(NULL));

	// 블록 랜덤 생성
	block_id = rand() % 28;
	curPosX = GBOARD_ORIGIN_X + 6;
	curPosY = GBOARD_ORIGIN_Y;

	showBlock(blockModel[block_id]);
	deleteBlock(blockModel[block_id]);

	return 0;
}