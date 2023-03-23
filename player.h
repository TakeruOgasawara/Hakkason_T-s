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
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//前回の位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 rotDest;		//目標の向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	PlayerPart part[MAX_PART];	//パーツ
}Player;

//*********************
// プロトタイプ宣言
//*********************
//基本処理
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

//情報取得
Player *GetPlayer(void);

//読み込み処理
void LoadMotion(FILE *pFile);

//操作処理
void ControlPlayerKeyboard(void);
void ControlPlayerPad(void);

//その他
void FactingRot(float *pfRot,float fRotDest);

#endif // !_PLAYER_H_