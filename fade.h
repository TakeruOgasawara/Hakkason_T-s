//======================================================================================
//
//フェードの処理[explosion.h]
//Author;小笠原　彪
//
//======================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//フェードの状態
typedef enum
{
	FADE_NONE = 0,			//何もしていない状態
	FADE_IN,				//フェードイン状態
	FADE_OUT,				//フェードアウト状態
	FADE_MAX
}Fade;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);

Fade GetFade(void);

#endif