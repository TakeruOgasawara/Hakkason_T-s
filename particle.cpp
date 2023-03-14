//======================================================================================
//
//パーティクルの処理[Player.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "main.h"
#include "particle.h"
#include "effect.h"

//*****************************
//マクロ定義
//*****************************
#define MAX_PARTICLE			(MAX_EFFECT)			//パーティクルの最大数

//*****************************
//プロトタイプ宣言
//*****************************
void ParticleType(int nCntParticle);

//*****************************
//グローバル宣言
//*****************************
Particle g_aParticle[MAX_PARTICLE];			//パーティクルの情報

//========================================================================
// パーティクルの初期化処理
//========================================================================
void InitParticle(void)
{
	//変数宣言
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//構造体の初期化
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量を初期化する
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する(Z値を使用)
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].type = PARTICLE_TYPE_NONE;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;				//使用していない状態にする
	}
}

//========================================================================
//パーティクルの終了処理
//========================================================================
void UninitParticle(void)
{

}

//========================================================================
// パーティクルの更新処理
//========================================================================
void UpdateParticle(void)
{
	
}

//========================================================================
// パーティクルタイプ0(火花)
//========================================================================
void ParticleType(int nCntParticle)
{
	//変数宣言
	float fRot;
	float fMove;

	for (int nCntApper = 0; nCntApper < 20; nCntApper++)
	{
		//位置の設定
		//g_aParticle[nCntParticle].pos;

		//角度の設定
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//移動量の設定
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.x = sinf(fRot) * fMove;

		//角度の設定
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//移動量の設定
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.y = cosf(fRot) * fMove;

		//角度の設定
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		//移動量の設定
		fMove = (float)(rand() % 30) / 10;

		g_aParticle[nCntParticle].move.z = sinf(fRot) * fMove;

		//色の設定
		g_aParticle[nCntParticle].col = D3DXCOLOR{ 1.0f, 0.6f, 0.3f, 1.0f };

		//半径の設定
		g_aParticle[nCntParticle].fRadius = 10.0f;
	
		//エフェクト設定
		SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].move,
			g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].nLife);
	}
}

//========================================================================
//パーティクルの描画処理
//========================================================================
void DrawParticle(void)
{

}

//========================================================================
//パーティクルの設定処理
//========================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PARTICLE_TYPE type)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//使用していない状態なら
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].bUse = true;

			switch (g_aParticle[nCntParticle].type)
			{//タイプによる分岐
			case PARTICLE_TYPE_HIT:
				g_aParticle[nCntParticle].nLife = 100;
				ParticleType(nCntParticle);
				break;
			}

			break;
		}
	}
}
