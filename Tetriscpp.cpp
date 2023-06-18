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

/* Ű���� Ű�� ���� */
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

// ���Ӻ���
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

int curPosX, curPosY;	// ���� Ŀ�� ��ġ ���� ����
int block_id;			// ������ ����� id

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

// ����� �׸�
void showBlock(char blockInfo[4][4])
{
	// ���� Ŀ���� ��ġ ��ȯ
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// Ŀ���� �����̸� 4*4 ������ ����� �׸�
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("��");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// ó�� Ŀ�� ��ġ�� ����
	}
}

// ����� ����
void deleteBlock(char blockInfo[4][4])
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// Ŀ���� �����̸� 4*4 ������ �׷ȴ� ����� ����
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("  ");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
		// ó�� Ŀ�� ��ġ�� ����
	}
}

// ����� �������� ������
// ��ȣ�� ���� 2ĭ ���� +-2
void ShiftLeft()
{
	deleteBlock(blockModel[block_id]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// ����� ���������� ������
void ShiftRight()
{
	deleteBlock(blockModel[block_id]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}


// ���� �ð����� ������� �Ʒ������� ����
void BlockDown()
{
	deleteBlock(blockModel[block_id]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// ����� ���������� ������
void ShiftRight()
{
	if (!DetectCollision(curPosX + 2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);
}

// Ű �Է��� ����
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
			}
		}

		Sleep(1000);
	}
}


// main
int main()
{
	srand(time(NULL));

	// ��� ���� ����
	block_id = rand() % 28;
	curPosX = GBOARD_ORIGIN_X + 6;
	curPosY = GBOARD_ORIGIN_Y;

	showBlock(blockModel[block_id]);
	deleteBlock(blockModel[block_id]);

	return 0;
}