//========================================================================================
//
// パーティクルの処理[particle.cpp]
// Author: 丹野 竜之介
//
//========================================================================================
#include "particle.h"
#include "effect.h"

#define MAX_PARTICLE (128)

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move; //移動量
	D3DXVECTOR3 movePos; //出現位置の移動量
	D3DXVECTOR3 rot; //角度
	D3DXCOLOR col; //色
	float fMove; //移動量
	float fRadius; //半径
	int nLife; //寿命
	int nEffect; //エフェクトの同時呼び出し数
	int nEffectLife; //エフェクトの寿命
	int nRand; //拡散度合い
	int nMaxSpeed; //最高速度
	bool bUse; //使用しているか
	PARTICLE_TYPE  type;  //パーティクルの種類
}Particle;

Particle g_aParticle[MAX_PARTICLE];

void InitParticle()
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].bUse = false;

		g_aParticle[nCntParticle].type = PARTICLE_TYPE_KEY;
	}
}
void UninitParticle()
{

}
void UpdateParticle()
{
	float fRot;//角度
	float fRot2;//角度
	float fMove;//移動量
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nEffect; nCntAppear++)
			{
				g_aParticle[nCntParticle].nLife--;
				fRot = (float)(rand() % g_aParticle[nCntParticle].nRand - g_aParticle[nCntParticle].nRand / 2) / 100.0f;
				fRot2 = (float)(rand() % g_aParticle[nCntParticle].nRand - g_aParticle[nCntParticle].nRand / 2) / 100.0f;
				fMove = (float)(rand() % g_aParticle[nCntParticle].nMaxSpeed) / 2;
				g_aParticle[nCntParticle].move.x = cosf(fRot + g_aParticle[nCntParticle].rot.x) * sinf(fRot2 + g_aParticle[nCntParticle].rot.y) * fMove;
				g_aParticle[nCntParticle].move.y = cosf(fRot + g_aParticle[nCntParticle].rot.x) * cosf(fRot2 + g_aParticle[nCntParticle].rot.y) *fMove;
				g_aParticle[nCntParticle].move.z = sinf(fRot + g_aParticle[nCntParticle].rot.x) *fMove;

				g_aParticle[nCntParticle].move += g_aParticle[nCntParticle].movePos / 2;

				SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].move, g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].nEffectLife, g_aParticle[nCntParticle].type);

				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					g_aParticle[nCntParticle].bUse = false;
				}

				else
				{
					g_aParticle[nCntParticle].col.a -= 0.1f;
					g_aParticle[nCntParticle].fRadius -= 0.1f;
				}
			}
			switch (g_aParticle[nCntParticle].type)
			{
			case PARTICLE_TYPE_KEY:

				KeyParticle();

				break;
			}
		}
	}
}
void DrawParticle()
{

}
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fRad, float fMove, int nLife, PARTICLE_TYPE type, int nEffect, int nEffectLife, int nRand, int nMaxSpeed)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].movePos = move;
			g_aParticle[nCntParticle].rot = rot;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].fRadius = fRad;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].fMove = fMove;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;
			g_aParticle[nCntParticle].nEffect = nEffect;
			g_aParticle[nCntParticle].nEffectLife = nEffectLife;
			g_aParticle[nCntParticle].nRand = nRand;
			g_aParticle[nCntParticle].nMaxSpeed = nMaxSpeed;
			break;
		}
	}
}

//==============================
//鍵のパーティクル
//==============================
void KeyParticle(void)
{
	//鍵のパーティクルの色
	D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f);
}
