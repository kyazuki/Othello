#ifndef FUNCTION_H
#define FUNCTION_H

#define FrameNum 8	//�}�X�̐��B4�ȏ�̋����̂ݐݒ��(�]���֐��̓s����ύX�s��)

//�Έʒu�ɂ��]���l
int boardscore[88] = { 1000,-12,0,-1,-1,0,-12,1000,0,0,-12,-15,-3,-3,-3,-3,-15,-12,0,0,0,-3,0,-1,-1,0,-3,0,0,0,-1,-3,-1,-1,-1,-1,-3,-1,0,0,-1,-3,-1,-1,-1,-1,-3,-1,0,0,0,-3,0,-1,-1,0,-3,0,0,0,-12,-15,-3,-3,-3,-3,-15,-12,0,0,1000,-12,0,-1,-1,0,-12,1000 };
int setable[(FrameNum - 1) * 10 + FrameNum] = {}; //�w��̈ʒu�ɐ΂������邩�ǂ���
int changestone[(FrameNum * FrameNum) + 1]; //�Ԃ��ꂽ�΂̈ʒu

int again = false; //�Đ킷�邩�ǂ���
int result[3] = {}; //����
int first = 1; //���̃v���C���[
int dir_x[8] = {0,1,1,1,0,-1,-1,-1}; //�S�����T���p�̕����w��z��
int dir_y[8] = {-1,-1,0,1,1,1,0,-1}; //����
int StoneMap[FrameNum][FrameNum]; //�΂����邩�ǂ���
int StoneNum[2] = {}; //�ŏI�I�ɉ��̐΂��������
int Turn = 1; //�^�[����
char PlayerName[50] = { "Unknown" }; //�v���C���[��
FILE *fp, *rfp;

void WindowSizeSet(void); //�E�B���h�E�T�C�Y�ύX
void setColor(int, int); //�����E�w�i�F�ύX
void SetStone(int); //�΂�u��
void Display(int); //�Ֆʕ`��
void GameInit(void); //������
void player_in(void); //�v���C���[����
void player_ai(void); //AI����
int FlipTest(int, int, int, int); //�Ђ�����Ԃ��邩
int FlipFlop(int, int, int, int); //�Ђ�����Ԃ��֐�
int FlipConfirm(int, bool); //����1:player �v���C���[�����ɒu����ꏊ���L�邩�ǂ���
void StoneCount(void); //�΂̐��𐔂���
void MatchResult(void); //�������ʂ̕\��
void P1_first(void); //�v���C���[��U
void P2_first(void); //AI��U
void Save(int); //�Ֆʕۑ��֐�
void Load(int); //�ՖʑY�t�֐�
float AlphaBeta(int, int, float, float, int, int); //Alpha-Beta�@
float Evaluate(int, int); //�����]���֐�
float Evaluate1(void); //�]���֐��@(�Έʒu�]��)
float Evaluate2(int); //�]���֐��A(�J���x�]��)
float Evaluate3(int); //�]���֐��B(����\���]��)
float Evaluate4(void); //�]���֐��C(�m��Ε]��)
void ResultOutput(void); //�������ʏo�͊֐�

#endif FUNCTION_H