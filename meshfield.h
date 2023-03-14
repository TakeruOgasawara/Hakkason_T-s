//====================================================================
//
// フィールドの処理[field.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************
// マクロ定義
//*****************************************
#define MAX_MESHFIELD			(12)

//******************************************
//フィールド構造体の定義
//******************************************
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXMATRIX mtxWorldField;			//ワールドマトリックス
	int nTexType;						//テクスチャ―のタイプ
	bool bUse;							//使っているか
} MeshField;

//******************************************
//プロトタイプ宣言
//******************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexType);


#endif