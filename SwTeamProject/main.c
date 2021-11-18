#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "characters.h"
#include "cursor.h"
#include "stage.h"
#include "DragonBall.h"
#include "Track.h"
#include "cloud.h"



#define LEFT 75
#define RIGHT 77
#define JUMPHEIGHT -7

//특수구름 낱개 출력



//초기 화면 설정
void initCmd()
{
	RemoveCursor();
	system("mode con cols=200 lines=50");
}


void initPlayer(player* p)
{
	p->x = 56;
	p->y = 32;
	p->life = 3;
	p->totalBalls = 0;
	p->balls = 0;
	p->stageNum = 0;
	p->targetBall[0] = 2;
	p->targetBall[1] = 3;
	p->targetBall[2] = 3;
}

void initNPC(NPC* npc, int posX, int posY)
{
	npc->homeX = 1;
	npc->homeY = 1;
	npc->x = npc->homeX;
	npc->y = npc->homeY;
	npc->haveBall = 1;

	npc->cnt = 0; //npc 움직임 주기 카운트용 변수
	npc->npcSpeed = 7; //npc 움직임 주기
}


//맵 테두리 그려주기
void printStage()
{
	int i;

	//윗,아래 - 58
	for (i = 1; i < stage1X - 1; i++)
	{
		SetCurrentCursorPos(OriginX + (i * 2), OriginY);
		printf("─");
		SetCurrentCursorPos(OriginX + (i * 2), stage1Y + OriginY - 1);
		printf("─");
	}

	//옆
	for (i = 0; i < stage1Y; i++)
	{
		if (i == 0)
		{
			SetCurrentCursorPos(OriginX, i + OriginY);
			printf("┌");
			SetCurrentCursorPos(OriginX + (stage1X - 1) * 2, i + OriginY);
			printf("┐");
		}
		else if (i == stage1Y - 1)
		{
			SetCurrentCursorPos(OriginX, i + OriginY);
			printf("└");
			SetCurrentCursorPos(OriginX + (stage1X - 1) * 2, i + OriginY);
			printf("┘");
		}
		else
		{
			SetCurrentCursorPos(OriginX, i + OriginY);
			printf("│");
			SetCurrentCursorPos(OriginX + (stage1X - 1) * 2, i + OriginY);
			printf("│");
		}
	}
}


//맵 내부 그리기
void printCloud(int stage[40][60], DragonBall dgb[])
{
	//맵 그리기 전에 내부 한번 초기화 시켜주기
	//다음 스테이지 이동시 필요
	int i, j;
	int curX, curY;
	for (i = 1; i < stage1Y - 1; i++)
	{
		for (j = 1; j < stage1X - 1; j++)
		{
			curX = j * 2 + OriginX;
			curY = i + OriginY;
			SetCurrentCursorPos(curX, curY);
			printf("  ");
		}
	}

	//구름 그려주기
	for (i = 1; i < stage1Y - 1; i++)
	{
		for (j = 1; j < stage1X - 1; j++)
		{
			curX = j * 2 + OriginX;
			curY = i + OriginY;
			SetCurrentCursorPos(curX, curY);
			if (stage[i][j] == 2)
				printf("◎");
		}
	}

	//드래곤볼 그려주기
	for (int i = 0; i < 3; i++) {
		curX = dgb[i].x * 2 + OriginX;
		curY = dgb[i].y + OriginY;
		SetCurrentCursorPos(curX, curY);
		printf("●");
	}
}

void drawPlayer(player* p)
{
	/*if (p->y < 0)
		p->y = 0;*/
	int posX = OriginX + (p->x * 2);
	int posY = OriginY + p->y;
	SetCurrentCursorPos(posX, posY++);
	printf("●");
	SetCurrentCursorPos(posX, posY++);
	printf("■");
	SetCurrentCursorPos(posX, posY);
	printf("△");
}

void deletePlayer(player* p, int stage[][60])
{
	int posX = OriginX + (p->x * 2);
	int posY = OriginY + p->y;
	SetCurrentCursorPos(posX, posY++);								//해당 부분 다시 맵과 비교해서 복구
	recoverCloud(p->x, p->y, stage);
	SetCurrentCursorPos(posX, posY++);
	recoverCloud(p->x, p->y + 1, stage);
	SetCurrentCursorPos(posX, posY);
	recoverCloud(p->x, p->y + 2, stage);
}

//void drawNPC(NPC* npc)
//{
//	int posX = OriginX + (npc->x * 2);
//	int posY = OriginY + npc->y - 1;
//	SetCurrentCursorPos(posX, posY++);
//	printf("○");
//	SetCurrentCursorPos(posX, posY++);
//	printf("◇");
//	SetCurrentCursorPos(posX, posY);
//	printf("▲");
//}

void UpdateRecord(player* p)
{
	int curX = 135;
	int curY = 20;
	int i;
	SetCurrentCursorPos(curX, curY++);
	printf("현재 스테이지: %d", p->stageNum);
	SetCurrentCursorPos(curX, curY++);
	printf("남은 목숨: ");
	for (i = 0; i < p->life; i++)
		printf("♥");
	for (i = 0; i < 3 - p->life; i++)
		printf("  ");
	SetCurrentCursorPos(curX, curY++);
	printf("모은 드래곤 볼 총 개수: %d", p->totalBalls);
	SetCurrentCursorPos(curX, curY++);
	printf("이번 스테이지에서 모은 볼 개수: %d", p->balls);
	SetCurrentCursorPos(curX, curY++);
	printf("좌우 이동 <- -> 방향키");
}

int detectColl(int x, int y, int stage[][60])
{
	if (y < 0) y = 0;

	if (stage[y][x] != 0 && (stage[y][x] % 11 == 0 || stage[y + 1][x] % 11 == 0 || stage[y + 2][x] % 11 == 0)) //바닥에 추락
		return -1;
	else if (stage[y][x] == 0 || stage[y + 1][x] == 0 || stage[y + 2][x] == 0) //벽면 충돌
		return 2;
	else if (stage[y][x] == 1 && stage[y + 1][x] == 1 && stage[y + 2][x] == 1) //방해물 x
		return 0;
	//else if (stage[y][x] == 2 || stage[y + 1][x] == 2 || stage[y + 2][x] == 2) //구름과 겹쳐진 상태
	//	return 1;
	else
		return 1;
}

int jumpColl(int x, int y, int stage[][60])
{
	if (stage[y][x] != 1) //발이 구름
		return 3;
	if (stage[y + 1][x] != 1) //몸이 구름
		return 2;
	if (stage[y + 2][x] != 1) //머리가 구름
		return 1;
	return 0;
}

int JumpVector(int y)
{
	//위쪽으로 갈수록 y좌표가 작아지니, 올라가는 상황의 벡터는 음수
	float temp = -(2.0 / 5.0);
	return temp * y;
}

int playerJump(player* p, int* jump, int stage[][60])
{
	//캐릭터가 끼인 상태일 때 내려갈 수 있게 해주는 변수
	static int flag = 0;

	int moveY = JumpVector(*jump);
	(*jump)++;
	if (moveY > 2)
		moveY = 2;
	else if (moveY < -2)
		moveY = -2;

	int coll = detectColl(p->x, p->y - moveY, stage);

	//천장에 부딪히는 겨우
	if (coll == 2)
	{
		*jump = -(*jump);
		moveY = -moveY;
		coll = detectColl(p->x, p->y - moveY, stage);
	}

	Sleep(50);  //각 동작에 대한 지연
	if (moveY < 0 && coll == 1 && flag <= 0)		// 구름 위에 서 있는 상태 -> col == 1인 이유는 구름 밑으로 몸이 떨어지면 낑겨있는 상황이기 떄문
	{
		moveY = -1;
		int temp = detectColl(p->x, p->y - moveY, stage); //한칸 위로 이동했을 경우에서 detectColl 실행
		if (temp == 0) //구름 위에 서 있는 상태라면
		{
			//캐릭터 올려주기
			deletePlayer(p, stage);
			p->y -= moveY;
			drawPlayer(p);
		}
		return 1;
	}
	else if (coll == 0 || (coll != 2 && moveY >= 0) || flag > 0)	//일반적인 점프 or 구름에 껴 있는 경우
	{
		deletePlayer(p, stage);
		p->y -= moveY;
		drawPlayer(p);

		flag--; //flag 값이 0이하가 될 때까지 이 조건에 갇힌다. -> flag값이 양수인 것은 구름에 낑겨있는 상태. 

		if (coll != 0 && moveY == 0) //구름에 껴 있는 상황
			flag = jumpColl(p->x, p->y, stage);

		return 0;
	}
	else if (coll == -1)
		return -1;
	else 
		return 1;
}

void moveLeft(player* p, int stage[][60])
{
	if (detectColl(p->x - 1, p->y, stage) != 2)
	{
		deletePlayer(p,stage);
		p->x -= 1;
		drawPlayer(p);
	}
}

void moveRight(player* p, int stage[][60])
{
	if (detectColl(p->x + 1, p->y, stage) != 2)
	{
		deletePlayer(p,stage);
		p->x += 1;
		drawPlayer(p);
	}
}


void processKeyInput(player* p, int stage[][60])
{
	int i, key;

	for (i = 0; i < 5; i++)
	{
		if (_kbhit() != 0)
		{
			key = _getch();
			switch (key)
			{
			case LEFT:
				moveLeft(p, stage);
				break;
			case RIGHT:
				moveRight(p, stage);
				break;
			}
		}
	}
}

void respawnPlayer(player* p, int stage[][60])
{
	deletePlayer(p, stage);
	p->x = 56;
	p->y = 32;
	p->life--;
}

int isGameOver(player* p)
{
	if (p->life <= 0)
		return 1;
	return 0;
}

int distance(int x, int y)   //캐릭터와 npc거리 잴 때
{
	return x * x + y * y;
}

//추적 알고리즘 모듈 정리
void deleteNpc(NPC *npc, int stageNum) {
	int posX = OriginX + (npc->x * 2);
	int posY = OriginY + npc->y;
	SetCurrentCursorPos(posX, posY);
	recoverCloud(npc->x, npc->y, stageArr[stageNum]);
	
}
void drawNpc(NPC *npc) {
	int posX = OriginX + (npc->x * 2);
	int posY = OriginY + npc->y;
	SetCurrentCursorPos(posX, posY);
	printf("○");  //임시 npc
}


//스테이지 이동 관련 함수들 - 출력을 해야 하기 때문에 따로 모듈로 만들기 x
void setStageDoor(StageDoor* std) {
	std[0].x = 46; std[0].y = 4;
	std[1].x = 46; std[1].y = 4;
	std[2].x = 46; std[2].y = 4;
}
void printStageDoor(StageDoor* std, int stage[][60], int stageNum) {
	int posX = std[stageNum].x * 2 + OriginX;
	int posY = std[stageNum].y + OriginY;
	SetCurrentCursorPos(posX, posY);
	printf("★");
}
void gotoNextStage(player* p, DragonBall dgball[][3], StageDoor *std) {
	p->stageNum++;
	printStage();
	printCloud(stageArr[p->stageNum], dgball[p->stageNum]);
	
	p->life++; //respawnPlayer에서 life를 한칸 깍으므로 미리 추가해두기
	respawnPlayer(p, stageArr[p->stageNum]); //deletePlayer 함수 에서 문을 삭제시키므로 가장 마지막에 printStageDoor 출력해야함
	printStageDoor(std, stageArr[p->stageNum], p->stageNum);

	p->balls = 0;
	UpdateRecord(p);
}
int checkStageDoor(player* p, StageDoor *std, int stageNum) {
	if(p->balls == p->targetBall[p->stageNum] && p->x == std[stageNum].x && p->y+2 == std[stageNum].y)
		return 1;
	else
		return 0;
}


//특수 구름 출력, 움직임



int main()
{
	player p;
	NPC npc;
	DragonBall dgb[7];
	DragonBall dgball[3][3];
	StageDoor stageDoor[3];
	cloud sCloud[5];
	
	srand(NULL);

	npc.x = 1, npc.y = 1;     //기본 값
	int jump = JUMPHEIGHT;
	int gameFlag = 1;
	int jumpFlag = 0;
	int posX, posY;
	int count = 0;

	initCmd();
	printStage();
	initPlayer(&p);
	setDragonBallPos(dgball);

	initNPC(&npc, 1, 1);
	initSpecialCloud(sCloud, 5);
	printCloud(stageArr[p.stageNum], dgball[p.stageNum]);
	drawPlayer(&p);
	UpdateRecord(&p);

	//stageDoor
	setStageDoor(stageDoor);
	printStageDoor(stageDoor, stageArr[p.stageNum], p.stageNum);

	for (int i = 0; i < 5; i++)			//구름 출력
		drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

	while (1)
	{	
		if (isGameOver(&p))
			break;

		if (count % 5 == 0)
		{
			for (int i = 0; i < 5; i++)
				deleteSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

			for (int i = 0; i < 5; i++)
				moveCloud(&sCloud[i], stageArr[p.stageNum]);

			for (int i = 0; i < 5; i++)
				drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);
		}


		//플레이어 이동

		jumpFlag = playerJump(&p, &jump, stageArr[p.stageNum]);
		count++;


		if (jumpFlag == 1) //점프 완료 후 재점프 
		{
			jump = collSpecialCloud(sCloud, &p, stageArr[p.stageNum]);
		}
		else if (jumpFlag == -1) //추락한 경우
		{
			//npc 삭제해주고 위치 재설정
			deleteNpc(&npc, p.stageNum);
			npc.x = 1; npc.y = 1;

			//플레이어 삭제 and 위치 재설정 and 기록변경
			respawnPlayer(&p, stageArr[p.stageNum]);
			UpdateRecord(&p);
		}

		//드래곤볼 and 스테이지 종료 확인
		if (dragonBallGet(&p, dgball[p.stageNum]) == 1) //드래곤볼 위치와 플레이어 위치 비교 후 정보 업데이트
			UpdateRecord(&p);
		if (checkStageDoor(&p, stageDoor, p.stageNum)) { //드래곤볼 전부 모으고 and 문 앞에 서 있으면 다음 스테이지로 이동
			gotoNextStage(&p, dgball, stageDoor);
			initSpecialCloud(sCloud, 5);
		}

		processKeyInput(&p, stageArr[p.stageNum]);
		//플레이어 움직인 후 npc이동 시작
		//추적 알고리즘 시작
		//addNpcCnt(&npc);
		//int dis =getDistance(p.x - npc.x, p.y - npc.y);   //캐릭터와 npc사이의 거리
		//if (dis <= 1) {  //최소 거리 루트2 이하이면 끝내기.
		//	deleteNpc(&npc, p.stageNum);
		//	deletePlayer(&p);
		//	respawnPlayer(&p);
		//	npc.x = 1; npc.y = 1;
		//}
		//if (npc.cnt % npc.npcSpeed == 0){
		//	deleteNpc(&npc, p.stageNum);
		//	updateNpcPos(&p, &npc);
		//	drawNpc(&npc);
		//}

	}

	SetCurrentCursorPos(0, 0);
	
	printf("GameOver");

	getchar();

	return 0;
}