//====================================================================
//
// フィールドの処理[field.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//フィールド構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXMATRIX mtxWorldField;			//ワールドマトリックス
	bool bUse;							//使っているか
} Field;

//プロトタイプ宣言
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
void SetField(D3DXVECTOR3 pos);

#endif