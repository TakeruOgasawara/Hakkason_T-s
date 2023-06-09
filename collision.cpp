//======================================================================================
//
// 当たり判定の処理[Enemy.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "collision.h"

//========================================================================
// 円の当たり判定(X軸 Y軸)
//========================================================================
bool ClsCircleXY(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;
	bool bUse = false;

	//敵とプレイヤー間の対角線の長さを算出する
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->y - pPos2->y) * (pPos1->y - pPos2->y));
	
	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//使用する
	}

	return bUse;
}

//========================================================================
// 円の当たり判定 x軸 z軸
//========================================================================
bool ClsCircleXZ(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;				//半径の計算結果の保存用
	bool bUse = false;			//falseに初期化(判定用)

	//敵とプレイヤー間の対角線の長さを算出する
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//使用する
	}

	return bUse;
}

//========================================================================
// 円の当たり判定 x軸 z軸 (同類との当たり)
//========================================================================
bool ClsCircleXZAllies(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2, int MaxCnt)
{
	float fLength;				//半径の計算結果の保存用
	bool bUse = false;			//falseに初期化(判定用)

	//同類との対角線の長さを算出する
	for (int nCnt = 0; nCnt < MaxCnt; nCnt++, pPos2++)
	{//poa1のカウントを進める
		fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
						(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));
	}

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//使用する
	}

	return bUse;
}

//========================================================================
// 大人の壁処理
//========================================================================
bool ClsAdultWall(D3DXVECTOR3 *pPos, float fWidth)
{
	bool bAdultWall = false;

	if (pPos->x <= -170.0f + fWidth)
	{
		pPos->x = -170.0f + fWidth;
		bAdultWall = true;
	}
	if (pPos->x >= 170.0f - fWidth)
	{
		pPos->x = 170.0f - fWidth;
		bAdultWall = true;
	}
	/*if (pPos->z <= -700.0f + fWidth)
	{
		pPos->z = -700.0f + fWidth;
		bAdultWall = true;
	}
	if (pPos->z >= 700.0f - fWidth)
	{
		pPos->z = 700.0f - fWidth;
		bAdultWall = true;
	}*/

	return bAdultWall;
}
