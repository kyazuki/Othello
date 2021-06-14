#ifndef FUNCTION_H
#define FUNCTION_H

#define FrameNum 8	//マスの数。4以上の偶数のみ設定可(評価関数の都合上変更不可)

//石位置による評価値
int boardscore[88] = { 1000,-12,0,-1,-1,0,-12,1000,0,0,-12,-15,-3,-3,-3,-3,-15,-12,0,0,0,-3,0,-1,-1,0,-3,0,0,0,-1,-3,-1,-1,-1,-1,-3,-1,0,0,-1,-3,-1,-1,-1,-1,-3,-1,0,0,0,-3,0,-1,-1,0,-3,0,0,0,-12,-15,-3,-3,-3,-3,-15,-12,0,0,1000,-12,0,-1,-1,0,-12,1000 };
int setable[(FrameNum - 1) * 10 + FrameNum] = {}; //指定の位置に石がおけるかどうか
int changestone[(FrameNum * FrameNum) + 1]; //返された石の位置

int again = false; //再戦するかどうか
int result[3] = {}; //結果
int first = 1; //先手のプレイヤー
int dir_x[8] = {0,1,1,1,0,-1,-1,-1}; //全方向探索用の方向指定配列
int dir_y[8] = {-1,-1,0,1,1,1,0,-1}; //同上
int StoneMap[FrameNum][FrameNum]; //石があるかどうか
int StoneNum[2] = {}; //最終的に何個の石を取ったか
int Turn = 1; //ターン数
char PlayerName[50] = { "Unknown" }; //プレイヤー名
FILE *fp, *rfp;

void WindowSizeSet(void); //ウィンドウサイズ変更
void setColor(int, int); //文字・背景色変更
void SetStone(int); //石を置く
void Display(int); //盤面描画
void GameInit(void); //初期化
void player_in(void); //プレイヤー操作
void player_ai(void); //AI操作
int FlipTest(int, int, int, int); //ひっくり返せるか
int FlipFlop(int, int, int, int); //ひっくり返す関数
int FlipConfirm(int, bool); //引数1:player プレイヤーが次に置ける場所が有るかどうか
void StoneCount(void); //石の数を数える
void MatchResult(void); //試合結果の表示
void P1_first(void); //プレイヤー先攻
void P2_first(void); //AI先攻
void Save(int); //盤面保存関数
void Load(int); //盤面塑逆関数
float AlphaBeta(int, int, float, float, int, int); //Alpha-Beta法
float Evaluate(int, int); //総合評価関数
float Evaluate1(void); //評価関数①(石位置評価)
float Evaluate2(int); //評価関数②(開放度評価)
float Evaluate3(int); //評価関数③(着手可能数評価)
float Evaluate4(void); //評価関数④(確定石評価)
void ResultOutput(void); //試合結果出力関数

#endif FUNCTION_H