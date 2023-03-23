//======================================================================================
//
// �����蔻��̏���[Enemy.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "collision.h"

//========================================================================
// �~�̓����蔻��(X�� Y��)
//========================================================================
bool ClsCircleXY(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;
	bool bUse = false;

	//�G�ƃv���C���[�Ԃ̑Ίp���̒������Z�o����
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->y - pPos2->y) * (pPos1->y - pPos2->y));
	
	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//�g�p����
	}

	return bUse;
}

//========================================================================
// �~�̓����蔻�� x�� z��
//========================================================================
bool ClsCircleXZ(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2)
{
	float fLength;				//���a�̌v�Z���ʂ̕ۑ��p
	bool bUse = false;			//false�ɏ�����(����p)

	//�G�ƃv���C���[�Ԃ̑Ίp���̒������Z�o����
	fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
					(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//�g�p����
	}

	return bUse;
}

//========================================================================
// �~�̓����蔻�� x�� z�� (���ނƂ̓�����)
//========================================================================
bool ClsCircleXZAllies(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, float fLength1, float fLength2, int MaxCnt)
{
	float fLength;				//���a�̌v�Z���ʂ̕ۑ��p
	bool bUse = false;			//false�ɏ�����(����p)

	//���ނƂ̑Ίp���̒������Z�o����
	for (int nCnt = 0; nCnt < MaxCnt; nCnt++, pPos2++)
	{//poa1�̃J�E���g��i�߂�
		fLength = sqrtf((pPos1->x - pPos2->x) * (pPos1->x - pPos2->x) +
						(pPos1->z - pPos2->z) * (pPos1->z - pPos2->z));
	}

	if (fLength <= (fLength1 / 2) + (fLength1 / 2))
	{
		bUse = true;			//�g�p����
	}

	return bUse;
}

//========================================================================
// ��l�̕Ǐ���
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
