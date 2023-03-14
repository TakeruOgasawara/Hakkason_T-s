//======================================================================================
//
//タイトル画面処理[game.h]
//Author;小笠原　彪
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

//プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif