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

	npc.x = 1, npc.y = 1;     //기본 값
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

	for (int i = 0; i < 5; i++)			//구름 출력
		drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

	//아이템 관련
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
			deleteNpc(&npc, stageArr[p.stageNum]);
			/*npc.x = 1; npc.y = 1*/;

			//플레이어 삭제 and 위치 재설정 and 기록변경
			respawnPlayer(&p, stageArr[p.stageNum]);
			UpdateRecord(&p);
		}

		//드래곤볼 and 스테이지 종료 확인
		if (dragonBallGet(&p, dgball[p.stageNum],stageArr[p.stageNum]) == 1) //드래곤볼 위치와 플레이어 위치 비교 후 정보 업데이트
			UpdateRecord(&p);
		if (checkStageDoor(&p, stageDoor, p.stageNum)) { //드래곤볼 전부 모으고 and 문 앞에 서 있으면 다음 스테이지로 이동
			p.stageNum++;
			gotoNextStage(&p, dgball, stageDoor, stageArr[p.stageNum]);
			initSpecialCloud(sCloud, 5);
			InititemBox(speed1, speed2);
		}

		processKeyInput(&p, stageArr[p.stageNum]);


		//플레이어 움직인 후 npc이동 시작
		//추적 알고리즘 시작
		//2개의 NPC가 플레이어를 특정 거리 이하 일 때 추적
		/*for (int i = 0; i < 2; i++) {
			addNpcCnt(&n[i]);
			int dis = getDistance(p.x - n[i].x, p.y - n[i].y);   //캐릭터와 npc사이의 거리
			if (dis <= 1) {  //최소 거리 루트2 이하이면 끝내기.
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
	

		//캐릭터, npc 이후 아이템
		Fallitem(&p, stageArr[p.stageNum]);
	}

	

	SetCurrentCursorPos(0, 0);

	printf("GameOver");

	return 0;
}