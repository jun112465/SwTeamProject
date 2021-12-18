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
		//시작화면 
		gameStart();
		memcpy(stageArr, tmpStageArr, sizeof(int) * 3 * 40 * 60);
	}

	return 0;
}

void gameStart(void) {
	initCmd();
	//시작화면 
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

	for (int i = 0; i < 5; i++)			//구름 출력
		drawSpecialCloud(&sCloud[i], stageArr[p.stageNum]);

	zone z;
	InitZone(&z);
	safeZone(&z, p.stageNum, stageArr[p.stageNum]);
	printZone(&z, p.stageNum);
	



	//아이템 관련
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
			//deleteNpc(&npcArr, stageArr[p.stageNum]);
			/*npc.x = 1; npc.y = 1*/;


			//플레이어 삭제 and 위치 재설정 and 기록변경
			respawnPlayer(&p, stageArr[p.stageNum],difficulty,&npcArr,&npcNum);

			UpdateRecord(&p);
		}

		//드래곤볼 and 스테이지 종료 확인
		if (dragonBallGet(&p, dgball[p.stageNum], stageArr[p.stageNum]) == 1) //드래곤볼 위치와 플레이어 위치 비교 후 정보 업데이트
			UpdateRecord(&p);
		if (checkStageDoor(&p, stageDoor, p.stageNum)) { //드래곤볼 전부 모으고 and 문 앞에 서 있으면 다음 스테이지로 이동
			//마지막 스테이지 통과하면 게임 끝내기
			if (p.stageNum == 2) {
				gameClearScreen();
				return;
			}
			p.stageNum++;
			nextStageEffect(p.stageNum);
			gotoNextStage(&p, dgball, stageDoor, stageArr[p.stageNum]);
			initSpecialCloud(sCloud, 5, dgball[p.stageNum]);
			for (int i = 0; i < 5; i++)			//구름 출력
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


//		플레이어 움직인 후 npc이동 시작
//		추적 알고리즘 시작
		for (int i = 0; i < npcNum; i++) {
			addNpcCnt(&npcArr[i]);
			int dis = min(getDistance(p.x - npcArr[i].x, p.y - npcArr[i].y), getDistance(p.x - npcArr[i].x, p.y + 1 - npcArr[i].y)); //캐릭터와 npc사이의 거리
			dis = min(dis, getDistance(p.x - npcArr[i].x, p.y + 2 - npcArr[i].y));
			if (dis == 0) {  //최소 거리 루트2 이하이면 끝내기.
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


		//캐릭터, npc 이후 아이템
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