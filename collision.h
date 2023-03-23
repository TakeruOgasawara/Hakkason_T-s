//======================================================================================
//
// 当たり判定処理[collision.h]
// Author;小笠原　彪
//
//======================================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//プロトタイプ宣言
bool ClsCircleXY(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2);
bool ClsCircleXZ(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2);
bool ClsCircleXZAllies(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2, int MaxCnt);
bool ClsAdultWall(D3DXVECTOR3 *pPos, float fWidth);
//bool ClsCrossProduct(D3DXVECTOR3 *pPos, int nNum);

#endif
