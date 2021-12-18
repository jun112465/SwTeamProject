#include "DragonBall.h"

void setDragonBallPos(DragonBall dgb[][3]) {
	srand((unsigned int)time(NULL));
	dgb[0][0].x = 30;
	dgb[0][0].y = 15;
	dgb[0][0].get = 0;
	dgb[0][1].x = 20;
	dgb[0][1].y = 10;
	dgb[0][1].get = 0;
	dgb[0][2].x = 1000;
	dgb[0][2].y = 1000;
	dgb[0][2].get = 0;

	dgb[1][0].x = 5;
	dgb[1][0].y = 10;
	dgb[1][0].get = 0;
	dgb[1][1].x = 34;
	dgb[1][1].y = 8;
	dgb[1][1].get = 0;
	dgb[1][2].x = 1000;
	dgb[1][2].y = 1000;
	dgb[1][2].get = 0;

	dgb[2][0].x = 53;
	dgb[2][0].y = 15;
	dgb[2][0].get = 0;
	dgb[2][1].x = 5;
	dgb[2][1].y = 31;
	dgb[2][1].get = 0;
	dgb[2][2].x = 3;
	dgb[2][2].y = 7;
	dgb[2][2].get = 0;
}


int dragonBallGet(player *p, DragonBall *dgb, int stage[][60]) {
	for (int i = 0; i < 3; i++) {
		//공과 플레이어의 위치가 일치하고 공을 갖고 있지 않은 상태일 경우에 
		if (dgb[i].x == p->x && (dgb[i].y == p->y || dgb[i].y == p->y+1 || dgb[i].y == p->y+2) && dgb[i].get == 0) {
			dgb[i].get = 1;
			p->balls++;
			p->totalBalls++;
			stage[dgb[i].y][dgb[i].x] /= 89;
			return 1;
		}
	}
	return 0;
}

void printDragonBall() {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);		//빨강
	printf("●");
}