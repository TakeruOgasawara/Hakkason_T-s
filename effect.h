//======================================================================================
//
//エフェクトの処理[Player.h]
//Author;小笠原　彪
//
//======================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

//*****************************
//マクロ定義
//*****************************
#define MAX_EFFECT			(10000)			//エフェクトの最大数

//*****************************
//エフェクト構造体の定義
//*****************************
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 size;				//大きさ
	D3DXMATRIX mtxWorldBullet;	//ワールドマトリックス
	D3DXCOLOR col;					//色
	float fRadius;					//半径(大きさ)
	int nLife;						//寿命
	int nMaxLife;					//最大寿命
	int nType;						//種類
	bool bUse;						//使用しているかどうか
}Effect;

//*****************************
//プロトタイプ宣言
//*****************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);
void SetEffectLIfe(int nLife);
Effect *GetEffect(void);
int GetEffectNum(void);

#endif