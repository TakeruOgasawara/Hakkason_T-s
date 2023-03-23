//===========================
//
//ダイレクトX.playerヘッダー
//Author:髙山桃也
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "model.h"
#include <stdio.h>

//*********************
// 列挙型定義
//*********************
typedef enum
{// プレイヤーの状態
	PLAYERSTATE_APPEAR = 0,			//出現状態
	PLAYERSTATE_NORMAL,				//通常状態
	PLAYERSTATE_MOVE,				//移動状態
	PLAYERSTATE_DAMAGE,				//ダメージ状態
	PLAYERSTATE_DEATH,				//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************
// 構造体定義
//*********************
// パーツの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nIdx;	//番号
	int nIdxParent;	//親の番号
}PlayerPart;

// プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	PlayerPart part[MAX_PART];	//パーツ
}Player;

//*********************
// プロトタイプ宣言
//*********************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void LoadMotion(FILE *pFile);

#endif // !_PLAYER_H_