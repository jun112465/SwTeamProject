#include "Common.h"

#include "StageArr.h"
#include "DragonBall.h"
#include "NpcTrack.h"
#include "NextStage.h"
#include "StartScreen.h"
#include "cloud.h"
#include "Item.h"

void gameStart(void);

int main()
{
	int tmpStageArr[3][40][60];
	memcpy(tmpStageArr, stageArr, sizeof(int) * 3 * 40 * 60);

	initCmd();
	while (1) {
		system("cls");
		//����ȭ�� 
		gameStart();
		memcpy(stageArr, tmpStageArr, sizeof(int) * 3 * 40 * 60);
	}

	return 0;
}

void gameStart(void) {
	initCmd();
	//����ȭ�� 
	int difficulty = printStartScreen();
	player p;
	DragonBall dgball[3][3];
	StageDoor stageDoor[3];
	cloud sCloud[5];

	NPC* npcArr = NULL;
	int npcNum = 1;

	npcArr = (NPC*)malloc(sizeof(NPC) * 3);

	srand((unsigned int)time(NULL));

	int jump = JUMPHEIGHT;
	int gameFlag = 1;
	int jumpFlag = 0;
	int count = 0;

	initPlayer(&p);
	nextStageEffect(p.stageNum);
	printStage();
	
	initNPC(npcArr, difficulty, p.stageNum, &npcNum);
	setDragonBallPos(dgball);

	initSpecialCloud(sCloud, 5, dgball[p.stageNum]);

	printCloud(stageArr[p.stageNum], dgball[p.stageNum]);
	drawPlayer(&p);
	UpdateRecord(&p);


	setStageDoor(stageDoor);
	printStageDoor(stageDoor, stageArr[p.stageNum], p.stageNum);

	for (int i = 0; i < 5; i++)			//���� ���
		drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

	zone z;
	InitZone(&z);
	safeZone(&z, p.stageNum, stageArr[p.stageNum]);
	printZone(&z, p.stageNum);
	



	//������ ����
	int speed1 = 1, speed2 = 5;

	item item[50];
	InititemBox(speed1, speed2);

	while (1)
	{
		if (isGameOver(&p))
			break;

		if (count % 5 == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				deleteSpecialCloud(&sCloud[i], stageArr[p.stageNum]);
			}

			for (int i = 0; i < 5; i++)
				moveCloud(&sCloud[i], stageArr[p.stageNum]);
			
			for (int i = 0; i < 5; i++) {
				drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);
			}
		}



		//�÷��̾� �̵�

		jumpFlag = playerJump(&p, &jump, stageArr[p.stageNum]);
		count++;


		if (jumpFlag == 1) //���� �Ϸ� �� ������ 
		{
			jump = collSpecialCloud(sCloud, &p, stageArr[p.stageNum]);
		}
		else if (jumpFlag == -1) //�߶��� ���
		{
			//npc �������ְ� ��ġ �缳��
			//deleteNpc(&npcArr, stageArr[p.stageNum]);
			/*npc.x = 1; npc.y = 1*/;


			//�÷��̾� ���� and ��ġ �缳�� and ��Ϻ���
			respawnPlayer(&p, stageArr[p.stageNum],difficulty,&npcArr,&npcNum);

			UpdateRecord(&p);
		}

		//�巡�ﺼ and �������� ���� Ȯ��
		if (dragonBallGet(&p, dgball[p.stageNum], stageArr[p.stageNum]) == 1) //�巡�ﺼ ��ġ�� �÷��̾� ��ġ �� �� ���� ������Ʈ
			UpdateRecord(&p);
		if (checkStageDoor(&p, stageDoor, p.stageNum)) { //�巡�ﺼ ���� ������ and �� �տ� �� ������ ���� ���������� �̵�
			//������ �������� ����ϸ� ���� ������
			if (p.stageNum == 2) {
				gameClearScreen();
				return;
			}
			p.stageNum++;
			nextStageEffect(p.stageNum);
			gotoNextStage(&p, dgball, stageDoor, stageArr[p.stageNum]);
			initSpecialCloud(sCloud, 5, dgball[p.stageNum]);
			for (int i = 0; i < 5; i++)			//���� ���
				drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);
			InititemBox(speed1, speed2);
			SetCurrentCursorPos(0, 0);
			initNPC(npcArr, difficulty, p.stageNum, &npcNum);

			safeZone(&z, p.stageNum, stageArr[p.stageNum]);
			printZone(&z, p.stageNum);
			p.x = p.spawnPos[p.stageNum][0];
			p.y = p.spawnPos[p.stageNum][1];
		}

		processKeyInput(&p, stageArr[p.stageNum]);


//		�÷��̾� ������ �� npc�̵� ����
//		���� �˰��� ����
		for (int i = 0; i < npcNum; i++) {
			addNpcCnt(&npcArr[i]);
			int dis = min(getDistance(p.x - npcArr[i].x, p.y - npcArr[i].y), getDistance(p.x - npcArr[i].x, p.y + 1 - npcArr[i].y)); //ĳ���Ϳ� npc������ �Ÿ�
			dis = min(dis, getDistance(p.x - npcArr[i].x, p.y + 2 - npcArr[i].y));
			if (dis == 0) {  //�ּ� �Ÿ� ��Ʈ2 �����̸� ������.
				deleteNpc(&npcArr[i], stageArr[p.stageNum]);
				deletePlayer(&p, stageArr[p.stageNum]);
				respawnPlayer(&p, stageArr[p.stageNum],difficulty,&npcArr,&npcNum);
			}
			if (npcArr[i].cnt % npcArr[i].npcSpeed == 0) {
				deleteNpc(&npcArr[i], stageArr[p.stageNum]);
				updateNpcPos(&z, &p, &npcArr[i]);
				drawNpc(&npcArr[i]);
			}
			else {
				deleteNpc(&npcArr[i], stageArr[p.stageNum]);
				drawNpc(&npcArr[i]);
			}
		}


		//ĳ����, npc ���� ������
		Fallitem(&p, stageArr[p.stageNum], npcArr, sCloud, 5, &z, difficulty, &npcNum);

	}
	if (npcArr != NULL)
	{
		free(npcArr);
		npcArr = NULL;
	}
	system("cls");
	printEndScreen();
}