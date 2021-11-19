#include "NextStage.h"

void setStageDoor(StageDoor* std) {
	std[0].x = 46; std[0].y = 4;
	std[1].x = 46; std[1].y = 4;
	std[2].x = 46; std[2].y = 4;
}

void printStageDoor(StageDoor* std, int stage[][60], int stageNum) {
	int posX = std[stageNum].x * 2 + OriginX;
	int posY = std[stageNum].y + OriginY;
	SetCurrentCursorPos(posX, posY);
	printf("��");
}

int checkStageDoor(player* p, StageDoor* std, int stageNum) {
	if (p->balls == p->targetBall[p->stageNum] && p->x == std[stageNum].x && p->y + 2 == std[stageNum].y)
		return 1;
	else
		return 0;
}

void gotoNextStage(player* p, DragonBall dgball[][3], StageDoor* std, int stage[][60]) {
	p->stageNum++;
	printStage();
	printCloud(stage, dgball[p->stageNum]);

	p->life++; //respawnPlayer���� life�� ��ĭ �����Ƿ� �̸� �߰��صα�
	respawnPlayer(p, stage); //deletePlayer �Լ� ���� ���� ������Ű�Ƿ� ���� �������� printStageDoor ����ؾ���
	printStageDoor(std, stage, p->stageNum);

	p->balls = 0;
	UpdateRecord(p);
}