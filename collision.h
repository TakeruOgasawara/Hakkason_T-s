//======================================================================================
//
// �����蔻�菈��[collision.h]
// Author;���}���@�V
//
//======================================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//�v���g�^�C�v�錾
bool ClsCircleXY(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2);
bool ClsCircleXZ(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2);
bool ClsCircleXZAllies(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2, int MaxCnt);
bool ClsAdultWall(D3DXVECTOR3 *pPos, float fWidth);
//bool ClsCrossProduct(D3DXVECTOR3 *pPos, int nNum);

#endif
