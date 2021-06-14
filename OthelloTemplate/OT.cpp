//雛形FlipConfirm関数にミスあり if条件はdir <= 8ではなくdir <= 7

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

//テスト用
#include <random>
using namespace std;
random_device rd;
mt19937 mt(rd());
void player_ran(int);

//オプション
bool arrange = true; //AIの思考演出
bool ranmode = false; //AIを乱数AIと対戦
bool gameloop = false; //強制再戦
#define MODE 1 //乱数AIの設定(完全ランダム:0, 隅を優先→辺を優先→ランダム:1)
#define OUTPUT true //試合結果をファイルに出力するかどうか(有効:true, 無効:false)
bool skip = true; //スキップを有効にするか

int main() {
	if (OUTPUT) { //result.txtに現在の日付時刻を出力(OUTPUTがtrue設定時のみ)
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
			printf("プレイヤー名を入力してください\n");
			setColor(COL_ME, COL_BACK_DEFAULT);
			scanf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		}
	printf("\n先攻を入力してください\n");
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
				printf("1か2で入力してください\n");
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			}
		}
	do {
		system("cls");
		GameInit();
		StoneCount();
		if (first == 1) {
			printf("Turn 1 ○");
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
			printf("Turn 1 〇");
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
		printf("\n試合終了\n");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		StoneCount();
		MatchResult();
		if (OUTPUT) ResultOutput();
		if (gameloop == false) {
			printf("\n再戦しますか？再戦する場合は先攻･後攻が入れ替わります\ny/n\n");
			char gameagain[2];
			int loop2 = 1;
			while (loop2) {
				scanf("%s", gameagain);
				if (strcmp(gameagain, "y") == 0) { again = true; loop2 = 0; }
				else if (strcmp(gameagain, "n") == 0) { again = false; loop2 = 0; }
				else {
					setColor(COL_CAUTION1, COL_BACK_DEFAULT);
					printf("\nyかnで入力してください\n");
					setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				}
			}
		}
		if (first == 1) first = 2;
		else first = 1;
	} while (gameloop || again);
}


//ウィンドウサイズ変更
void WindowSizeSet(){
	BOOL bRtn;
	HANDLE hWrite;
	SMALL_RECT rctWindowRect = {0,0,149,38};
	COORD dwCoord = {150,39};

	hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	bRtn = SetConsoleScreenBufferSize(hWrite, dwCoord);
	bRtn = SetConsoleWindowInfo(hWrite,TRUE,&rctWindowRect);
}

//文字色変更
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

//プレイヤー先攻
void P1_first() {
	int EndNum = 0;
	while(1){
		StoneCount();
		if(FlipConfirm(1, false) == 0) { //プレイヤーが置く場所が無かったら
			EndNum++;
			system("cls");
			printf("Turn %d ○", Turn);
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
			printf("置ける場所が無いのでパスします\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ○", Turn);
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
			printf("Turn %d ○", Turn);
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
		if(EndNum >= 2) { //2人とも石を置ける場所が無かった場合
			break;
		}
		if(FlipConfirm(2, false) == 0) { //AIが置く場所が無かったら
			EndNum++;
			system("cls");
			printf("Turn %d ●", Turn);
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
			printf("置ける場所が無いのでパスします\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ●", Turn);
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
			printf("Turn %d ●", Turn);
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
		if(EndNum >= 2) { //2人とも石を置ける場所が無かった場合
			break;
		}
	}
}

//AI先攻
void P2_first() {
	int EndNum = 0;
	while(1){
		StoneCount();
		if(FlipConfirm(2, false) == 0) { //AIが置く場所が無かったら
			EndNum++;
			system("cls");
			printf("Turn %d 〇", Turn);
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
			printf("置ける場所が無いのでパスします\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d 〇", Turn);
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
			printf("Turn %d 〇", Turn);
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
		if(EndNum >= 2) { //2人とも石を置ける場所が無かった場合
			break;
		}
		if(FlipConfirm(1, false) == 0) { //プレイヤーが置く場所が無かったら
			EndNum++;
			system("cls");
			printf("Turn %d ●", Turn);
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
			printf("置ける場所が無いのでパスします\n");
			Sleep(500);
			if (skip == false) { getchar(); getchar(); }
		}
		else {
			system("cls");
			printf("Turn %d ●", Turn);
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
			printf("Turn %d ●", Turn);
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
		if(EndNum >= 2) { //2人とも石を置ける場所が無かった場合
			break;
		}
	}
}

//試合結果の表示
void MatchResult(){
	if(StoneNum[0] > StoneNum[1]){
		result[0]++;
		if (first == 1)	printf("〇");
		else printf("●");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("%s", PlayerName);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("の勝利\n\nこれまでの戦績\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("プレイヤー");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, 引き分け: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
	else if(StoneNum[0] < StoneNum[1]){
		result[1]++;
		if (first == 1)	printf("●");
		else printf("〇");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("の勝利\n\nこれまでの戦績\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("プレイヤー");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, 引き分け: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
	else{
		result[2]++;
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("引き分け\n");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("\nこれまでの戦績\n");
		setColor(COL_ME, COL_BACK_DEFAULT);
		printf("プレイヤー");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_GREEN, COL_BACK_DEFAULT);
		printf("%d", result[0]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, ");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf(": ");
		setColor(COL_YELLOW, COL_BACK_DEFAULT);
		printf("%d", result[1]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("勝, 引き分け: ");
		setColor(COL_BLUE, COL_BACK_DEFAULT);
		printf("%d\n", result[2]);
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	}
}

//石を何個取ったか数える
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

//playerが石を置ける場所があるか調べる
int FlipConfirm(int player, bool pre){
	int Confirm = 0;
	for(int x = 0; x <= FrameNum - 1 ; x++){
		for(int y = 0 ; y <= FrameNum - 1 ; y++){
			for(int dir = 0 ; dir <= 7 ; dir++){
				if (FlipTest(dir, player, x, y)) {	//全部のマスを探索して挟める石があるか調べる
					Confirm++;
					StoneSearch(Confirm, x, y, pre);
				}
			}
		}
	}
	return Confirm;
}

//実際にひっくり返す関数
int FlipFlop(int dir, int player, int x, int y) {
	if (dir == 0) { for (int i = 0; i <= (FrameNum * FrameNum); i++) changestone[i] = 0; }
	int EnemyStone = 0;	//
	int Enemy;
	int Enemy_x[FrameNum];	//挟める敵の石位置を保存する配列
	int Enemy_y[FrameNum];
	for(int i = 0; i <= FrameNum - 1 ;i++) { //初期化
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
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0) { //探索範囲が枠の外になってしまったら
			//printf("x=%d,y=%d枠外です\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player) { //もし探索方向に自分の石が有ったら
			if(EnemyStone != 0) { //挟める石が有ったら
				changestone[0] = EnemyStone;
				EnemyStone--;
				for (int i = 1; EnemyStone >= 0; i++) {
					StoneMap[Enemy_x[EnemyStone]][Enemy_y[EnemyStone]] = player;
					//printf("x=%d y=%dをひっくり返します\n",Enemy_x[EnemyStone],Enemy_y[EnemyStone]);
					changestone[i] = (Enemy_x[EnemyStone]) * 10 + Enemy_y[EnemyStone];
					EnemyStone--;
				}
				return 1;
			}
			else {
				//printf("x=%d,y=%d挟める石がありません\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy) { //もし探索方向に敵の石が有ったら
			//printf("x=%d,y=%d敵の石有り\n",x,y);
			Enemy_x[EnemyStone] = x;
			Enemy_y[EnemyStone] = y;
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0) { //もし探索方向が開きだったら
			//printf("x=%d,y=%d探索方向が開きです\n",x,y);
			return 0;
		}
	}
}

//ひっくり返せるかテストする関数
int FlipTest(int dir,int player,int x,int y) { //dir:探索方向の指定, player:どのプレイヤーの操作か x,y:自分の今の位置
	int EnemyStone = 0;	//
	int Enemy;
	if(player == 1) {
		Enemy = 2;
	}
	else {
		Enemy = 1;
	}
	//printf("初期位置x=%d,y=%d\n",x,y);
	if(StoneMap[x][y] != 0) { //既になんか石が有って置けない場合
		return 0;
	}
	while(1) {
		x = x + dir_x[dir];
		y = y + dir_y[dir];
		if(x >= FrameNum || x < 0 || y >= FrameNum || y < 0) { //探索範囲が枠の外になってしまったら
			//printf("x=%d,y=%d枠外です\n",x,y);
			return 0;
		}
		if(StoneMap[x][y] == player) { //もし探索方向に自分の石が有ったら
			if(EnemyStone != 0) { //挟める石が有ったら
				//printf("挟める石有り\n");
				return 1;
			}
			else {
				//printf("x=%d,y=%d挟める石がありません\n",x,y);
				return 0;
			}
		}
		else if(StoneMap[x][y] == Enemy) { //もし探索方向に敵の石が有ったら
			//printf("x=%d,y=%d敵の石有り\n",x,y);
			EnemyStone++;
		}
		else if(StoneMap[x][y] == 0) { //もし探索方向が開きだったら
			//printf("x=%d,y=%d探索方向が開きです\n",x,y);
			return 0;
		}
	}
}

//プレイヤーからの入力を受け付ける
void player_in() {
	char postxt[100];
	int pos,x,y;
	int return_flag = 1;
	if (first == 1)	printf("〇");
	else printf("●");
	setColor(COL_ME, COL_BACK_DEFAULT);
	printf("%s", PlayerName);
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("のターン\n\n");
	do {
		int Loop = 1;
		getchar();
		while (Loop) {
			printf("位置番号=");
			scanf("%[0-9]", postxt);
			pos = atoi(postxt);
			if (pos >= 11 && pos <= FrameNum * 10 + FrameNum && pos % 10 >= 1 && pos % 10 <= 8) {
					Loop = 0;
			}
			else {
				getchar();
				Loop = 1;
				system("cls");
				if (first == 1)	printf("Turn %d ○", Turn);
				else printf("Turn %d ●", Turn);
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
				if (first == 1) printf("〇");
				else printf("●");
				setColor(COL_ME, COL_BACK_DEFAULT);
				printf("%s", PlayerName);
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
				printf("のターン\n");
				setColor(COL_CAUTION1, COL_BACK_DEFAULT);
				printf("値が不適切です。入力しなおしてください\n");
				setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			}
			strcpy(postxt, "Init");
		}
		y = pos % 10 - 1;
		x = (pos - y) / 10 - 1;
		int FlipResult = 0;
		for(int i = 0; i <= 7 ; i++) { //全方向探索で置けるかどうか判定
			if(FlipTest(i,1,x,y)) {
				FlipResult++;
			}
		}
		if(pos <= FrameNum * 10 + FrameNum && StoneMap[x][y] == 0 && FlipResult != 0) { //その場所に石が無い、且つ相手をひっくり返せるなら
			return_flag = 0;
		}
		else {
			system("cls");
			if (first == 1) printf("Turn %d ○", Turn);
			else printf("Turn %d ●", Turn);
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
			if (first == 1) printf("〇");
			else printf("●");
			setColor(COL_ME, COL_BACK_DEFAULT);
			printf("%s", PlayerName);
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
			printf("のターン\n");
			setColor(COL_CAUTION2, COL_BACK_DEFAULT);
			printf("その位置には置けません。もう一度入力してください\n");
			setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		}
	} while(return_flag != 0);
	StoneMap[x][y] = 1;
	for(int dir = 0; dir <= 7 ; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//全方位探索をして実際にひっくり返す関数
	}
}

//AI処理
void player_ai() {
	int pos, x, y;
	int return_flag = 1;
	if (first == 1) printf("●");
	else printf("〇");
	setColor(COL_ENEMY, COL_BACK_DEFAULT);
	printf("EnemyAI");
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("のターン\n\n");
	printf("位置番号=");
	if (arrange == true) printf("\n");
	
	if (StoneOption[0] > 1) { //置ける場所が一ヶ所でないなら
		if (arrange == true) {
			setColor(COL_ENEMY, COL_BACK_DEFAULT);
			printf("思考中");
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
	else if (StoneOption[0] == 1) pos = StoneOption[1]; //置ける場所が一ヶ所ならそこに置く

	y = pos % 10;
	x = (pos - y) / 10;

	if (arrange == true) {
		system("cls");
		if (first == 1)	printf("Turn %d ●", Turn);
		else printf("Turn %d 〇", Turn);
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
		if (first == 1) printf("●");
		else printf("〇");
		setColor(COL_ENEMY, COL_BACK_DEFAULT);
		printf("EnemyAI");
		setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
		printf("のターン\n\n");
		printf("位置番号=%d", pos + 11);
		Sleep(1500);
	}
	else printf("%d", pos + 11);

	StoneMap[x][y] = 2;
	for (int dir = 0; dir <= 7; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//全方位探索をして実際にひっくり返す関数
	}
}

//ランダム着手AIプレイヤー(テスト用)
void player_ran(int mode) {
	int pos, x, y;
	int return_flag = 1;
	if (first == 1) printf("〇");
	else printf("●");
	setColor(COL_ME, COL_BACK_DEFAULT);
	printf("%s", PlayerName);
	setColor(COL_FRONT_DEFAULT, COL_BACK_DEFAULT);
	printf("のターン\n\n");
	printf("位置番号=");

	if (StoneOption[0] > 1) { //置ける場所が一ヶ所でないなら
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
	else if (StoneOption[0] == 1) pos = StoneOption[1]; //置ける場所が一ヶ所ならそこに置く

	y = pos % 10;
	x = (pos - y) / 10;

	printf("%d\n", pos + 11);

	StoneMap[x][y] = 1;
	for (int dir = 0; dir <= 7; dir++) {
		FlipFlop(dir, StoneMap[x][y], x, y);	//全方位探索をして実際にひっくり返す関数
	}
}

//Alpha-Beta法
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

//総合評価関数
float Evaluate(int depth, int player) {
	float W1 = 0.4, W2 = 0.6, W3 = 0.7, W4 = 0.8; //各評価関数の重み
	float score = W1 * Evaluate1() + W2 * Evaluate2(depth) + W3 * Evaluate3(player) + W4 * Evaluate4();
	return score;
}

//評価関数①(石位置評価※-15～30(1000))
float Evaluate1() {
	int score = 0;
	for (int i = 0; i < FrameNum; i++) {
		for (int j = 0; j < FrameNum; j++) {
			if (StoneMap[i][j] == 2) score = score + boardscore[i * 10 + j]; //自分の石なら点数表より加点
			else if (StoneMap[i][j] == 1) score = score - boardscore[i * 10 + j]; //敵の石なら点数表より減点
		}
	}
	float rscore = (float)score;
	return rscore;
}

//評価関数②(開放度評価※-30～0くらい？)
float Evaluate2(int depth) {
	int score = 0, x = 0, y = 0;
	Save(Prediction + 1);
	Load(depth + 1);
	for (int i = 1; i <= changestone[0]; i++) { //返した石が、返す前の状態で周囲に空きマスがあるならその分減点
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

//評価関数③(敵着手可能数評価※-10～0くらい？)
float Evaluate3(int player) {
	int score = 0;
	if (player == 1) score = score - StoneOption[0]; //敵局面なら着手可能数1つあたり減点
	float rscore = (float)score * 3.0;
	return rscore;
}

//評価関数④(4辺の確定石評価※-147～49)
float Evaluate4() {
	int score = 0, up = 0, right = 0, left = 0, down = 0;
	//四隅に石がないなら評価しない
	if (StoneMap[0][0] == 0 && StoneMap[FrameNum - 1][0] == 0 && StoneMap[FrameNum - 1][FrameNum - 1] == 0 && StoneMap[0][FrameNum - 1] == 0) return 0;
	
	//隅に自分の石がある場合
	if (StoneMap[0][0] == 2) { //左上隅に自分の石があるなら
		for (int i = 1; i <= FrameNum - 1; i++) { //自分の石が続くたびに加点
			if (StoneMap[i][0] == 2) {
				score++;
				if (i == FrameNum - 1) { up = true; score--; } //右上隅まで行ったら上辺はすべて取っていると設定、右上隅分の石を減らす
			}
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) { //自分の石が続くたびに加点
			if (StoneMap[0][j] == 2) {
				score++;
				if (j == FrameNum - 1) { left = true; score--; } //左下隅まで行ったら左辺はすべて取っていると設定、左下隅分の石を減らす
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][0] == 2) { //右上隅に自分の石があるなら
		for (int i = 6; i >= 0; i--) { //自分の石が続くたびに加点
			if (up == false && StoneMap[i][0] == 2) score++; //上辺をすべて取っているなら加点しない
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) { //自分の石が続くたびに加点
			if (StoneMap[FrameNum - 1][j] == 2) {
				score++;
				if (j == FrameNum - 1) { right = true; score--; } //右下隅まで行ったら右辺はすべて取っていると設定、右下隅分の石を減らす
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][FrameNum - 1] == 2) { //右下隅に自分の石があるなら
		for (int i = 6; i >= 0; i--) { //自分の石が続くたびに加点
			if (StoneMap[i][FrameNum - 1] == 2) {
				score++;
				if (i == 0) { down = true; score--; } //左下隅まで行ったら下辺はすべて取っていると設定、左下隅分の石を減らす
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) { //自分の石が続くたびに加点
			if (right == false && StoneMap[FrameNum - 1][j] == 2) score++; //右辺をすべて取っているなら加点しない
			else break;
		}
	}
	if (StoneMap[0][FrameNum - 1] == 2) { //自分の石が続くたびに加点
		for (int i = 1; i <= FrameNum - 1; i++) {
			if (down == false && StoneMap[i][FrameNum - 1] == 2) score++; //下辺をすべて取っているなら加点しない
			else break;
		}
		for (int j = 6; j >= 0; j--) { //自分の石が続くたびに加点
			if (left == false && StoneMap[0][j] == 2) score++; //左辺をすべて取っているなら加点しない
			else break;
		}
	}

	up = 0, right = 0, left = 0, down = 0;
	//隅に相手の石がある場合
	if (StoneMap[0][0] == 1) { //左上隅に相手の石があるなら
		for (int i = 1; i <= FrameNum - 1; i++) {
			if (StoneMap[i][0] == 1) { //相手の石が続くたびに減点
				score = score - 3;
				if (i == FrameNum - 1) { up = true; score = score + 3; } //右上隅まで行ったら上辺はすべて取られていると設定、右上隅分の石を減らす
			}
			else if (StoneMap[i][0] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int a = 1; i + a <= FrameNum - 1; a++) {
					if (StoneMap[i + a][0] == 1) { i = FrameNum; break; }
					else if (StoneMap[i + a][0] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 1; j <= FrameNum - 1; j++) {
			if (StoneMap[0][j] == 1) { //相手の石が続くたびに減点
				score = score - 3;
				if (j == FrameNum - 1) { left = true; score = score + 3; } //左下隅まで行ったら左辺はすべて取られていると設定、左下隅分の石を減らす
			}
			else if (StoneMap[j][0] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int b = 1; j + b <= FrameNum - 1; b++) {
					if (StoneMap[j + b][0] == 1) { b = FrameNum; break; }
					else if (StoneMap[j + b][0] == 0) score = score - (b + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][0] == 1) { //右上隅に相手の石があるなら
		for (int i = 6; i >= 0; i--) { //相手の石が続くたびに減点
			if (up == false && StoneMap[i][0] == 1) score = score - 3; //右辺をすべて取られているなら減点しない
			else if (StoneMap[i][0] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
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
				if (j == FrameNum - 1) { right = true; score = score + 3; } //右下隅まで行ったら右辺はすべて取られていると設定、右下隅分の石を減らす
			}
			else if (StoneMap[FrameNum - 1][j] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int a = 1; a + j <= FrameNum - 1; a++) {
					if (StoneMap[j + a][0] == 1) { a = FrameNum; break; }
					else if (StoneMap[j + a][0] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[FrameNum - 1][FrameNum - 1] == 1) { //右下隅に相手の石があるなら
		for (int i = 6; i >= 0; i--) { //相手の石が続くたびに減点
			if (StoneMap[i][FrameNum - 1] == 1) {
				score = score - 3;
				if (i == 0) { down = true; score = score + 3; } //左下隅まで行ったら下辺はすべて取られていると設定、左下隅分の石を減らす
			}
			else if (StoneMap[i][FrameNum - 1] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int a = 1; i - a >= 0; a++) {
					if (StoneMap[i - a][FrameNum - 1] == 1) { i = -1; break; }
					else if (StoneMap[i - a][FrameNum - 1] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) { //相手の石が続くたびに減点
			if (right == false && StoneMap[FrameNum - 1][j] == 1) score = score - 3; //右辺をすべて取られているなら減点しない
			else if (StoneMap[FrameNum - 1][j] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int b = 1; j - b <= FrameNum - 1; b++) {
					if (StoneMap[FrameNum - 1][j - b] == 1) { j = -1; break; }
					else if (StoneMap[FrameNum - 1][j - b] == 0) score = score - (b + 1) * 3;
				}
			}
			else break;
		}
	}
	if (StoneMap[0][FrameNum - 1] == 1) { //左下隅に相手の石があるなら
		for (int i = 1; i <= FrameNum - 1; i++) { //相手の石が続くたびに減点
			if (down == false && StoneMap[i][FrameNum - 1] == 1) score = score - 3; //下辺をすべて取られているなら減点しない
			else if (StoneMap[i][FrameNum - 1] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
				for (int a = 1; i + a <= FrameNum - 1; a++) {
					if (StoneMap[i + a][FrameNum - 1] == 1) { i = FrameNum; break; }
					else if (StoneMap[i + a][FrameNum - 1] == 0) score = score - (a + 1) * 3;
				}
			}
			else break;
		}
		for (int j = 6; j >= 0; j--) {
			if (left == false && StoneMap[0][j] == 1) score = score - 3; //左辺をすべて取られているなら減点しない
			else if (StoneMap[0][j] == 2) { //自分の石があったらその隣を調べていき、空きマスがあったら自分の石が続いている分減点、相手の石があったら減点無し
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

//ゲーム初期化関数
void GameInit() {
	for(int i = 0; i <= FrameNum - 1; i++) {
		for(int j = 0; j <= FrameNum - 1; j++) {
			StoneMap[i][j] = 0;
		}
	}
	StoneMap[(FrameNum / 2) - 1][(FrameNum / 2) - 1] = 1; //初期石の配置
	StoneMap[(FrameNum / 2)][(FrameNum / 2)] = 1;
	StoneMap[(FrameNum / 2)][(FrameNum / 2) - 1] = 2;
	StoneMap[(FrameNum / 2) - 1][(FrameNum / 2)] = 2;
}

//画面表示関数
void Display(int player) {
	//1行目枠
	for(int j = 0; j <= FrameNum - 1; j++) {
		for(int i = 0; i <= FrameNum - 1; i++) {
			printf("|");
			printf("￣￣￣");
		}
		printf("|\n");
		//枠内
		for(int k = 0; k <= 1; k++) { //枠内の行の数
			for(int i = 0; i <= FrameNum - 1; i++) { //1行の中に何個も枠を作るループ
				if(k == 0) { //枠内の石を置く行に来たら
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
		printf(" ￣￣￣");
	}
	printf("\n");
}

//石を置く関数
void SetStone(int choice) {
	if (choice == 1 && first == 1) {
		printf("|　〇　");
	}
	else if (choice == 2 && first == 1) {
		printf("|　●　");
	}
	else if (choice == 2 && first == 2) {
		printf("|　〇　");
	}
	else if (choice == 1 && first == 2) {
		printf("|　●　");
	}
}

//盤面保存関数
void Save(int place) {
	if (errno_t errorcs = fopen_s(&fp, "board.dat", "rb+") == 0) {
		fseek(fp, sizeof(int) * (FrameNum * FrameNum) * place, SEEK_SET);
		fwrite(StoneMap, sizeof(int), FrameNum * FrameNum, fp);
		fclose(fp);
		}
}

//盤面塑逆関数
void Load(int place) {
	if (errno_t errorcl = fopen_s(&fp, "board.dat", "rb") == 0) {
		fseek(fp, sizeof(int) * (FrameNum * FrameNum) * place, SEEK_SET);
		fread(StoneMap, sizeof(int), FrameNum * FrameNum, fp);
		fclose(fp);
	}
}

//試合結果出力関数
void ResultOutput() {
	fopen_s(&rfp, "result.txt", "a");
	fprintf(rfp, "%s vs EnemyAI(%d vs %d) 通算 %d : %d : %d\n", PlayerName, StoneNum[0], StoneNum[1], result[0], result[1], result[2]);
	fclose(rfp);
}

//StoneConfirm関数内で石をおける座標を配列に代入する関数
void StoneSearch(int n, int x, int y, bool pre) {
	int *op = &StoneOption[0];
	*op = n;
	op = op + n;
	*op = 10 * x + y;
	if (pre == false) setable[*op] = 1;
}