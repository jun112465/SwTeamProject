#include "StartScreen.h"

void printStartScreen() {

	SetCurrentCursorPos(OriginX, OriginY+2);
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("	�����������������������������������������������������������\n");
	printf("																									Sw���ʼ��� 7�� : ����, ������, ������, ���ؿ�");
	
	SetCurrentCursorPos(OriginX+20, OriginY + 25);
	printf("Press E to Play Easy Mode");
	SetCurrentCursorPos(OriginX + 20, OriginY + 27);
	printf("Press H to Play Hard Mode");
	SetCurrentCursorPos(OriginX + 20, OriginY + 29);
	printf("Press Q to Quit The Game");
	
	int k = getKeyInput();
	switch (k) {
	case E:
		return;
	case H:
		return;
	case Q:
		exit(-1);
	}
}

int getKeyInput() {

	while (_kbhit() == 0) {
		int key = _getch();
		switch (key) {
		case E:
			return E;
		case H:
			return H;
		case Q:
			return Q;
		}
	}
}