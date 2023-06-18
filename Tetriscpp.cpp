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
int speed = 1;			// block �ӵ� ����

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
				printf("??);
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
			printf("??);
		else
			printf("??);
	}

	for (x = 1; x <= GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2) - 1, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("?�");
	}

	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("??);
		else
			printf("??);
	}
}

// ��ϳ����� �浹�� �Ǻ�
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

// ����� �������� ������
// ��ȣ�� ���� 2ĭ ���� +-2
void ShiftLeft()
{
	// �浹 �ڵ� �߰�
	if (!DetectCollision(curPosX - 2, curPosY, blockModel[block_id]))
		return;

	deleteBlock(blockModel[block_id]);
	curPosX -= 2;
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


// ���� �ð����� ������� �Ʒ������� ����
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

// ��� ȸ��
int RotateBlock()
{
	int block_base = block_id / 4;
	int block_rotated = (block_base * 4) + ((block_id + 1) % 4);

	// ���� ��ȯ �� ��ϰ� �浹�� ���
	if (!DetectCollision(curPosX, curPosY, blockModel[block_rotated]))
		return 0;

	deleteBlock(blockModel[block_id]);
	block_id = (block_base * 4) + ((block_id + 1) % 4);
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id]);

	return 1;
}

// �����̽��� ������ �ٷ� ������ ������
void SpaceDown()
{
	while (BlockDown());
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


// main
int main()
{

	srand(time(NULL));
  
	while (1)
	{
		// ��� ���� ����
		block_id = rand() % 28;
		curPosX = GBOARD_ORIGIN_X + 6;
		curPosY = GBOARD_ORIGIN_Y;

		drawBoard();

		while (1)
		{

			if (BlockDown() == 0)	// ��Ʈ�������� ����� ��� ������
			{
				AddBlockToBoard();
				RemoveFillUpLine();
				break;
			}
			ProcessKeyInput();	// Ű���� �Է��� ����
		}
	}


	return 0;
}
