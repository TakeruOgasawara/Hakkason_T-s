

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//ポーズメニューの構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXCOLOR col;					//色
	bool bPause;					//ポーズを使っているか
}Pause;

//プロトタイプ宣言
void InitPause();
void UninitPause();
void UpdatePause();
void DrawPause();

Pause *GetPause(void);

#endif