#ifndef STONEOPTION_H
#define STONEOPTION_H

#define Prediction 3 //何手先まで読むか

int StoneOption[(FrameNum * FrameNum) - 3]; //石を置ける位置
int StoneOptionPre[(FrameNum * FrameNum) - 3][Prediction]; //評価関数用
void StoneSearch(int, int, int, bool); //石を置ける位置を調べる

#endif STONEOPTION_H