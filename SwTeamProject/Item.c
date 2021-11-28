#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "cloud.h"
#include "Common.h"
#include "item.h"
#include "NpcTrack.h"
#include "characters.h"

#define MAX 10      // 아이템 최대 갯수

//아이템 박스 초기화
void InititemBox(int speed1, int speed2)
{
    for (int i = 0; i < MAX; i++)
    {
        it[i].flag = 1;   //초기화 당시 아이템 위치는 보여지면 안됨
        it[i].x = rand() % (stage1X - 2) + 1;
        it[i].y = 1;
        it[i].itemNum = itemNum(rand() % 5);
        it[i].life = rand() % 10;   // 대기시간
        it[i].speed = rand() % (speed2 - speed1) + speed1;   //개별 스피드 부여
    }
}

//매 반복문마다 아이템 생성
int itemNum(int num)
{
    if (num == 0)   return 13;
    else if (num == 1) return 17;
    else if (num == 2) return 19;
    else if (num == 3) return 23;
    else if (num == 4) return 29;
}

//아이템과 플레이어 충돌 감지
int Detectitem(player* p, int i, int stage[][60])
{
    int pX = OriginX + p->x * 2;
    int pY = OriginY + p->y;
    int itemX = OriginX + it[i].x * 2;
    int itemY = OriginY + it[i].y;
    if ((pX >= itemX && pX <= itemX) && (pY == itemY || pY + 1 == itemY || pY +2 == itemY)) {   
        InitPosition(&it[i]);        
        return 1;
    }
    else return 0;
}

//충돌시 충돌한 아이템의 좌표 초기화 (맵에 그대로 남아있는 것 해결)
void InitPosition(item* item)
{
    item->x = 0;
    item->y = 0;
    item->flag = 0;
}

//아이템 떨어짐
void Fallitem(player* p, int stage[][60])
{
    for (int i = 0; i < MAX; i++)
    {
        if (Detectitem(p, i, stage) == 1)
        {
            // 아이템 활성화
            func_item(it[i].itemNum, p, stage);   //itemNum 인덱스에 해당하는 기능 수행.
           // printf("%d", p->life);
        }

        if (it[i].flag == 1 && --it[i].life == 0)   //해당 인덱스에 아이템 있으면 움직임 실행, 현재 아이템 인덱스의 대기시간 턴제로 줄이면서 자신의 순서 기다림
        {
            int posX = OriginX + it[i].x * 2;
            int posY = OriginY + it[i].y;
           
            int pX = OriginX + p->x * 2;
            int pY = OriginY + p->y;
            
            
            if (posY == OriginY + stage1Y - 2)   //최대 세로 거리 임시 지정
            {    //최대 거리까지 갔다면 지워주고 flag check 수정
                deleteItem(i, stage);
                it[i].flag = 0;  //다시 화면표시x로 지정
                continue;
            }

            //대기시간 끝났으면 자신의 스피드만큼 
            it[i].life = it[i].speed;    //speed라는게 결국 턴제. 작을 수록 떨어지는 속도 빨라보임

            deleteItem(i, stage);
            it[i].y++;
            posY = OriginY + it[i].y;
            SetCurrentCursorPos(posX, posY);
            showItem(it[i].itemNum);
        }
    }
}

void showItem(int num)
{
    if (num == 13) printf("☎");
    else if (num == 17) printf("▼");
    else if (num == 19) printf("♪");
    else if (num == 23) printf("＠");
    else if (num == 29) printf("♨");
}

void deleteItem(int i, int stage[][60]) {
    int posX = OriginX + it[i].x * 2;
    int posY = OriginY + it[i].y;
    SetCurrentCursorPos(posX, posY);
    recoverCloud(it[i].x, it[i].y, stage);
}

void func_item(int itemNum, player *p, int stage[][60])
{
    if (itemNum == 13) p->life++;   //추가목숨부여
    else if (itemNum == 17) p->life--; //목숨 감점
    else if (itemNum == 19)    //순간이동
    {
        deletePlayer(p, stage);
        p->x = (OriginX + stage1X) / 2; p->y = (OriginY + stage1Y) / 2;
        drawPlayer(p);
    }
    //else if(itemNum == 23)     //기능 또 뭐 추가???
}


