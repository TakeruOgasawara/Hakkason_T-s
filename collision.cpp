//======================================================================================
//
// “–‚½‚è”»’è‚Ìˆ—[Enemy.cpp]
// Author;¬Š}Œ´@•V
//
//======================================================================================
#include "collision.h"

//========================================================================
// ‰~‚Ì“–‚½‚è”»’è(XŽ² YŽ²)
//========================================================================
bool ClsCircleXY(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;
	bool bUse = false;

	//“G‚ÆƒvƒŒƒCƒ„[ŠÔ‚Ì‘ÎŠpü‚Ì’·‚³‚ðŽZo‚·‚é
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->y - pPos2->y) * (pPos1->y - pPos2->y));
	
	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//Žg—p‚·‚é
	}

	return bUse;
}

//========================================================================
// ‰~‚Ì“–‚½‚è”»’è xŽ² zŽ²
//========================================================================
bool ClsCircleXZ(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;				//”¼Œa‚ÌŒvŽZŒ‹‰Ê‚Ì•Û‘¶—p
	bool bUse = false;			//false‚É‰Šú‰»(”»’è—p)

	//“G‚ÆƒvƒŒƒCƒ„[ŠÔ‚Ì‘ÎŠpü‚Ì’·‚³‚ðŽZo‚·‚é
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//Žg—p‚·‚é
	}

	return bUse;
}

//========================================================================
// ‰~‚Ì“–‚½‚è”»’è xŽ² zŽ² (“¯—Þ‚Æ‚Ì“–‚½‚è)
//========================================================================
bool ClsCircleXZAllies(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2, int MaxCnt)
{
	float fLength;				//”¼Œa‚ÌŒvŽZŒ‹‰Ê‚Ì•Û‘¶—p
	bool bUse = false;			//false‚É‰Šú‰»(”»’è—p)

	//“¯—Þ‚Æ‚Ì‘ÎŠpü‚Ì’·‚³‚ðŽZo‚·‚é
	for (int nCnt = 0; nCnt < MaxCnt; nCnt++, pPos2++)
	{//poa1‚ÌƒJƒEƒ“ƒg‚ði‚ß‚é
		fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
						(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));
	}

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//Žg—p‚·‚é
	}

	return bUse;
}

//========================================================================
// ‘ål‚Ì•Çˆ—
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
