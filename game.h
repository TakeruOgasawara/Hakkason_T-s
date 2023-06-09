//======================================================================================
//
//ゲーム画面処理[game.h]
//Author;小笠原　彪
//
//======================================================================================
#ifndef _GAME_H_				//背景
#define _GAME_H_				//背景

#include "main.h"

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,			//何もしてない状態
	GAMESTATE_NORMAL,			//通常状態
	GAMESTATE_END,				//終了状態
} GAMESTATE;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);

#endif