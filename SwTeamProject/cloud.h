#pragma once
#define JUMPHEIGHT -7

typedef struct SPECIALCLOUD
{
	int movement;				//구름 움직임
	int cloudType;				//3 => 점프구름, 5 => 약한 구름, 7 = 깜박 구름, 11 => 지뢰구름
	int size;					//구름 크기
	int x, y;
	int vis;
}cloud;


int jumpHigh(int height)
{
	return (int)(height * 1.5);
}

void blink(int* cloudVis)
{
	if (*cloudVis == 0)
		*cloudVis = 1;
	else
		*cloudVis = 0;
}
void vanishCloud(int* cloudVis)
{
	blink(cloudVis);
}

int cloudColl(int x, int y, int size, int stage[][60])
{
	if (x <= 1 || x + size >= 59 || y <= 1 || y >= 39)
		return 1;
	else
		return 0;
}

void printJumpCloud()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3); //3 옥색
	printf("◎");
}

void printWeakCloud()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);		//10 연한 초록
	printf("◎");
}

void printBlinkCloud()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);	//8 회색
	printf("◎");
}

void printNormalCloud()
{
	printf("◎");
}

void printTrapCloud()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // 12 연한 빨강
	printf("▲");
}

void recoverCloud(int x, int y, int stage[][60]);


void drawSpecialCloud(cloud* cloud, int stage[][60]);


void deleteSpecialCloud(cloud* cloud, int stage[][60]);


void initSpecialCloud(cloud* cloudArray, int size);


int collSpecialCloud(cloud* cloudArr, player* p, int stage[][60]);


void moveCloud(cloud* cloudArr, int stage[][60]);

