#pragma once
#include "Common.h"

//스테이지 상에서 넘버 5로 표시
typedef struct DragonBall {
	//드래곤볼의 위치 좌표
	int x, y;
	//플레이어의 드래곤볼 보유 여부
	int get;
}DragonBall;

void setDragonBallPos(DragonBall dgb[][3]);
int dragonBallGet(player* p, DragonBall* dgb, int stage[][60]);
void printDragonBall(void);