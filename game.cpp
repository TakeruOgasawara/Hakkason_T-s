//===========================================================================================
//
// ゲーム中の動作[game.cpp]
// Author:小笠原　彪
//
//===========================================================================================
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "meshfield.h"
#include "field.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "Enemy.h"
#include "fog.h"
#include "object.h"
#include "dome.h"

//*****************************
// マクロ定義
//*****************************
#define GOAL_DIST	(150000.0f)	//ゴールまでの距離

//*****************************
// プロトタイプ宣言
//*****************************

//*****************************
//グローバル宣言
//*****************************
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

	//ライトの初期化処理
	InitLight();

	//カメラの初期化処理
	InitCamera();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//ポーズの初期化処理
	InitPause();

	//メッシュフィールドの初期化処理
	InitMeshField();
	InitField();

	//タイムの初期化
	InitTime();

	//プレイヤーの初期化
	InitPlayer();

	InitEnemy();
	//フォグの初期化
	InitFog();

	//フォグの設定
	SetFog(D3DFOG_LINEAR, D3DXCOLOR(0.7f, 0.7f, 0.8f, 1.0f), 100.0f, 7000.0f, 0.1f);

	InitObject();

	InitDome();

	//通常状態へ
	g_gameState = GAMESTATE_NORMAL;

	//状態管理のカウンターを初期化
	g_nCounterGameState = 0;

	//ポーズ情報の取得
	Pause *pPause = GetPause();

	//ポーズ状態のON/OFF
	pPause->bPause = false;

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);
}
//========================================================================
//ゲーム画面の終了処理
//========================================================================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//プレイヤー終了処理
	UninitPlayer();

	UninitEnemy();
	//エフェクトの終了処理
	UninitEffect();

	//タイムの終了
	UninitTime();

	//パーティクルの終了処理
	UninitParticle();

	//ポーズの終了処理
	UninitPause();

	UninitDome();
	//フォグの終了処理
	UninitFog();

	//オブジェクトの終了処理
	UninitObject();

	//メッシュフィールドの終了処理
	UninitMeshField();
	UninitField();
}

//========================================================================
//ゲーム画面の更新処理
//========================================================================
void UpdateGame(void)
{
	//情報の取得
	Pause *pPause = GetPause();			//ポーズ
	Fade pFade = GetFade();				//フェード
	Player *pPlayer = GetPlayer();

	if (pFade == FADE_NONE)
	{//フェード状態じゃない場合

		UpdateDome();

		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true)
		{//キーが押された場合
			//ポーズのON/OFF
			pPause->bPause = pPause->bPause ? false : true;
		}
		if (GetKeyboardTrigger(DIK_P) == true || GetJoyPadTrigger(BUTTON_START, 0) == true && pPause->bPause == false)
		{//ポーズ処理
			//ポーズの初期化
			InitPause();
		}

		//ポーズが使われているとき
		if (pPause->bPause == true)
		{
			//ポーズの更新処理
			UpdatePause();
		}
	}

	if (pPause->bPause == false)
	{
		//プレイヤーの更新
		UpdatePlayer();

		//タイムの更新
		UpdateTime();

		UpdateEnemy();

		UpdateEffect();

		UpdateParticle();

		UpdateCamera();

		UpdateFog();
	}

	

	//if (g_gameState == GAMESTATE_END)
	//{//条件がそろう時に行える
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL:			//通常状態

			if (pPlayer->pos.z > GOAL_DIST)
			{//ゴール判定
				g_gameState = GAMESTATE_END;
			}

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

	//カメラのセット
	SetCamera(0);

	DrawDome();
	//メッシュフィールドの描画処理
	DrawMeshField();
	DrawField();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	//プレイヤーの描画
	DrawPlayer();

	DrawEnemy();

	//オブジェクトの描画
	DrawObject();

	//タイムの描画
	DrawTime();

	//フォグの描画
	DrawFog();
	
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
