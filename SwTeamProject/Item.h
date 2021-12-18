#include "Common.h"
#define MAX 50
#define safeX 15
#define safeY 10


typedef struct item {
	int x, y;
	int flag;       //0,1로 상태 표시 화면에 표시 or 표시x
	int life;       //턴제 형식 대기시간
	int speed;      //개별 문자의 떨어지는 속도
	int itemNum;    //원하는 아이템 넘버
}item;
item it[MAX];
//아이템 발생 빈도와 속도 달라지도록 설정하자!

void InititemBox(int speed1, int speed2);
int Detectitem(player* p, int i, int stage[][60]);
void Fallitem(player* p, int stage[][60], NPC* npc, cloud* cloudArr, int CloudSize, zone* z);
void showItem(int num);
void deleteItem(int i, int stage[][60]);
void func_item(int itemNum, player* p, int stage[][60], NPC* npc, cloud* CloudArr, int CloudSize, zone* z, int difficulty, int* npcNum);
void InitPosition(item* item);
