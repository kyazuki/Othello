//���`FlipConfirm�֐��Ƀ~�X���� if������dir <= 8�ł͂Ȃ�dir <= 7

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <time.h>
#include "color.h"
#include "function.h"
#include "Stone Option.h"

//�e�X�g�p
#include <random>
using namespace std;
random_device rd;
mt19937 mt(rd());
void player_ran(int);

//�I�v�V����
bool arrange = true; //AI�̎v�l���o
bool ranmode = false; //AI�𗐐�AI�Ƒΐ�
bool gameloop = false; //�����Đ�
#define MODE 1 //����AI�̐ݒ�(���S�����_��:0, ����D�恨�ӂ�D�恨�����_��:1)
#define OUTPUT true //�������ʂ��t�@�C���ɏo�͂��邩�ǂ���(�L��:true, ����:false)
bool skip = true; //�X�L�b�v��L���ɂ��邩

int main() {
	if (OUTPUT) { //result.txt�Ɍ��݂̓��t�������o��(OUTPUT��true�ݒ莞�̂�)
		char date[64];
		time_t t = time(NULL);
		strftime(date, sizeof(date), "%Y/%m/%d %a %H:%M:%S", localtime(&t));
		fopen_s(&rfp, "result.txt", "a");
		fprintf(rfp, "%s\n", date);
		fclose(rfp);
	}
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	WindowSizeSet();
	GameInit();
	printf("Othello\n\n");
	Display(1);
	if (ranmode == false) {
			printf("�v���C���[������͂��Ă�������\n");
			setColor(COL_ME, COL_BACK_DEFAULT);
			scanf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		}
	printf("\n��U����͂��Ă�������\n");
	setColor(COL_ME, COL_BACK_DEFAULT);
	printf("%s", PlayerName);
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("(1) vs ");
	setColor(COL_ENEMY, COL_BACK_DEFAULT);
	printf("EnemyAI");
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("(2)\n1 or 2\n");
	char start[100];
	int loop = 1;
	while (loop) {
			scanf("%s", start);
			first = atoi(start);
			if (first == 1 || first == 2) loop = 0;
			else {
				setColor(COL_CAUTION1, COL_BACK_DEFAULT);
				printf("1��2�œ��͂��Ă�������\n");
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			}
		}
	do {
		system("cls");
		GameInit();
		StoneCount();
		if (first == 1) {
			printf("Turn 1 ��");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			printf("\n");
			P1_first();
		}
		else {
			printf("Turn 1 �Z");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(2);
			printf("\n");
			P2_first();
		}
		setColor(COL_VIOLET, COL_BACK_DEFAULT);
		printf("\n�����I��\n");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		StoneCount();
		MatchResult();
		if (OUTPUT) ResultOutput();
		if (gameloop == false) {
			printf("\n�Đ킵�܂����H�Đ킷��ꍇ�͐�U���U������ւ��܂�\ny/n\n");
			char gameagain[2];
			int loop2 = 1;
			while (loop2) {
				scanf("%s", gameagain);
				if (strcmp(gameagain, "y") == 0) { again = true; loop2 = 0; }
				else if (strcmp(gameagain, "n") == 0) { again = false; loop2 = 0; }
				else {
					setColor(COL_CAUTION1, COL_BACK_DEFAULT);
					printf("\ny��n�œ��͂��Ă�������\n");
					setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				}
			}
		}
		if (first == 1) first = 2;
		else first = 1;
	} while (gameloop || again);
}


//�E�B���h�E�T�C�Y�ύX
void WindowSizeSet(){
	BOOL bRtn;
	HANDLE hWrite;
	SMALL_RECT rctWindowRect = {0,0,149,38};
	COORD dwCoord = {150,39};

	hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	bRtn = SetConsoleScreenBufferSize(hWrite, dwCoord);
	bRtn = SetConsoleWindowInfo(hWrite,TRUE,&rctWindowRect);
}

//�����F�ύX
void setColor(int fg, int bg) {
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = 0;
	if (fg & COL_INTENSITY)
		attr |= FOREGROUND_INTENSITY;
	if (fg & COL_RED_MASK)
		attr |= FOREGROUND_RED;
	if (fg & COL_GREEN_MASK)
		attr |= FOREGROUND_GREEN;
	if (fg & COL_BLUE_MASK)
		attr |= FOREGROUND_BLUE;

	if (bg & COL_INTENSITY)
		attr |= BACKGROUND_INTENSITY;
	if (bg & COL_RED_MASK)
		attr |= BACKGROUND_RED;
	if (bg & COL_GREEN_MASK)
		attr |= BACKGROUND_GREEN;
	if (bg & COL_BLUE_MASK)
		attr |= BACKGROUND_BLUE;

	SetConsoleTextAttribute(hCons, attr);
}

//�v���C���[��U
void P1_first() {
	int EndNum = 0;
	while(1){
		StoneCount();
		if(FlipConfirm(1, false) == 0) { //�v���C���[���u���ꏊ������������
			EndNum++;
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			printf("�u����ꏊ�������̂Ńp�X���܂�\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			if (ranmode == false) player_in();
			else player_ran(MODE);
			StoneCount();
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			EndNum = 0;
			Display(1);
			printf("\n");
			for (int i = 0; i < (sizeof(setable) / sizeof(int)); i++) {
				setable[i] = 0;
			}
		}
		if(EndNum >= 2) { //2�l�Ƃ��΂�u����ꏊ�����������ꍇ
			break;
		}
		if(FlipConfirm(2, false) == 0) { //AI���u���ꏊ������������
			EndNum++;
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(2);
			printf("�u����ꏊ�������̂Ńp�X���܂�\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(2);
			player_ai();
			StoneCount();
			system("cls");
			Turn++;
			printf("Turn %d ��", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			EndNum = 0;
			Display(2);
			printf("\n");
			for (int i = 0; i < (sizeof(setable) / sizeof(int)); i++) {
				setable[i] = 0;
			}
		}
		if(EndNum >= 2) { //2�l�Ƃ��΂�u����ꏊ�����������ꍇ
			break;
		}
	}
}

//AI��U
void P2_first() {
	int EndNum = 0;
	while(1){
		StoneCount();
		if(FlipConfirm(2, false) == 0) { //AI���u���ꏊ������������
			EndNum++;
			system("cls");
			printf("Turn %d �Z", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(2);
			printf("�u����ꏊ�������̂Ńp�X���܂�\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d �Z", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(2);
			player_ai();
			StoneCount();
			system("cls");
			printf("Turn %d �Z", Turn);
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("EnemyAI");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			EndNum = 0;
			Display(2);
			printf("\n");
			for (int i = 0; i < (sizeof(setable) / sizeof(int)); i++) {
				setable[i] = 0;
			}
		}
		if(EndNum >= 2) { //2�l�Ƃ��΂�u����ꏊ�����������ꍇ
			break;
		}
		if(FlipConfirm(1, false) == 0) { //�v���C���[���u���ꏊ������������
			EndNum++;
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			printf("�u����ꏊ�������̂Ńp�X���܂�\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			if (ranmode == false) player_in();
			else player_ran(MODE);
			StoneCount();
			system("cls");
			Turn++;
			printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			EndNum = 0;
			Display(1);
			printf("\n");
			for (int i = 0; i < (sizeof(setable) / sizeof(int)); i++) {
				setable[i] = 0;
			}
		}
		if(EndNum >= 2) { //2�l�Ƃ��΂�u����ꏊ�����������ꍇ
			break;
		}
	}
}

//�������ʂ̕\��
void MatchResult(){
	if(StoneNum[0] > StoneNum[1]){
		result[0]++;
		if (first == 1)	printf("�Z");
		else printf("��");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("%s", PlayerName);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("�̏���\n\n����܂ł̐��\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("�v���C���[");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ��������: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
	else if(StoneNum[0] < StoneNum[1]){
		result[1]++;
		if (first == 1)	printf("��");
		else printf("�Z");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("�̏���\n\n����܂ł̐��\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("�v���C���[");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ��������: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
	else{
		result[2]++;
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("��������\n");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("\n����܂ł̐��\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("�v���C���[");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("��, ��������: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
}

//�΂����������������
void StoneCount(){
	StoneNum[0] = 0;
	StoneNum[1] = 0;
	for(int x = 0 ; x <= FrameNum - 1 ; x++){
		for(int y = 0 ; y <= FrameNum - 1 ; y++){
			if(StoneMap[x][y] == 1){
				StoneNum[0]++;
			}
			else if(StoneMap[x][y] == 2){
				StoneNum[1]++;
			}
		}
	}
}

//player���΂�u����ꏊ�����邩���ׂ�
int FlipConfirm(int player, bool pre){
	int Confirm = 0;
	for(int x = 0; x <= FrameNum - 1 ; x++){
		for(int y = 0 ; y <= FrameNum - 1 ; y++){
			for(int dir = 0 ; dir <= 7 ; dir++){
				if (FlipTest(dir, player, x, y)) {	//�S���̃}�X��T�����ċ��߂�΂����邩���ׂ�
					Confirm++;
					StoneSearch(Confirm, x, y, pre);
				}
			}
		}
	}
	return Confirm;
}

//���ۂɂЂ�����Ԃ��֐�
int FlipFlop(int dir, int player, int x, int y) {
	if (dir == 0) { for (int i = 0; i <= (FrameNum * FrameNum); i++) changestone[i] = 0; }
	int EnemyStone = 0;	//
	int Enemy;
	int Enemy_x[FrameNum];	//���߂�G�̐Έʒu��ۑ�����z��
	int Enemy_y[FrameNum];
	for(int i = 0; i <= FrameNum - 1 ;i++) { //������
		Enemy_x[i] = 0;
		Enemy_y[i] = 0;
	}
	if(player == 1) {
		Enemy = 2;
	}
	else {
		Enemy = 1;
	}
	while(1) {
		x = x + dir_x[dir];
		y = y + dir_y[dir];
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0) { //�T���͈͂��g�̊O�ɂȂ��Ă��܂�����
			//printf("x=%d,y=%d�g�O�ł�\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player) { //�����T�������Ɏ����̐΂��L������
			if(EnemyStone != 0) { //���߂�΂��L������
				changestone[0] = EnemyStone;
				EnemyStone--;
				for (int i = 1; EnemyStone >= 0; i++) {
					StoneMap[Enemy_x[EnemyStone]][Enemy_y[EnemyStone]] = player;
					//printf("x=%d y=%d���Ђ�����Ԃ��܂�\n",Enemy_x[EnemyStone],Enemy_y[EnemyStone]);
					changestone[i] = (Enemy_x[EnemyStone]) * 10 + Enemy_y[EnemyStone];
					EnemyStone--;
				}
				return 1;
			}
			else {
				//printf("x=%d,y=%d���߂�΂�����܂���\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy) { //�����T�������ɓG�̐΂��L������
			//printf("x=%d,y=%d�G�̐ΗL��\n",x,y);
			Enemy_x[EnemyStone] = x;
			Enemy_y[EnemyStone] = y;
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0) { //�����T���������J����������
			//printf("x=%d,y=%d�T���������J���ł�\n",x,y);
			return 0;
		}
	}
}

//�Ђ�����Ԃ��邩�e�X�g����֐�
int FlipTest(int dir,int player,int x,int y) { //dir:�T�������̎w��, player:�ǂ̃v���C���[�̑��삩 x,y:�����̍��̈ʒu
	int EnemyStone = 0;	//
	int Enemy;
	if(player == 1) {
		Enemy = 2;
	}
	else {
		Enemy = 1;
	}
	//printf("�����ʒux=%d,y=%d\n",x,y);
	if(StoneMap[x][y] != 0) { //���ɂȂ񂩐΂��L���Ēu���Ȃ��ꍇ
		return 0;
	}
	while(1) {
		x = x + dir_x[dir];
		y = y + dir_y[dir];
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0) { //�T���͈͂��g�̊O�ɂȂ��Ă��܂�����
			//printf("x=%d,y=%d�g�O�ł�\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player) { //�����T�������Ɏ����̐΂��L������
			if(EnemyStone != 0) { //���߂�΂��L������
				//printf("���߂�ΗL��\n");
				return 1;
			}
			else {
				//printf("x=%d,y=%d���߂�΂�����܂���\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy) { //�����T�������ɓG�̐΂��L������
			//printf("x=%d,y=%d�G�̐ΗL��\n",x,y);
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0) { //�����T���������J����������
			//printf("x=%d,y=%d�T���������J���ł�\n",x,y);
			return 0;
		}
	}
}

//�v���C���[����̓��͂��󂯕t����
void player_in() {
	char postxt[100];
	int pos,x,y;
	int return_flag = 1;
	if (first == 1)	printf("�Z");
	else printf("��");
	setColor(COL_ME, COL_BACK_DEFAULT);
	printf("%s", PlayerName);
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("�̃^�[��\n\n");
	do {
		int Loop = 1;
		getchar();
		while (Loop) {
			printf("�ʒu�ԍ�=");
			scanf("%[0-9]", postxt);
			pos = atoi(postxt);
			if (pos >= 11 && pos <= FrameNum * 10 + FrameNum && pos % 10 >= 1 && pos % 10 <= 8) {
					Loop = 0;
			}
			else {
				getchar();
				Loop = 1;
				system("cls");
				if (first == 1)	printf("Turn %d ��", Turn);
				else printf("Turn %d ��", Turn);
				setColor(COL_ME, COL_BACK_DEFAULT);
				printf("%s", PlayerName);
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				printf(" (");
				setColor(COL_ME, COL_BACK_DEFAULT);
				printf("%d", StoneNum[0]);
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				printf(" vs ");
				setColor(COL_ENEMY, COL_BACK_DEFAULT);
				printf("%d", StoneNum[1]);
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				printf(")\n\n");
				Display(1);
				if (first == 1) printf("�Z");
				else printf("��");
				setColor(COL_ME, COL_BACK_DEFAULT);
				printf("%s", PlayerName);
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				printf("�̃^�[��\n");
				setColor(COL_CAUTION1, COL_BACK_DEFAULT);
				printf("�l���s�K�؂ł��B���͂��Ȃ����Ă�������\n");
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			}
			strcpy(postxt, "Init");
		}
		y = pos % 10 - 1;
		x = (pos - y) / 10 - 1;
		int FlipResult = 0;
		for(int i = 0; i <= 7 ; i++) { //�S�����T���Œu���邩�ǂ�������
			if(FlipTest(i,1,x,y)) {
				FlipResult++;
			}
		}
		if(pos <= FrameNum * 10 + FrameNum && StoneMap[x][y] == 0 && FlipResult != 0) { //���̏ꏊ�ɐ΂������A��������Ђ�����Ԃ���Ȃ�
			return_flag = 0;
		}
		else {
			system("cls");
			if (first == 1) printf("Turn %d ��", Turn);
			else printf("Turn %d ��", Turn);
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" (");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%d", StoneNum[0]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(" vs ");
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("%d", StoneNum[1]);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf(")\n\n");
			Display(1);
			if (first == 1) printf("�Z");
			else printf("��");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf("�̃^�[��\n");
			setColor(COL_CAUTION2, COL_BACK_DEFAULT);
			printf("���̈ʒu�ɂ͒u���܂���B������x���͂��Ă�������\n");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		}
	} while(return_flag != 0);
	StoneMap[x][y] = 1;
	for(int dir = 0; dir <= 7 ; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//�S���ʒT�������Ď��ۂɂЂ�����Ԃ��֐�
	}
}

//AI����
void player_ai() {
	int pos, x, y;
	int return_flag = 1;
	if (first == 1) printf("��");
	else printf("�Z");
	setColor(COL_ENEMY, COL_BACK_DEFAULT);
	printf("EnemyAI");
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("�̃^�[��\n\n");
	printf("�ʒu�ԍ�=");
	if (arrange == true) printf("\n");
	
	if (StoneOption[0] > 1) { //�u����ꏊ���ꃖ���łȂ��Ȃ�
		if (arrange == true) {
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("�v�l��");
		}

		std::ofstream("board.dat");
		Save(0);
		pos = AlphaBeta(Prediction, 2, -1000000, 1000000, 0, 0);
		Load(0);
		remove("board.dat");

		if (arrange == true) {
			printf(".");
			Sleep(500);
			printf(".");
			Sleep(500);
			printf(".");
			Sleep(500);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		}
	}
	else if (StoneOption[0] == 1) pos = StoneOption[1]; //�u����ꏊ���ꃖ���Ȃ炻���ɒu��

	y = pos % 10;
	x = (pos - y) / 10;

	if (arrange == true) {
		system("cls");
		if (first == 1)	printf("Turn %d ��", Turn);
		else printf("Turn %d �Z", Turn);
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(" (");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("%d", StoneNum[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(" vs ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("%d", StoneNum[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(")\n\n");
		Display(2);
		if (first == 1) printf("��");
		else printf("�Z");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("�̃^�[��\n\n");
		printf("�ʒu�ԍ�=%d", pos + 11);
		Sleep(1500);
	}
	else printf("%d", pos + 11);

	StoneMap[x][y] = 2;
	for (int dir = 0; dir <= 7; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//�S���ʒT�������Ď��ۂɂЂ�����Ԃ��֐�
	}
}

//�����_������AI�v���C���[(�e�X�g�p)
void player_ran(int mode) {
	int pos, x, y;
	int return_flag = 1;
	if (first == 1) printf("�Z");
	else printf("��");
	setColor(COL_ME, COL_BACK_DEFAULT);
	printf("%s", PlayerName);
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("�̃^�[��\n\n");
	printf("�ʒu�ԍ�=");

	if (StoneOption[0] > 1) { //�u����ꏊ���ꃖ���łȂ��Ȃ�
		if (mode == 1) {
			if (setable[0] == 1) pos = 0;
			else if (setable[(FrameNum - 1) * 10] == 1) pos = (FrameNum - 1) * 10;
			else if (setable[(FrameNum - 1) * 10 + (FrameNum - 1)] == 1) pos = (FrameNum - 1) * 10 + (FrameNum - 1);
			else if (setable[FrameNum - 1] == 1) pos = FrameNum - 1;
			else for (int i = 1; i < FrameNum - 1; i++) {
				if (setable[i * 10] == 1) { pos = i * 10; break; }
				else if (setable[(FrameNum - 1) * 10 + i] == 1) { pos = (FrameNum - 1) * 10 + i; break; }
				else if (setable[i * 10 + (FrameNum - 1)] == 1) { pos = i * 10 + (FrameNum - 1); break; }
				else if (setable[i] == 1) { pos = i; break; }
				else {
					uniform_int_distribution<int> RanPos(1, StoneOption[0]);
					pos = StoneOption[RanPos(mt)];
				}
			}
		}
		else {
			uniform_int_distribution<int> RanPos(1, StoneOption[0]);
			pos = StoneOption[RanPos(mt)];
		}
	}
	else if (StoneOption[0] == 1) pos = StoneOption[1]; //�u����ꏊ���ꃖ���Ȃ炻���ɒu��

	y = pos % 10;
	x = (pos - y) / 10;

	printf("%d\n", pos + 11);

	StoneMap[x][y] = 1;
	for (int dir = 0; dir <= 7; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//�S���ʒT�������Ď��ۂɂЂ�����Ԃ��֐�
	}
}

//Alpha-Beta�@
float AlphaBeta(int depth, int player, float alpha, float beta, int count, int pass) {
	if (depth > 0 && FlipConfirm(player, true) == 0) {
		if (pass == 0 && player == 1) { player = 2; pass = 1; }
		else if (pass == 0 && player == 2) { player = 1; pass = 1; }
		else if (pass == 1) {
			StoneCount();
			if (StoneNum[0] < StoneNum[1]) return 10000;
			else if (StoneNum[0] > StoneNum[1]) return Evaluate(depth, player);
		}
	}
	else if (depth <= 0) return Evaluate(depth, player);
	for (int i = 0; i <= StoneOption[0] + 1; i++) StoneOptionPre[i][depth - 1] = StoneOption[i];
	int pos = 0;
	float val = 0;
	for (int i = 1; i <= StoneOptionPre[0][depth - 1]; i++) {
		Save(depth);
		int y = StoneOptionPre[i][depth - 1] % 10;
		int x = (StoneOptionPre[i][depth - 1] - y) / 10;
		StoneMap[x][y] = player;
		for (int dir = 0; dir <= 7; dir++) {
			FlipFlop(dir, StoneMap[x][y], x, y);
		}
		if (player == 1) val = AlphaBeta(depth - 1, 2, alpha, beta, count + 1, pass);
		else if (player == 2) val = AlphaBeta(depth - 1, 1, alpha, beta, count + 1, pass);
		pass = 0;
		Load(depth);
		if (player == 2 && val > alpha) {
			alpha = val;
			pos = StoneOptionPre[i][depth - 1];
			if (alpha >= beta) {
				if (count == 0) return StoneOptionPre[i][depth - 1];
				return beta;
			}
		}
		else if (player == 1 && val < beta) {
			beta = val;
			if (beta <= alpha)
				return alpha;
		}
	}
	if (player == 2) {
		if (count == 0) return pos;
		return alpha;
	}
	else if (player == 1) return beta;
}

//�����]���֐�
float Evaluate(int depth, int player) {
	float W1 = 0.4, W2 = 0.6, W3 = 0.7, W4 = 0.8; //�e�]���֐��̏d��
	float score = W1 * Evaluate1() + W2 * Evaluate2(depth) + W3 * Evaluate3(player) + W4 * Evaluate4();
	return score;
}

//�]���֐��@(�Έʒu�]����-15�`30(1000))
float Evaluate1() {
	int score = 0;
	for (int i = 0; i < FrameNum; i++) {
		for (int j = 0; j < FrameNum; j++) {
			if (StoneMap[i][j] == 2) score = score + boardscore[i * 10 + j]; //�����̐΂Ȃ�_���\�����_
			else if (StoneMap[i][j] == 1) score = score - boardscore[i * 10 + j]; //�G�̐΂Ȃ�_���\��茸�_
		}
	}
	float rscore = (float)score;
	return rscore;
}

//�]���֐��A(�J���x�]����-30�`0���炢�H)
float Evaluate2(int depth) {
	int score = 0, x = 0, y = 0;
	Save(Prediction + 1);
	Load(depth + 1);
	for (int i = 1; i <= changestone[0]; i++) { //�Ԃ����΂��A�Ԃ��O�̏�ԂŎ��͂ɋ󂫃}�X������Ȃ炻�̕����_
		y = changestone[i] % 10;
		x = (changestone[i] - y) / 10;
		if (StoneMap[x - 1][y - 1] == 0) score--;
		if (StoneMap[x][y - 1] == 0) score--;
		if (StoneMap[x + 1][y - 1] == 0) score--;
		if (StoneMap[x - 1][y] == 0) score--;
		if (StoneMap[x + 1][y] == 0) score--;
		if (StoneMap[x - 1][y + 1] == 0) score--;
		if (StoneMap[x][y + 1] == 0) score--;
		if (StoneMap[x + 1][y + 1] == 0) score--;
	}
	Load(Prediction + 1);
	float rscore = (float)score * 1.5;
	return rscore;
}

//�]���֐��B(�G����\���]����-10�`0���炢�H)
float Evaluate3(int player) {
	int score = 0;
	if (player == 1) score = score - StoneOption[0]; //�G�ǖʂȂ璅��\��1�����茸�_
	float rscore = (float)score * 3.0;
	return rscore;
}

//�]���֐��C(4�ӂ̊m��Ε]����-147�`49)
float Evaluate4() {
	int score = 0, up = 0, right = 0, left = 0, down = 0;
	//�l���ɐ΂��Ȃ��Ȃ�]�����Ȃ�
	if (StoneMap[0][0] == 0 && StoneMap[FrameNum - 1][0] == 0 && StoneMap[FrameNum - 1][FrameNum - 1] == 0 && StoneMap[0][FrameNum - 1] == 0) return 0;
	
	//���Ɏ����̐΂�����ꍇ
	if (StoneMap[0][0] == 2) { //������Ɏ����̐΂�����Ȃ�
		for (int i = 1; i <= FrameNum - 1; i++) { //�����̐΂��������тɉ��_
			if (StoneMap[i][0] == 2) {
				score++;
				if (i == FrameNum - 1) { up = true; score--; } //�E����܂ōs�������ӂ͂��ׂĎ���Ă���Ɛݒ�A�E������̐΂����炷
			}
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) { //�����̐΂��������тɉ��_
			if (StoneMap[0][j] == 2) {
				score++;
				if (j == FrameNum - 1) { left = true; score--; } //�������܂ōs�����獶�ӂ͂��ׂĎ���Ă���Ɛݒ�A���������̐΂����炷
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][0] == 2) { //�E����Ɏ����̐΂�����Ȃ�
		for (int i = 6; i >= 0; i--) { //�����̐΂��������тɉ��_
			if (up == false && StoneMap[i][0] == 2) score++; //��ӂ����ׂĎ���Ă���Ȃ���_���Ȃ�
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) { //�����̐΂��������тɉ��_
			if (StoneMap[FrameNum - 1][j] == 2) {
				score++;
				if (j == FrameNum - 1) { right = true; score--; } //�E�����܂ōs������E�ӂ͂��ׂĎ���Ă���Ɛݒ�A�E�������̐΂����炷
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][FrameNum - 1] == 2) { //�E�����Ɏ����̐΂�����Ȃ�
		for (int i = 6; i >= 0; i--) { //�����̐΂��������тɉ��_
			if (StoneMap[i][FrameNum - 1] == 2) {
				score++;
				if (i == 0) { down = true; score--; } //�������܂ōs�����牺�ӂ͂��ׂĎ���Ă���Ɛݒ�A���������̐΂����炷
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) { //�����̐΂��������тɉ��_
			if (right == false && StoneMap[FrameNum - 1][j] == 2) score++; //�E�ӂ����ׂĎ���Ă���Ȃ���_���Ȃ�
			else break;
		}
	}
	if (StoneMap[0][FrameNum - 1] == 2) { //�����̐΂��������тɉ��_
		for (int i = 1; i <= FrameNum - 1; i++) {
			if (down == false && StoneMap[i][FrameNum - 1] == 2) score++; //���ӂ����ׂĎ���Ă���Ȃ���_���Ȃ�
			else break;
		}
		for (int j = 6; j >= 0; j--) { //�����̐΂��������тɉ��_
			if (left == false && StoneMap[0][j] == 2) score++; //���ӂ����ׂĎ���Ă���Ȃ���_���Ȃ�
			else break;
		}
	}

	up = 0, right = 0, left = 0, down = 0;
	//���ɑ���̐΂�����ꍇ
	if (StoneMap[0][0] == 1) { //������ɑ���̐΂�����Ȃ�
		for (int i = 1; i <= FrameNum - 1; i++) {
			if (StoneMap[i][0] == 1) { //����̐΂��������тɌ��_
				score = score - 3;
				if (i == FrameNum - 1) { up = true; score = score + 3; } //�E����܂ōs�������ӂ͂��ׂĎ���Ă���Ɛݒ�A�E������̐΂����炷
			}
			else if (StoneMap[i][0] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int a = 1; i + a <= FrameNum - 1; a++) {
					if (StoneMap[i + a][0] == 1) { i = FrameNum; break; }
					else if (StoneMap[i + a][0] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) {
			if (StoneMap[0][j] == 1) { //����̐΂��������тɌ��_
				score = score - 3;
				if (j == FrameNum - 1) { left = true; score = score + 3; } //�������܂ōs�����獶�ӂ͂��ׂĎ���Ă���Ɛݒ�A���������̐΂����炷
			}
			else if (StoneMap[j][0] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int b = 1; j + b <= FrameNum - 1; b++) {
					if (StoneMap[j + b][0] == 1) { b = FrameNum; break; }
					else if (StoneMap[j + b][0] == 0) score = score - (b + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][0] == 1) { //�E����ɑ���̐΂�����Ȃ�
		for (int i = 6; i >= 0; i--) { //����̐΂��������тɌ��_
			if (up == false && StoneMap[i][0] == 1) score = score - 3; //�E�ӂ����ׂĎ���Ă���Ȃ猸�_���Ȃ�
			else if (StoneMap[i][0] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int a = 1; i - a >= 0; a++) {
					if (StoneMap[i - a][0] == 1) { i = -1; break; }
					else if (StoneMap[i - a][0] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) {
			if (StoneMap[FrameNum - 1][j] == 1) {
				score = score - 3;
				if (j == FrameNum - 1) { right = true; score = score + 3; } //�E�����܂ōs������E�ӂ͂��ׂĎ���Ă���Ɛݒ�A�E�������̐΂����炷
			}
			else if (StoneMap[FrameNum - 1][j] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int a = 1; a + j <= FrameNum - 1; a++) {
					if (StoneMap[j + a][0] == 1) { a = FrameNum; break; }
					else if (StoneMap[j + a][0] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][FrameNum - 1] == 1) { //�E�����ɑ���̐΂�����Ȃ�
		for (int i = 6; i >= 0; i--) { //����̐΂��������тɌ��_
			if (StoneMap[i][FrameNum - 1] == 1) {
				score = score - 3;
				if (i == 0) { down = true; score = score + 3; } //�������܂ōs�����牺�ӂ͂��ׂĎ���Ă���Ɛݒ�A���������̐΂����炷
			}
			else if (StoneMap[i][FrameNum - 1] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int a = 1; i - a >= 0; a++) {
					if (StoneMap[i - a][FrameNum - 1] == 1) { i = -1; break; }
					else if (StoneMap[i - a][FrameNum - 1] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) { //����̐΂��������тɌ��_
			if (right == false && StoneMap[FrameNum - 1][j] == 1) score = score - 3; //�E�ӂ����ׂĎ���Ă���Ȃ猸�_���Ȃ�
			else if (StoneMap[FrameNum - 1][j] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int b = 1; j - b <= FrameNum - 1; b++) {
					if (StoneMap[FrameNum - 1][j - b] == 1) { j = -1; break; }
					else if (StoneMap[FrameNum - 1][j - b] == 0) score = score - (b + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[0][FrameNum - 1] == 1) { //�������ɑ���̐΂�����Ȃ�
		for (int i = 1; i <= FrameNum - 1; i++) { //����̐΂��������тɌ��_
			if (down == false && StoneMap[i][FrameNum - 1] == 1) score = score - 3; //���ӂ����ׂĎ���Ă���Ȃ猸�_���Ȃ�
			else if (StoneMap[i][FrameNum - 1] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int a = 1; i + a <= FrameNum - 1; a++) {
					if (StoneMap[i + a][FrameNum - 1] == 1) { i = FrameNum; break; }
					else if (StoneMap[i + a][FrameNum - 1] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) {
			if (left == false && StoneMap[0][j] == 1) score = score - 3; //���ӂ����ׂĎ���Ă���Ȃ猸�_���Ȃ�
			else if (StoneMap[0][j] == 2) { //�����̐΂��������炻�ׂ̗𒲂ׂĂ����A�󂫃}�X���������玩���̐΂������Ă��镪���_�A����̐΂��������猸�_����
				for (int b = 1; j - b >= 0; b++) {
					if (StoneMap[0][j - b] == 1) { j = -1; break; }
					else if (StoneMap[0][j - b] == 0) score = score - (b + 1) * 3;
				}
			}
			else break;
		}
	}

	float rscore = (float)score / 3.0;
	return rscore;
}

//�Q�[���������֐�
void GameInit() {
	for(int i = 0; i <= FrameNum - 1; i++) {
		for(int j = 0; j <= FrameNum - 1; j++) {
			StoneMap[i][j] = 0;
		}
	}
	StoneMap[(FrameNum / 2) - 1][(FrameNum / 2) - 1] = 1; //�����΂̔z�u
	StoneMap[(FrameNum / 2)][(FrameNum / 2)] = 1;
	StoneMap[(FrameNum / 2)][(FrameNum / 2) - 1] = 2;
	StoneMap[(FrameNum / 2) - 1][(FrameNum / 2)] = 2;
}

//��ʕ\���֐�
void Display(int player) {
	//1�s�ژg
	for(int j = 0; j <= FrameNum - 1; j++) {
		for(int i = 0; i <= FrameNum - 1; i++) {
			printf("|");
			printf("�P�P�P");
		}
		printf("|\n");
		//�g��
		for(int k = 0; k <= 1; k++) { //�g���̍s�̐�
			for(int i = 0; i <= FrameNum - 1; i++) { //1�s�̒��ɉ����g����郋�[�v
				if(k == 0) { //�g���̐΂�u���s�ɗ�����
					if(StoneMap[i][j] != 0) {
						SetStone(StoneMap[i][j]);
					}
					else printf("|      ");
				}
				else {
					printf("|  ");
					if (setable[i * 10 + j]) {
						if (player == 1) {
							setColor(COL_GREEN, COL_BACK_DEFAULT);
						}
						else if (player == 2) {
							setColor(COL_YELLOW, COL_BACK_DEFAULT);
						}
					}
					printf("%d%d  ", i + 1, j + 1);
					setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				}
			}
			printf("|\n");
		}
	}
	for(int i = 0;i <= FrameNum - 1;i++) {
		printf(" �P�P�P");
	}
	printf("\n");
}

//�΂�u���֐�
void SetStone(int choice) {
	if (choice == 1 && first == 1) {
		printf("|�@�Z�@");
	}
	else if (choice == 2 && first == 1) {
		printf("|�@���@");
	}
	else if (choice == 2 && first == 2) {
		printf("|�@�Z�@");
	}
	else if (choice == 1 && first == 2) {
		printf("|�@���@");
	}
}

//�Ֆʕۑ��֐�
void Save(int place) {
	if (errno_t errorcs = fopen_s(&fp, "board.dat", "rb+") == 0) {
		fseek(fp, sizeof(int) * (FrameNum * FrameNum) * place, SEEK_SET);
		fwrite(StoneMap, sizeof(int), FrameNum * FrameNum, fp);
		fclose(fp);
		}
}

//�ՖʑY�t�֐�
void Load(int place) {
	if (errno_t errorcl = fopen_s(&fp, "board.dat", "rb") == 0) {
		fseek(fp, sizeof(int) * (FrameNum * FrameNum) * place, SEEK_SET);
		fread(StoneMap, sizeof(int), FrameNum * FrameNum, fp);
		fclose(fp);
	}
}

//�������ʏo�͊֐�
void ResultOutput() {
	fopen_s(&rfp, "result.txt", "a");
	fprintf(rfp, "%s vs EnemyAI(%d vs %d) �ʎZ %d : %d : %d\n", PlayerName, StoneNum[0], StoneNum[1], result[0], result[1], result[2]);
	fclose(rfp);
}

//StoneConfirm�֐����Ő΂���������W��z��ɑ������֐�
void StoneSearch(int n, int x, int y, bool pre) {
	int *op = &StoneOption[0];
	*op = n;
	op = op + n;
	*op = 10 * x + y;
	if (pre == false) setable[*op] = 1;
}