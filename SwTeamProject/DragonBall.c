#include "DragonBall.h"

void setDragonBallPos(DragonBall dgb[][3]) {
	srand(time(NULL));
	dgb[0][0].x = 30;
	dgb[0][0].y = 15;
	dgb[0][0].get = 0;
	dgb[0][1].x = 20;
	dgb[0][1].y = 10;
	dgb[0][1].get = 0;
	dgb[0][2].x = 0;
	dgb[0][2].y = 0;
	dgb[0][2].get = 0;

	dgb[1][0].x = 5;
	dgb[1][0].y = 10;
	dgb[1][0].get = 0;
	dgb[1][1].x = 20;
	dgb[1][1].y = 10;
	dgb[1][1].get = 0;
	dgb[1][2].x = 40;
	dgb[1][2].y = 7;
	dgb[1][2].get = 0;

	dgb[2][0].x = 45;
	dgb[2][0].y = 21;
	dgb[2][0].get = 0;
	dgb[2][1].x = 45;
	dgb[2][1].y = 21;
	dgb[2][1].get = 0;
	dgb[2][2].x = 45;
	dgb[2][2].y = 21;
	dgb[2][2].get = 0;
	//for (int i = 0; i < 7; i++) {
	//	//38 * 58
	//	//�ӽ÷� ��� �巡�ﺼ�� ��ġ�� �ʻ� ǥ���ϰ� �س���
	//	dgb[i].x = rand() % 58+1;
	//	dgb[i].y = rand() % 38 + 1;
	//	dgb[i].get = 0; 
	//}
}


int dragonBallGet(player *p, DragonBall dgb[]) {
	for (int i = 0; i < 3; i++) {
		//���� �÷��̾��� ��ġ�� ��ġ�ϰ� ���� ���� ���� ���� ������ ��쿡 
		if (dgb[i].x == p->x && (dgb[i].y == p->y || dgb[i].y == p->y+1 || dgb[i].y == p->y+2) && dgb[i].get == 0) {
			dgb[i].get = 1;
			p->balls++;
			p->totalBalls++;
			return 1;
		}
	}
	return 0;
}


//void checkDragonBall(void) {
//	
//};
//void stageCheck(void) {
//
//};
//void gotoNextStage(void) {
//
//};