//==================
//パーティクル処理
//==================
#ifndef _Particle_H_
#define _Particle_H_
#include "main.h"

//プロトタイプ宣言
void InitParticle();
void UninitParticle();
void UpdateParticle();
void DrawParticle();
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,D3DXVECTOR3 rot, float fRad, float fMove, int nLife, int nType, int nEffect,int nEffectLife,int nRand,int nMaxSpeed);

#endif // !_Particle_H_