#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "cloud.h"
#include "Common.h"
#include "item.h"
#include "NpcTrack.h"
#include "characters.h"

#define MAX 10      // ������ �ִ� ����

//������ �ڽ� �ʱ�ȭ
void InititemBox(int speed1, int speed2)
{
    for (int i = 0; i < MAX; i++)
    {
        it[i].flag = 1;   //�ʱ�ȭ ��� ������ ��ġ�� �������� �ȵ�
        it[i].x = rand() % (stage1X - 2) + 1;
        it[i].y = 1;
        it[i].itemNum = itemNum(rand() % 5);
        it[i].life = rand() % 10;   // ���ð�
        it[i].speed = rand() % (speed2 - speed1) + speed1;   //���� ���ǵ� �ο�
    }
}

//�� �ݺ������� ������ ����
int itemNum(int num)
{
    if (num == 0)   return 13;
    else if (num == 1) return 17;
    else if (num == 2) return 19;
    else if (num == 3) return 23;
    else if (num == 4) return 29;
}

//�����۰� �÷��̾� �浹 ����
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

//�浹�� �浹�� �������� ��ǥ �ʱ�ȭ (�ʿ� �״�� �����ִ� �� �ذ�)
void InitPosition(item* item)
{
    item->x = 0;
    item->y = 0;
    item->flag = 0;
}

//������ ������
void Fallitem(player* p, int stage[][60])
{
    for (int i = 0; i < MAX; i++)
    {
        if (Detectitem(p, i, stage) == 1)
        {
            // ������ Ȱ��ȭ
            func_item(it[i].itemNum, p, stage);   //itemNum �ε����� �ش��ϴ� ��� ����.
           // printf("%d", p->life);
        }

        if (it[i].flag == 1 && --it[i].life == 0)   //�ش� �ε����� ������ ������ ������ ����, ���� ������ �ε����� ���ð� ������ ���̸鼭 �ڽ��� ���� ��ٸ�
        {
            int posX = OriginX + it[i].x * 2;
            int posY = OriginY + it[i].y;
           
            int pX = OriginX + p->x * 2;
            int pY = OriginY + p->y;
            
            
            if (posY == OriginY + stage1Y - 2)   //�ִ� ���� �Ÿ� �ӽ� ����
            {    //�ִ� �Ÿ����� ���ٸ� �����ְ� flag check ����
                deleteItem(i, stage);
                it[i].flag = 0;  //�ٽ� ȭ��ǥ��x�� ����
                continue;
            }

            //���ð� �������� �ڽ��� ���ǵ常ŭ 
            it[i].life = it[i].speed;    //speed��°� �ᱹ ����. ���� ���� �������� �ӵ� ������

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
    if (num == 13) printf("��");
    else if (num == 17) printf("��");
    else if (num == 19) printf("��");
    else if (num == 23) printf("��");
    else if (num == 29) printf("��");
}

void deleteItem(int i, int stage[][60]) {
    int posX = OriginX + it[i].x * 2;
    int posY = OriginY + it[i].y;
    SetCurrentCursorPos(posX, posY);
    recoverCloud(it[i].x, it[i].y, stage);
}

void func_item(int itemNum, player *p, int stage[][60])
{
    if (itemNum == 13) p->life++;   //�߰�����ο�
    else if (itemNum == 17) p->life--; //��� ����
    else if (itemNum == 19)    //�����̵�
    {
        deletePlayer(p, stage);
        p->x = (OriginX + stage1X) / 2; p->y = (OriginY + stage1Y) / 2;
        drawPlayer(p);
    }
    //else if(itemNum == 23)     //��� �� �� �߰�???
}


