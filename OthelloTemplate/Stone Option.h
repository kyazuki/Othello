#ifndef STONEOPTION_H
#define STONEOPTION_H

#define Prediction 3 //�����܂œǂނ�

int StoneOption[(FrameNum * FrameNum) - 3]; //�΂�u����ʒu
int StoneOptionPre[(FrameNum * FrameNum) - 3][Prediction]; //�]���֐��p
void StoneSearch(int, int, int, bool); //�΂�u����ʒu�𒲂ׂ�

#endif STONEOPTION_H