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
int score = 0;			// ���� �߰�
int speed = 1;			// block �ӵ� ����
int score = 0;			// ���� �߰�

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


// Ŀ�� �����
void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
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

// ���Ӻ��忡 ��� ������ ����
void AddBlockToBoard()
{
	// 4*4 ������ ���鼭 ���Ӻ����� 0�� �κ��� 1�� �����Ͽ� ��� ���� ����
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

// �߰��� ��� ������ �����Ͽ� ���Ӻ��带 �ٽ� �׸�
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
				printf("��");
			else
				printf("  ");
		}
	}
}

// �� ������ ��� ������� ���� á�� ��� �ش� ���� ����
void RemoveFillUpLine()
{
	int y, x, line;

	// ���Ӻ��� ��ü�� ��
	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		// ��ü�� ���ٰ� ��� �� ���� ó������ ���������� 1�� ���
		// �� ���� �ϼ��ƴٰ� �Ǵ�
		if (x == GBOARD_WIDTH + 1)
		{
			// �ش� ���� �ʱ�ȭ
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			// ���� �ϳ��� �Ʒ��� ����
			y += 1;
			score += 10;	// �� ���� �� ���� ȹ��
		}
	}
	RedrawBlocks();
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

// ���Ӻ��忡 ��� ������ ����
void AddBlockToBoard()
{
	// 4*4 ������ ���鼭 ���Ӻ����� 0�� �κ��� 1�� �����Ͽ� ��� ���� ����
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

// �߰��� ��� ������ �����Ͽ� ���Ӻ��带 �ٽ� �׸�
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
				printf("��");
			else
				printf("  ");
		}
	}
}

// �� ������ ��� ������� ���� á�� ��� �ش� ���� ����
void RemoveFillUpLine()
{
	int y, x, line;

	// ���Ӻ��� ��ü�� ��
	for (y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		for (x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}

		// ��ü�� ���ٰ� ��� �� ���� ó������ ���������� 1�� ���
		// �� ���� �ϼ��ƴٰ� �Ǵ�
		if (x == GBOARD_WIDTH + 1)
		{
			// �ش� ���� �ʱ�ȭ
			for (line = 0; y - line > 0; line++)
				memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			// ���� �ϳ��� �Ʒ��� ����
			y += 1;
			score += 10;	// �� ���� �� ���� ȹ��
		}
	}
	RedrawBlocks();
}

// ����� ���� ���� �̻����� �ö���� ���� ����
int IsGameOver()
{
	if (!DetectCollision(curPosX, curPosY, blockModel[block_id]))
		return 1;

	return 0;
}

// ���� ������ �����ִ� �������̽�
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
		// ��� ���� ����
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

			if (BlockDown() == 0)	// ��Ʈ�������� ����� ��� ������
			{
				// ���� �浹 �� 0�� ��ȯ
				// ����� ���� ��ġ�� ����
				// ���Ӻ��带 �ٽ� �׸�
				AddBlockToBoard();
				RemoveFillUpLine();
				break;
			}
			ProcessKeyInput();	// Ű���� �Է��� ����
		}
	}

	getchar();

	return 0;
}
