//==================
//エフェクト処理
//==================
#ifndef _Effect_H_
#define _Effect_H_
#include "main.h"

//プロトタイプ宣言
void InitEffect();
void UninitEffect();
void UpdateEffect();
void DrawEffect();
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,D3DXVECTOR3 move, float fRadius, int nLife,int nType);

#endif // !_Effect_H_
