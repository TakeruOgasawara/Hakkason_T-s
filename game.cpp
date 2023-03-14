//===========================================================================================
//
//[game.cpp]
//Author:小笠原　彪
//
//===========================================================================================
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "particle.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"

//*****************************
// プロトタイプ宣言
//*****************************
//void SlowUpdate(void);

//*****************************
//グローバル宣言
//*****************************
LPDIRECT3D9 g_pDGame = NULL;						//Directx3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDeviceGame = NULL;			//Directx3Dへのデバイスへのポインタ
LPD3DXFONT g_pFontGame = NULL;						//フォントへのポインタ
GAMESTATE g_gameState = GAMESTATE_NONE;				//ゲームの状態
int g_nCounterGameState = 0;						//状態管理のカウンター
bool g_bPause = false;								//ポーズ状態のON/OFF

//========================================================================
//ゲーム画面の初期化処理
//========================================================================
void InitGame(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//カメラの初期化処理
	//InitCamera();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//ポーズの初期化処理
	InitPause();

	//通常状態へ
	g_gameState = GAMESTATE_NORMAL;

	//状態管理のカウンターを初期化
	g_nCounterGameState = 0;

	//ポーズ情報の取得
	Pause *pPause = GetPause();

	//ポーズ状態のON/OFF
	pPause->bPause = false;

	//サウンドの再生
	//PlaySound(SOUND_LABEL_BGM000);

	//デバック表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFontGame);
}
//========================================================================
//ゲーム画面の終了処理
//========================================================================
void UninitGame(void)
{
	//サウンドの停止
	//StopSound();

	//エフェクトの終了処理
	UninitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//ポーズの終了処理
	UninitPause();

	//デバック表示用フォントの破棄
	if (g_pFontGame != NULL)
	{
		g_pFontGame->Release();
		g_pFontGame = NULL;
	}
}

//========================================================================
//ゲーム画面の更新処理
//========================================================================
void UpdateGame(void)
{
	//ポーズ情報の取得
	Pause *pPause = GetPause();
	Fade pFade = GetFade();

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true)
		{//ポーズ処理
			pPause->bPause = pPause->bPause ? false : true;
		}

		if (pPause->bPause == true)
		{
			UpdatePause();
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoyPadTrigger(BUTTON_A, 0) == true)
		{//決定キー(ENTERキー)が押された
		 //モードの設定(ゲーム画面に移行)
			SetFade(MODE_RESULT);
		}
	}

	//if (g_gameState == GAMESTATE_END)
	//{//条件がそろう時に行える
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:			//通常状態
			break;

		case GAMESTATE_END:				//終了状態

			//状態管理のカウンターを引く
			g_nCounterGameState--;

			if (g_nCounterGameState <= 0)
			{//0になった場合
				g_gameState = GAMESTATE_NONE;				//何もしていない状態に設定

				//フェード設定(ステージセレクト画面に移行)
				SetFade(MODE_RESULT);

				////ランキングの設定
				//SetRanking(GetScore());
			}
			break;
		}
	//}
}

//========================================================================
//ゲーム画面の描画処理
//========================================================================
void DrawGame(void)
{
	//ポーズ情報の取得
	Pause *pPause = GetPause();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();
	
	if (pPause->bPause == true)
	{//ポーズ中だった場合
		//ポーズの描画処理
		DrawPause();
	}
}

//========================================================================
//ゲーム状態の設定
//========================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

//========================================================================
//ゲームの状態の取得
//========================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
