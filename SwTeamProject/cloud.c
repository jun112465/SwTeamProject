#include "cloud.h"


void recoverCloud(int x, int y, int stage[][60])
{
	if (stage[y][x] % 2 == 0)
		printNormalCloud();
	else if (stage[y][x] % 3 == 0)
		printJumpCloud();
	else if (stage[y][x] % 5 == 0)
		printWeakCloud();
	else if (stage[y][x] % 7 == 0)
		printBlinkCloud();
	else if (stage[y][x] % 11 == 0)
		printTrapCloud();
	else if (stage[y][x] == 1)
		printf("  ");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);		//7 흰색
}


void drawSpecialCloud(cloud* cloud, int stage[][60])
{
	int posX = OriginX + (cloud->x * 2);
	int posY = OriginY + cloud->y;
	int i;
	SetCurrentCursorPos(posX, posY);

	if (cloud->vis == 0)
		return;
	if (cloud->cloudType == 3)
	{
		for (i = 0; i < cloud->size; i++)
		{
			stage[cloud->y][cloud->x + i] *= cloud->cloudType;
			printJumpCloud();
		}
	}
	else if (cloud->cloudType == 5)
	{
		for (i = 0; i < cloud->size; i++)
		{
			stage[cloud->y][cloud->x + i] *= cloud->cloudType;
			printWeakCloud();
		}
	}
	else if (cloud->cloudType == 7)
	{
		for (i = 0; i < cloud->size; i++)
		{
			stage[cloud->y][cloud->x + i] *= cloud->cloudType;
			printBlinkCloud();
		}
	}
	else if (cloud->cloudType == 11)
	{
		for (i = 0; i < cloud->size; i++)
		{
			stage[cloud->y][cloud->x + i] *= cloud->cloudType;
			printTrapCloud();
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);		//7 흰색


	return;
}

void deleteSpecialCloud(cloud* cloud, int stage[][60])
{
	int posX = OriginX + (cloud->x * 2);
	int posY = OriginY + cloud->y;
	int i;


	if (stage[cloud->y][cloud->x] > 2)		//그려진 것을 지워야 함 => 1은 허공 => 없는 것을 지우면 X
	{
		for (i = 0; i < cloud->size; i++)
			stage[cloud->y][cloud->x + i] /= cloud->cloudType;

		SetCurrentCursorPos(posX, posY);

		for (i = 0; i < cloud->size; i++)
			recoverCloud(cloud->x + i, cloud->y, stage);
	}
}

void initSpecialCloud(cloud* cloudArray, int size)
{
	int i;
	int temp;
	for (i = 0; i < size; i++)
	{
		temp = rand() % 4;
		if (temp == 0)
			cloudArray[i].cloudType = 3;		//점프 구름
		else if (temp == 1)
			cloudArray[i].cloudType = 5;		//1회용 구름
		else if (temp == 2)
			cloudArray[i].cloudType = 7;		//깜박 구름
		else if (temp == 3)
			cloudArray[i].cloudType = 11;		//함정 구름


		cloudArray[i].movement = rand() % 5 - 3;		//0 정지, 1 좌우 2 상하 (+- 방향)
		cloudArray[i].size = rand() % 5 + 3;

		cloudArray[i].x = rand() % (59 - cloudArray[i].size) + 1;
		cloudArray[i].y = rand() % 37 + 2;
		cloudArray[i].vis = 1;
	}

	return;
}

int collSpecialCloud(cloud* cloudArr, player* p, int stage[][60])
{
	int temp = stage[p->y + 3][p->x];
	int i;

	for (i = 0; i < 5; i++)
	{
		int xS = cloudArr[i].x;
		int xE = cloudArr[i].x + cloudArr[i].size;
		int y = cloudArr[i].y;
		if (xS <= p->x && xE >= p->x && y == p->y + 3)
		{
			if (cloudArr[i].cloudType == 3)
				return jumpHigh(JUMPHEIGHT);
			else if (cloudArr[i].cloudType == 5)
			{
				vanishCloud(&(cloudArr[i].vis));
				return JUMPHEIGHT;
			}
		}
	}
	return JUMPHEIGHT;
}

void moveCloud(cloud* cloudArr, int stage[][60])
{
	static int blinkCount = 0;

	if (cloudArr->cloudType == 7)
	{
		if (blinkCount % 2 == 0)
			blink(&(cloudArr->vis));
	}
	blinkCount++;

	if (cloudArr->movement == 0)
		return;
	if (cloudArr->movement == 1)
	{
		if (cloudColl(cloudArr->x + 1, cloudArr->y, cloudArr->size, stage))
			cloudArr->movement *= -1;
		else
			cloudArr->x++;
	}
	else if (cloudArr->movement == -1)
	{
		if (cloudColl(cloudArr->x - 1, cloudArr->y, cloudArr->size, stage))
			cloudArr->movement *= -1;
		else
			cloudArr->x--;
	}
	else if (cloudArr->movement == 2)
	{
		if (cloudColl(cloudArr->x, cloudArr->y + 1, cloudArr->size, stage))
			cloudArr->movement *= -1;
		else
			cloudArr->y++;
	}
	else if (cloudArr->movement == -2)
	{
		if (cloudColl(cloudArr->x, cloudArr->y - 1, cloudArr->size, stage))
			cloudArr->movement *= -1;
		else
			cloudArr->y--;
	}
}

