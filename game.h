//======================================================================================
//
//�Q�[����ʏ���[game.h]
//Author;���}���@�V
//
//======================================================================================
#ifndef _GAME_H_				//�w�i
#define _GAME_H_				//�w�i

#include "main.h"

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,			//�������ĂȂ����
	GAMESTATE_NORMAL,			//�ʏ���
	GAMESTATE_END,				//�I�����
} GAMESTATE;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);

#endif