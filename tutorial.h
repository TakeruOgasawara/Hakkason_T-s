//======================================================================================
//
//�^�C�g����ʏ���[game.h]
//Author;���}���@�V
//
//======================================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

typedef enum
{
	STARTT_NONE = 0,
	STARTT_IN,
	STARTT_OUT,
	STARTT_MAX
} STARTT;

//�v���g�^�C�v�錾
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif