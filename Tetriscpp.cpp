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

// ���Ӻ���
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

// ���� Ŀ�� ��ġ x, y�� ����
void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ���� Ŀ�� ��ġ �޾ƿ���
COORD GetCurrentCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO	curInfo;
	COORD curPoint;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

// ���� �� ���Ӻ��� �ʱ�ȭ
void gameBoardinit()
{
	// ���Ӻ��� ������ 0���� �ʱ�ȭ
	memset(gameBoardInfo, 0, sizeof(int));

	// �ܺ� ���� 1�� �ʱ�ȭ
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}

	for (int x = 0; x < GBOARD_WIDTH + 2; x++)
		gameBoardInfo[GBOARD_HEIGHT][x] = 1;
}

// ���� ���� ȭ�鿡 ���
void drawBoard()
{
	int x, y;

	// ���� ���� �� �ʱ�ȭ�ϸ� ���
	gameBoardinit();

	// 1�� �ܺ� �κ��� �׸�
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("��");
		else
			printf("��");
	}

	for (x = 1; x <= GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2) - 1, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("��");
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("��");
		else
			printf("��");
	}
}

// main
int main()
{

	drawBoard();


	return 0;
}

