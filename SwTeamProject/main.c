#include "Common.h"

#include "StageArr.h"
#include "DragonBall.h"
#include "NpcTrack.h"
#include "NextStage.h"
#include "StartScreen.h"
#include "cloud.h"
#include "Item.h"

int main()
{
	initCmd();
	printStartScreen();

	player p;
	NPC npc;
	DragonBall dgball[3][3];
	StageDoor stageDoor[3];
	cloud sCloud[5];
	NPC n[2];
	initNPC(&n[0], 10, 10);
	initNPC(&n[1], 50, 20);
	

	
	srand((unsigned int)time(NULL));

	npc.x = 1, npc.y = 1;     //�⺻ ��
	int jump = JUMPHEIGHT;
	int gameFlag = 1;
	int jumpFlag = 0;
	int count = 0;

	
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

	for (int i = 0; i < 5; i++)			//���� ���
		drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

	//������ ����
	int speed1 = 30, speed2 = 50;
	item item[50];
	InititemBox(speed1, speed2);

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
			deleteNpc(&npc, stageArr[p.stageNum]);
			/*npc.x = 1; npc.y = 1*/;

			//�÷��̾� ���� and ��ġ �缳�� and ��Ϻ���
			respawnPlayer(&p, stageArr[p.stageNum]);
			UpdateRecord(&p);
		}

		//�巡�ﺼ and �������� ���� Ȯ��
		if (dragonBallGet(&p, dgball[p.stageNum],stageArr[p.stageNum]) == 1) //�巡�ﺼ ��ġ�� �÷��̾� ��ġ �� �� ���� ������Ʈ
			UpdateRecord(&p);
		if (checkStageDoor(&p, stageDoor, p.stageNum)) { //�巡�ﺼ ���� ������ and �� �տ� �� ������ ���� ���������� �̵�
			p.stageNum++;
			gotoNextStage(&p, dgball, stageDoor, stageArr[p.stageNum]);
			initSpecialCloud(sCloud, 5);
			InititemBox(speed1, speed2);
		}

		processKeyInput(&p, stageArr[p.stageNum]);


		//�÷��̾� ������ �� npc�̵� ����
		//���� �˰��� ����
		//2���� NPC�� �÷��̾ Ư�� �Ÿ� ���� �� �� ����
		/*for (int i = 0; i < 2; i++) {
			addNpcCnt(&n[i]);
			int dis = getDistance(p.x - n[i].x, p.y - n[i].y);   //ĳ���Ϳ� npc������ �Ÿ�
			if (dis <= 1) {  //�ּ� �Ÿ� ��Ʈ2 �����̸� ������.
				deleteNpc(&n[i], stageArr[p.stageNum]);
				deletePlayer(&p, stageArr[p.stageNum]);
				respawnPlayer(&p, stageArr[p.stageNum]);
			}
			if (n[i].cnt % n[i].npcSpeed == 0) {
				deleteNpc(&n[i], stageArr[p.stageNum]);
				updateNpcPos(&p, &n[i]);
				drawNpc(&n[i]);
			}
		}*/
	

		//ĳ����, npc ���� ������
		Fallitem(&p, stageArr[p.stageNum]);
	}

	

	SetCurrentCursorPos(0, 0);

	printf("GameOver");

	return 0;
}