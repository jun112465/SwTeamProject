#include "Common.h"
#define MAX 50

typedef struct item {
	int x, y;
	int flag;       //0,1�� ���� ǥ�� ȭ�鿡 ǥ�� or ǥ��x
	int life;       //���� ���� ���ð�
	int speed;      //���� ������ �������� �ӵ�
	int itemNum;    //���ϴ� ������ �ѹ�
}item;
item it[MAX];
//������ �߻� �󵵿� �ӵ� �޶������� ��������!

void InititemBox(int speed1, int speed2);
int Detectitem(player* p, int i, int stage[][60]);
void Fallitem(player* p, int stage[][60]);
void showItem(int num);
void deleteItem(int i, int stage[][60]);
void func_item(int num, player *p);
void InitPosition(item* item);
