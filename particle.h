//========================================================================================
//
// パーティクルヘッダー[particle.h]
// Author: 丹野 竜之介
//
//========================================================================================
#ifndef _Particle_H_
#define _Particle_H_
#include "main.h"

//パーティクルのタイプ
typedef enum
{
	PARTICLE_TYPE_KEY = 0,
	PARTICLE_TYPE_MAX,
}PARTICLE_TYPE;

//プロトタイプ宣言
void InitParticle();
void UninitParticle();
void UpdateParticle();
void DrawParticle();
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 rot, float fRad, float fMove, int nLife, PARTICLE_TYPE type, int nEffect, int nEffectLife, int nRand, int nMaxSpeed);
void KeyParticle(void);

#endif // !_Particle_H_